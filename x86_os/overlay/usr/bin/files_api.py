#!/usr/bin/env python3

import os
from flask import Flask, jsonify, request
from flask_cors import CORS

try:
    import dbus
except ModuleNotFoundError:
    dbus = None

app = Flask(__name__)
CORS(app)

# --- Files API ---

ROOT_DIR = "/home/default" 

@app.route('/api/fs', methods=['GET'])
def list_files():
    requested_path = request.args.get('path', '/')
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
            items.append({"name": item, "type": "directory" if is_dir else "file"})
        return jsonify({"path": requested_path, "items": items})
    except FileNotFoundError:
        return jsonify({"error": "Path not found"}), 404
    except Exception as e:
        return jsonify({"error": str(e)}), 500

# --- OTA Update API ---

@app.route('/api/ota/install', methods=['POST'])
def ota_install():
    """
    Triggers a RAUC update via D-Bus.
    Expects a JSON body with the URL of the update bundle.
    e.g., {"url": "http://example.com/update.raucb"}
    """
    data = request.get_json()
    if not data or 'url' not in data:
        return jsonify({"error": "Missing 'url' in request body"}), 400

    if dbus is None:
        return jsonify({
            "error": "Update service is unavailable on this build.",
            "details": "The Python 'dbus' module is not installed in the Acrylic OS image."
        }), 501
    
    bundle_url = data['url']
    
    try:
        bus = dbus.SystemBus()
        # Get a proxy for the RAUC D-Bus object
        rauc_proxy = bus.get_object('de.pengutronix.rauc', '/')
        # Get the interface
        installer_iface = dbus.Interface(rauc_proxy, 'de.pengutronix.rauc.Installer')
        
        print(f"Triggering RAUC install for bundle: {bundle_url}")
        # Call the 'InstallBundle' method
        installer_iface.InstallBundle(bundle_url)
        
        return jsonify({"message": "Update installation started."})
    except dbus.exceptions.DBusException as e:
        print(f"D-Bus error: {e}")
        return jsonify({"error": "Failed to communicate with update service.", "details": str(e)}), 500
    except Exception as e:
        print(f"Generic error in OTA install: {e}")
        return jsonify({"error": "An unexpected error occurred.", "details": str(e)}), 500


if __name__ == '__main__':
    # Make sure the root directory for files exists
    if not os.path.exists(ROOT_DIR):
        os.makedirs(ROOT_DIR)
        with open(os.path.join(ROOT_DIR, "welcome.txt"), "w") as f:
            f.write("Welcome to Acrylic OS!")

    app.run(host='0.0.0.0', port=5001)
