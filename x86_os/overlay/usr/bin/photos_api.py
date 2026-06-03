#!/usr/bin/env python3

import os
from flask import Flask, jsonify, send_from_directory
from flask_cors import CORS
from PIL import Image

app = Flask(__name__)
CORS(app)

# --- Configuration ---
# The directory where original, full-resolution photos are stored.
# In a real system, this might be /home/default/Pictures
PHOTOS_DIR = "/home/default/Pictures"
# The directory where we'll store generated thumbnails.
THUMBNAIL_DIR = "/home/default/.thumbnails"
THUMBNAIL_SIZE = (128, 128)

def ensure_dirs():
    """Ensure the photos and thumbnails directories exist."""
    os.makedirs(PHOTOS_DIR, exist_ok=True)
    os.makedirs(THUMBNAIL_DIR, exist_ok=True)

def create_thumbnail(image_path, thumbnail_path):
    """Creates a thumbnail for a given image."""
    try:
        with Image.open(image_path) as img:
            img.thumbnail(THUMBNAIL_SIZE)
            img.save(thumbnail_path, "JPEG")
        return True
    except Exception as e:
        print(f"Could not create thumbnail for {image_path}: {e}")
        return False

@app.route('/api/photos', methods=['GET'])
def list_photos():
    """
    Scans the photos directory, generates thumbnails if needed,
    and returns a list of all photos.
    """
    ensure_dirs()
    photos = []
    
    # Add a dummy image if the directory is empty
    if not any(fname.lower().endswith(('.png', '.jpg', '.jpeg')) for fname in os.listdir(PHOTOS_DIR)):
         with Image.new('RGB', (600, 400), color = 'red') as img:
            img.save(os.path.join(PHOTOS_DIR, 'example.jpg'))

    for filename in sorted(os.listdir(PHOTOS_DIR)):
        if not filename.lower().endswith(('.png', '.jpg', '.jpeg')):
            continue

        thumbnail_filename = f"thumb_{filename}.jpg"
        full_image_path = os.path.join(PHOTOS_DIR, filename)
        thumbnail_path = os.path.join(THUMBNAIL_DIR, thumbnail_filename)

        # Create thumbnail if it doesn't exist
        if not os.path.exists(thumbnail_path):
            create_thumbnail(full_image_path, thumbnail_path)
        
        photos.append({
            "id": filename,
            "full_url": f"/api/photos/full/{filename}",
            "thumbnail_url": f"/api/photos/thumbnail/{thumbnail_filename}"
        })
        
    return jsonify(photos)

@app.route('/api/photos/full/<path:filename>')
def get_full_image(filename):
    """Serves a full-resolution image."""
    return send_from_directory(PHOTOS_DIR, filename)

@app.route('/api/photos/thumbnail/<path:filename>')
def get_thumbnail_image(filename):
    """Serves a thumbnail image."""
    return send_from_directory(THUMBNAIL_DIR, filename)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5002)
