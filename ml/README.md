## Machine Learning

This folder contains the **machine learning pipeline** used for **anomaly detection**.

- **train.py** – Trains the **Isolation Forest** model using sensor data  
- **predict.py** – Performs anomaly prediction on incoming data  
- **model/** – Stores trained artifacts used during inference  
  - **model.pkl** – Trained anomaly detection model  
  - **scaler.pkl** – Feature scaler used during training and inference
