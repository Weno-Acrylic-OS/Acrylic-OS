#!/usr/bin/env python3

import os
from flask import Flask, jsonify, request
from flask_cors import CORS

app = Flask(__name__)
# Allow CORS for all domains on all routes, which is fine for a local, isolated server.
CORS(app)

# Define a root directory to prevent directory traversal attacks.
# In the kiosk OS, the user's home directory would be a safe choice.
ROOT_DIR = "/home/default" 

@app.route('/api/fs', methods=['GET'])
def list_files():
    """
    Lists files and directories at a given path.
    'path' query parameter should be a sub-path relative to the ROOT_DIR.
    """
    requested_path = request.args.get('path', '/')
    
    # Security: Ensure the requested path is safe and within the root directory.
    # os.path.abspath will resolve '..' components.
    # os.path.join handles leading slashes safely.
    base_path = os.path.abspath(ROOT_DIR)
    full_path = os.path.abspath(os.path.join(base_path, requested_path.lstrip('/')))

    if not full_path.startswith(base_path):
        return jsonify({"error": "Access denied"}), 403

    try:
        if not os.path.isdir(full_path):
            return jsonify({"error": "Path is not a directory"}), 400

        items = []
        for item in os.listdir(full_path):
            item_path = os.path.join(full_path, item)
            is_dir = os.path.isdir(item_path)
            items.append({
                "name": item,
                "type": "directory" if is_dir else "file"
            })
        
        return jsonify({
            "path": requested_path,
            "items": items
        })

    except FileNotFoundError:
        return jsonify({"error": "Path not found"}), 404
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    # Make sure the root directory exists
    if not os.path.exists(ROOT_DIR):
        os.makedirs(ROOT_DIR)
        # Create a dummy file for demonstration
        with open(os.path.join(ROOT_DIR, "welcome.txt"), "w") as f:
            f.write("Welcome to Acrylic OS!")

    app.run(host='0.0.0.0', port=5001)
