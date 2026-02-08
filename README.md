# Health-Monitoring-System
IoT-based Worker Safety Wearable using ESP32 to monitor heart rate, SpO₂, motion, temperature, humidity, and air quality. It triggers buzzer alerts on unsafe conditions and sends real-time data to the Blynk dashboard for remote monitoring, providing an affordable and reliable safety solution for hazardous workplaces.

📌 Project Description
The Worker Safety Wearable System is an IoT-based smart monitoring device designed to enhance safety in hazardous work environments. It continuously monitors the worker’s health parameters and surrounding conditions in real time.
The system measures:
--> Heart rate (BPM)
--> Blood oxygen level (SpO₂)
--> Body movement and motion
--> Temperature and humidity
--> Air quality
If any parameter crosses a safe threshold, the system instantly triggers an alert using a buzzer and sends live data to the Blynk IoT dashboard for remote supervision.
This project provides a low-cost, portable, and effective solution to protect workers in industries such as construction, mining, chemical plants, and factories.

🔧 Features
✔ Real-time health monitoring
✔ Environmental condition tracking
✔ Automatic emergency alerts
✔ Wireless IoT connectivity
✔ Remote data visualization via Blynk
✔ Low power and portable design
✔ Continuous data logging

🧩 Hardware Components
Component                        Purpose                              
ESP32                            Main microcontroller & WiFi connectivity 
MAX30102                         Heart rate and SpO₂ sensor               
MPU6050                          Motion and acceleration monitoring       
DHT11                            Temperature & humidity sensing           
MQ135                            Air quality monitoring                   
Buzzer                           Emergency alert system                   
OLED / Serial Monitor            Data display                             
Connecting wires & power supply  Circuit integration                      

🛠 Software & Tools Used

--> Arduino IDE
--> ESP32 Board Package
--> Blynk IoT Platform
--> C++ (Arduino Programming)
--> MAX30102 & MPU6050 Libraries
--> WiFi and IoT Communication Protocols

⚙️ Working Principle

1. All sensors collect real-time data from the worker and environment.
2. ESP32 processes the sensor readings.
3. Data is analyzed against predefined safety thresholds.
4. If unsafe conditions are detected, the buzzer turns ON.
5. All readings are sent to the Blynk cloud dashboard for remote monitoring.
6. Supervisors can track worker safety from anywhere using a mobile app.

📲 Blynk Dashboard Integration
Virtual Pins Used:
 Parameter          Blynk Pin 
 MQ135 Air Quality  V0        
 Temperature        V1        
 Humidity           V2        
 BPM                V3        
 SpO₂               V4        
 MPU X              V5        
 MPU Y              V6        
 MPU Z              V7        
 Trigger Status     V8        

 🚨 Alert Conditions

The system activates the buzzer when:
--> Air quality exceeds safe limit
--> Temperature rises above threshold
--> Humidity is too high
--> Abnormal heart rate detected
--> Excessive body movement detected

📚 Required Libraries

--> Blynk
--> MAX3010
--> Adafruit MPU6050
--> DHT Sensor Library
--> Wire
--> Spo2 Algorithm

🧪 Future Enhancements
--> GPS tracking for location monitoring
--> SMS alert system
--> Cloud data analytics
--> Battery optimization
--> Machine learning-based health prediction

🧩 Applications

--> Industrial worker safety
--> Mining and construction sites
--> Hazardous chemical plants
--> Remote patient monitoring
--> Smart healthcare wearables

👨‍💻 Developed By

Vishnu Kumawat
IoT & Embedded Systems Enthusiast

⭐ If you like this project, please give it a star on GitHub!
