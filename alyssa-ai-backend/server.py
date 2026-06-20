from flask import Flask, request, jsonify
from flask_cors import CORS
from sentence_transformers import SentenceTransformer, util
import torch

app = Flask(__name__)
CORS(app) # Enable CORS for all routes

# --- AI Model and Capabilities Setup ---

# Load a small, efficient sentence-similarity model.
# This will be downloaded on first run.
print("Loading sentence similarity model...")
model = SentenceTransformer('all-MiniLM-L6-v2')
print("Model loaded.")

# Define the "intents" that our AI understands.
capabilities = {
    "get_weather": "What is the weather like?",
    "get_time": "What time is it?",
    "open_calculator": "Open the calculator app",
    "tell_joke": "Tell me a joke",
}

# Pre-compute the embeddings for our capabilities for efficiency.
# This is a key optimization.
print("Computing capability embeddings...")
capability_names = list(capabilities.keys())
capability_descriptions = list(capabilities.values())
capability_embeddings = model.encode(capability_descriptions, convert_to_tensor=True)
print("Embeddings computed.")


# --- API Endpoint ---

@app.route('/api/ask', methods=['POST'])
def ask_alyssa():
    data = request.get_json()
    if not data or 'prompt' not in data:
        return jsonify({"error": "Missing 'prompt' in request body"}), 400
    
    prompt = data['prompt']
    
    # Encode the user's prompt into an embedding.
    prompt_embedding = model.encode(prompt, convert_to_tensor=True)
    
    # Compute cosine similarity between the prompt and all capabilities.
    cosine_scores = util.cos_sim(prompt_embedding, capability_embeddings)
    
    # Find the highest score and its index.
    top_score, top_idx = torch.max(cosine_scores, dim=-1)
    
    top_score = top_score.item()
    top_idx = top_idx.item()
    
    # Define a confidence threshold.
    CONFIDENCE_THRESHOLD = 0.5
    
    if top_score > CONFIDENCE_THRESHOLD:
        intent = capability_names[top_idx]
        print(f"Prompt: '{prompt}' -> Intent: {intent} (Score: {top_score:.2f})")
        return jsonify({"intent": intent, "confidence": top_score})
    else:
        print(f"Prompt: '{prompt}' -> Intent: unknown (Score: {top_score:.2f})")
        return jsonify({"intent": "unknown", "confidence": top_score})

if __name__ == '__main__':
    # Run on a different port to avoid conflicts
    app.run(host='0.0.0.0', port=5004)
