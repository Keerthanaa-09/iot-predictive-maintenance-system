# IoT-Based Predictive Maintenance System for Medical Equipment

## Overview
An end-to-end IoT-based predictive maintenance solution designed for medical equipment monitoring.  
The system continuously tracks temperature and vibration parameters, streams data to the cloud, visualizes equipment health in real time, and applies machine learning to identify abnormal operating conditions.

## System Architecture
```text
Sensors (DS18B20, ADXL345)
        ↓
ESP8266 (WiFi)
        ↓
Adafruit IO (Cloud – MQTT)
        ↓                ↓
   Python ML		Node-RED
 (Isolation Forest	(Monitoring &
   Anomaly Detection)	Alerts)
```  


## Technology Stack
- **Hardware:** ESP8266, DS18B20, ADXL345  
- **Cloud:** Adafruit IO (MQTT)  
- **Visualization:** Node-RED Dashboard  
- **Analytics:** Python, Isolation Forest  

## Key Capabilities
- Real-time monitoring of medical equipment parameters  
- Cloud-based sensor data ingestion using MQTT  
- Live dashboards with threshold-based alerts  
- Machine learning–based anomaly detection  
- Supports predictive maintenance and early fault detection  

## Working Principle
Sensor data from medical equipment is transmitted via ESP8266 to a cloud MQTT platform.  
The same data stream is consumed in parallel by:
- **Node-RED** for real-time visualization and alerts  
- **Python ML module** for detecting anomalous behavior  

This architecture enables continuous monitoring and intelligent decision support.

## Repository Structure
```text
iot-predictive-maintenance/
├── README.md
│
├── LICENSE
|
├── iot/
│   ├── esp8266_temperature.ino
│   ├── esp8266_vibration.ino
│   └── README.md
│
├── data/
│   ├── dataset.csv
│   ├── dataset1.csv
│   ├── dataset_augmented.csv
│   └── README.md
│
├── ml/
│   ├── train.py
│   ├── predict.py
│   ├── README.md
│   └── model/
│       ├── model.pkl
│       └── scaler.pkl
│
├── nodered/
    ├── README.md
    └── screenshots/
        ├── temperature_flow.png
        ├── vibration_flow.png
        └── dashboard.png
```

## Outcome
- Improved reliability of medical equipment  
- Early identification of abnormal operating conditions  
- Reduced downtime through predictive maintenance  

## About the Project
Developed as an academic and practical implementation of IoT-enabled predictive maintenance for medical equipment, combining embedded systems, cloud communication, dashboards, and machine learning.


