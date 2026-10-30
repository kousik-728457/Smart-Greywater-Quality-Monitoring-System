"""Run a local prediction using the saved Random Forest model."""
import argparse
from pathlib import Path

import joblib
import pandas as pd

ROOT = Path(__file__).resolve().parents[1]
MODEL_DIR = ROOT / "models"

def predict(ph: float, tds: float, turbidity: float, temperature: float) -> str:
    model = joblib.load(MODEL_DIR / "rf_model.pkl")
    scaler = joblib.load(MODEL_DIR / "scaler.pkl")
    encoder = joblib.load(MODEL_DIR / "label_encoder.pkl")
    features = pd.DataFrame([{
        "pH": ph,
        "TDS": tds,
        "Turbidity": turbidity,
        "Temperature": temperature,
    }])
    scaled = scaler.transform(features)
    label = model.predict(scaled)
    return str(encoder.inverse_transform(label)[0])

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--ph", type=float, required=True)
    parser.add_argument("--tds", type=float, required=True)
    parser.add_argument("--turbidity", type=float, required=True)
    parser.add_argument("--temperature", type=float, required=True)
    args = parser.parse_args()
    print("Predicted dataset class:", predict(args.ph, args.tds, args.turbidity, args.temperature))
    print("Note: this class is not a certification of drinking-water safety.")
