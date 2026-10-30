# Water Quality Detection and Monitoring System

An embedded water-quality monitoring prototype that reads pH, total dissolved solids (TDS), turbidity, and temperature, displays measurements locally, sends readings to Blynk, and uses a trained Random Forest classifier to predict a dataset-defined quality class.

> **Important:** This is a student prototype, not a certified drinking-water test. The model predicts labels from the included dataset (`Low_Quality`, `Medium_Quality`, `High_Quality`). Those labels should not be interpreted as proof that water is potable or safe for consumption without validated reference measurements and appropriate standards-based testing.

## Features

- Four input parameters: pH, TDS, turbidity, and temperature.
- Microcontroller firmware for sensor acquisition and LCD display.
- Blynk integration for remote readings and notification/status updates.
- Random Forest model with a saved MinMaxScaler and LabelEncoder.
- Jupyter notebooks for training, evaluation, and prediction.

## System overview

1. Sensors provide analog measurements for pH, TDS, and turbidity; a DS18B20-compatible temperature sensor provides temperature.
2. Firmware processes the sensor readings and displays them on an I²C LCD.
3. The firmware publishes readings to Blynk virtual pins V0–V3 when the send button is enabled.
4. The Python prediction notebook retrieves readings from Blynk, scales the four features, predicts a class, and can publish the prediction to Blynk V6.

### Feature order expected by the model

The training and prediction code uses this exact column order:

1. `pH`
2. `TDS`
3. `Turbidity`
4. `Temperature`

Keep units and calibration consistent between the dataset and live sensors. The firmware currently uses simple `map()` conversions, so calibrate these conversions against known reference solutions/meters before using the measurements for meaningful assessment.

## Repository structure

```text
water-quality-detection-system/
├── README.md
├── requirements.txt
├── .gitignore
├── .env.example
├── notebooks/
│   ├── Train.ipynb
│   └── Prediction.ipynb
├── src/
│   └── predict_local.py
├── firmware/
│   └── water_quality_monitor.ino
├── data/
│   ├── dataset.csv
│   └── README.md
├── models/
│   ├── rf_model.pkl
│   ├── scaler.pkl
│   └── label_encoder.pkl
├── docs/
│   └── README.md
└── results/
    └── README.md
```

## Hardware

The provided firmware uses ESP32-specific libraries (`WiFi.h` and `BlynkSimpleEsp32.h`), analog input pins 34, 35, and 39, GPIO 15 for the OneWire temperature sensor, and an I²C LCD at address `0x27`. **Confirm the exact board used in the physical build before publishing this description.** The supplied firmware does not currently show RP2040 or ESP8266-specific setup.

Expected components based on the firmware:
- ESP32-compatible development board
- pH sensor module
- TDS sensor module
- Turbidity sensor module
- DS18B20-compatible temperature sensor
- I²C LCD
- Wi-Fi access and Blynk account

## Software requirements

- Python 3.10+ recommended
- Jupyter Notebook
- pandas, NumPy, scikit-learn, matplotlib, seaborn, joblib, requests
- Arduino IDE with the libraries listed below

Arduino libraries used:
- Blynk
- LCD_I2C
- OneWire
- DallasTemperature

## Run the machine-learning notebook

1. Install Python and create a virtual environment if desired.
2. Install dependencies:

   ```bash
   pip install -r requirements.txt
   ```

3. Start Jupyter:

   ```bash
   jupyter notebook
   ```

4. Open `notebooks/Train.ipynb`.
5. Ensure the notebook loads `data/dataset.csv` (adjust its relative path to `../data/dataset.csv` if running from the `notebooks` folder).
6. Run the cells in order to train and evaluate the model.

The included model files were exported from the supplied project. Only load pickle/joblib files that you trust, because serialized Python models can execute code when loaded.

## Run a local prediction

From the repository root:

```bash
python src/predict_local.py --ph 7.0 --tds 463 --turbidity 35 --temperature 28
```

## Blynk setup and secrets

**Never commit Wi-Fi passwords, Blynk auth tokens, or API tokens to GitHub.** The originally supplied files contained credentials. They have been replaced with placeholders in this cleaned copy. Rotate/revoke the previously embedded Blynk tokens and change the Wi-Fi password if it is still active, because the credentials were present in the uploaded archive.

For the prediction notebook, set an environment variable locally before running it:

**Windows PowerShell**
```powershell
$env:BLYNK_TOKEN = "your_new_blynk_token"
jupyter notebook
```

For firmware, replace `YOUR_WIFI_SSID`, `YOUR_WIFI_PASSWORD`, and `YOUR_BLYNK_AUTH_TOKEN` locally in the Arduino IDE. Do not commit those replacements.

The firmware and prediction notebook use virtual pins V0 (pH), V1 (TDS), V2 (turbidity), V3 (temperature), and V6 (prediction upload) in the provided code. Confirm these mappings in your Blynk template.

## Results and interpretation

The original training notebook reports **99.90% accuracy** for both Random Forest and Gradient Boosting on a random 80/20 train/test split of the supplied 20,000-row dataset. Treat this as a result reported by the existing notebook, not as independently verified real-world performance. Before presenting it as a reliable result, verify the dataset source and label-generation method, check for leakage or overly simple label rules, and evaluate on independently collected, calibrated sensor data.

The dataset has 20,000 rows and columns `pH`, `TDS`, `Turbidity`, `Temperature`, and `Nutrient Level`; there are no missing values in the supplied CSV. The target labels are imbalanced, so report per-class precision/recall and a confusion matrix alongside accuracy.

## Known limitations and next improvements

- Calibrate sensor conversions and document measurement units.
- Validate pH/TDS/turbidity values against reference instruments and standards.
- Verify what the target labels mean and how they were generated.
- Evaluate on independent real sensor readings; do not rely on accuracy alone.
- Add a confusion matrix, per-class metrics, and reproducible train/test split.
- Add wiring and architecture diagrams only after verifying the actual connections.
- Add error handling/timeouts for network requests and test the Blynk integration with a newly generated token.
- Avoid describing the output as “safe to drink” unless validated against applicable drinking-water standards.

## License

........
