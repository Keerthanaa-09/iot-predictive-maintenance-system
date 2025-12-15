import serial
import joblib
import pandas as pd
from datetime import datetime
import os

# -------------------------------
# Paths (relative, GitHub-safe)
# -------------------------------
MODEL_PATH = os.path.join("ml", "model", "model.pkl")
SCALER_PATH = os.path.join("ml", "model", "scaler.pkl")

# Load trained model and scaler
model = joblib.load(MODEL_PATH)
scaler = joblib.load(SCALER_PATH)

# -------------------------------
# Serial configuration
# -------------------------------
SERIAL_PORT = "COM9"   # change if needed (COM3, COM5, etc.)
BAUD_RATE = 9600

def parse_serial_line(line):
    parts = line.strip().split(',')
    if len(parts) != 4:
        return None
    try:
        return list(map(float, parts))
    except ValueError:
        return None

def main():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2)
    print("📡 Live anomaly prediction started...\n")

    while True:
        try:
            line = ser.readline().decode('utf-8', errors='ignore')
            data = parse_serial_line(line)

            if data:
                df_input = pd.DataFrame(
                    [data],
                    columns=['temperature', 'acc_x', 'acc_y', 'acc_z']
                )

                X_scaled = scaler.transform(df_input)
                prediction = model.predict(X_scaled)[0]  # 1 = normal, -1 = anomaly

                label = "✅ Normal" if prediction == 1 else "❌ Anomaly"
                print(
                    f"[{datetime.now().strftime('%H:%M:%S')}] "
                    f"Temp={data[0]:.2f}, "
                    f"V=({data[1]:.2f}, {data[2]:.2f}, {data[3]:.2f}) → {label}"
                )

        except KeyboardInterrupt:
            print("\n🛑 Stopped by user.")
            break
        except Exception as e:
            print("Error:", e)

    ser.close()

if __name__ == "__main__":
    main()
