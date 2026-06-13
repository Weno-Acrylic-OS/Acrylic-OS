#!/usr/bin/env python3

from flask import Flask
from flask_sock import Sock
import json

app = Flask(__name__)
sock = Sock(app)

# A simple list to hold all active WebSocket clients
clients = []

@sock.route('/ws')
def weno_car_socket(ws):
    """
    Handles WebSocket connections. Adds new clients to the list and
    broadcasts received messages to all other clients.
    """
    clients.append(ws)
    print(f"Client connected. Total clients: {len(clients)}")
    
    try:
        while True:
            data = ws.receive()
            print(f"Received message: {data}")
            # Broadcast the message to all other clients
            for client in clients:
                if client != ws:
                    try:
                        client.send(data)
                        print(f"Sent message to a client.")
                    except Exception as e:
                        print(f"Error sending to client: {e}")
    except ConnectionResetError:
        print("Client connection reset.")
    finally:
        # Remove the client from the list upon disconnection
        clients.remove(ws)
        print(f"Client disconnected. Total clients: {len(clients)}")

if __name__ == '__main__':
    # Runs on port 5003 to avoid conflicts
    app.run(host='0.0.0.0', port=5003, debug=False)
