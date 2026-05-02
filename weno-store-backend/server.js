const express = require('express');
const cors = require('cors');
const multer = require('multer');
const path = require('path');
const fs = require('fs');
const tar = require('tar');

const app = express();
const port = 3000;
const dbPath = path.join(__dirname, 'db.json');

// --- Database ---
let apps = [];
// Load the database from file on startup
try {
    if (fs.existsSync(dbPath)) {
        const dbData = fs.readFileSync(dbPath, 'utf8');
        apps = JSON.parse(dbData);
        console.log('App database loaded successfully.');
    } else {
        console.log('No app database found, starting fresh.');
    }
} catch (error) {
    console.error('Error loading app database:', error);
}

// Function to save the database to file
function saveDatabase() {
    try {
        fs.writeFileSync(dbPath, JSON.stringify(apps, null, 2));
        console.log('App database saved.');
    } catch (error) {
        console.error('Error saving app database:', error);
    }
}


// --- Middleware & Setup ---
const uploadsDir = path.join(__dirname, 'public', 'uploads');
fs.mkdirSync(uploadsDir, { recursive: true });

app.use(cors());
app.use(express.json());
app.use(express.urlencoded({ extended: true }));
app.use(express.static('public'));

const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        cb(null, uploadsDir);
    },
    filename: function (req, file, cb) {
        const appName = req.body.name ? req.body.name.replace(/\s+/g, '-').toLowerCase() : 'app';
        const fileExt = path.extname(file.originalname) || '.wfo';
        cb(null, `${appName}-${Date.now()}${fileExt}`);
    }
});
const upload = multer({ storage: storage });

// --- API Endpoints ---

app.get('/api/apps', (req, res) => {
    res.json(apps);
});

app.post('/api/apps', upload.single('appPackage'), (req, res) => {
    const { name, version, description } = req.body;
    const { file } = req;

    if (!name || !version || !description || !file) {
        return res.status(400).send('Missing app information or package file.');
    }

    const newApp = {
        id: apps.length > 0 ? Math.max(...apps.map(a => a.id)) + 1 : 1, // Ensure unique ID
        name,
        version,
        description,
        packageUrl: `/uploads/${file.filename}`
    };

    apps.push(newApp);
    saveDatabase(); // Save to file
    console.log('New app uploaded:', newApp);
    res.status(201).json(newApp);
});

app.get('/api/apps/source/:appName', (req, res) => {
    const { appName } = req.params;
    const app = apps.find(a => a.name.replace(/\s+/g, '-').toLowerCase() === appName.replace(/\s+/g, '-').toLowerCase());

    if (!app) {
        return res.status(404).send('App not found.');
    }

    const packagePath = path.join(__dirname, 'public', app.packageUrl);
    if (!fs.existsSync(packagePath)) {
        return res.status(404).send('App package file not found on server.');
    }

    let jsCode = '';
    try {
        tar.x({
            file: packagePath,
            sync: true,
            filter: (path, stat) => path === 'index.js',
            onentry: entry => {
                entry.on('data', chunk => {
                    jsCode += chunk.toString();
                });
            }
        });

        if (jsCode) {
            res.type('text/plain').send(jsCode);
        } else {
            res.status(500).send('Failed to extract index.js from package.');
        }
    } catch (error) {
        console.error('Error extracting from tar:', error);
        res.status(500).send('Server error during app extraction.');
    }
});


// --- Simple Frontend for Uploading ---

app.get('/', (req, res) => {
    res.send(`
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Weno Store - Developer Upload</title>
            <style>
                body { font-family: sans-serif; max-width: 600px; margin: 40px auto; padding: 20px; line-height: 1.6; }
                input, textarea { width: 100%; padding: 8px; margin-bottom: 10px; box-sizing: border-box; }
                button { padding: 10px 15px; }
            </style>
        </head>
        <body>
            <h1>Upload App to Weno Store</h1>
            <form action="/api/apps" method="POST" enctype="multipart/form-data">
                <label for="name">App Name:</label><br>
                <input type="text" id="name" name="name" required><br>
                
                <label for="version">Version:</label><br>
                <input type="text" id="version" name="version" required><br>
                
                <label for="description">Description:</label><br>
                <textarea id="description" name="description" rows="4" required></textarea><br>
                
                <label for="appPackage">App Package (.wfo file):</label><br>
                <input type="file" id="appPackage" name="appPackage" accept=".wfo" required><br><br>
                
                <button type="submit">Upload App</button>
            </form>
        </body>
        </html>
    `);
});

app.listen(port, () => {
    console.log(`Weno Store backend server listening at http://localhost:${port}`);
});
