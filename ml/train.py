import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.ensemble import IsolationForest
from sklearn.preprocessing import StandardScaler
import joblib
import os

# -------------------------------
# Load dataset
# -------------------------------
DATA_PATH = "data/dataset_augmented.csv"
OUTPUT_DIR = "ml/model"

os.makedirs(OUTPUT_DIR, exist_ok=True)

df = pd.read_csv(DATA_PATH)

# Drop non-feature columns
if 'timestamp' in df.columns:
    df.drop(columns=['timestamp'], inplace=True)

y_true = None
if 'machine_status' in df.columns:
    y_true = df['machine_status']
    df.drop(columns=['machine_status'], inplace=True)

# Select features
X = df[['temperature', 'acc_x', 'acc_y', 'acc_z']]

# -------------------------------
# Feature scaling
# -------------------------------
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# -------------------------------
# Isolation Forest Model
# -------------------------------
model = IsolationForest(
    n_estimators=100,
    contamination=0.1,
    random_state=42
)
model.fit(X_scaled)

# Predict anomalies
#  1  -> Normal
# -1  -> Anomaly
y_pred = model.predict(X_scaled)

# -------------------------------
# Visualization
# -------------------------------
plt.figure(figsize=(10, 6))
plt.plot(X['temperature'], label='Temperature')
plt.scatter(
    X.index[y_pred == -1],
    X['temperature'][y_pred == -1],
    color='red',
    label='Anomaly',
    s=10
)
plt.title("Temperature with Detected Anomalies")
plt.legend()
plt.tight_layout()
plt.savefig("results/temperature_anomalies.png")
plt.close()

# -------------------------------
# Save model artifacts
# -------------------------------
joblib.dump(model, f"{OUTPUT_DIR}/isolation_forest.pkl")
joblib.dump(scaler, f"{OUTPUT_DIR}/scaler.pkl")

# -------------------------------
# Report
# -------------------------------
with open("results/training_report.txt", "w") as f:
    f.write("Isolation Forest Anomaly Detection Report\n")
    f.write("----------------------------------------\n")
    f.write(f"Total samples: {len(X)}\n")
    f.write(f"Detected anomalies: {(y_pred == -1).sum()}\n")
    f.write("Features used: temperature, acc_x, acc_y, acc_z\n")

print("✅ Isolation Forest model trained and saved successfully.")
