# Medicine Alert IoT Project using ESP8266

## 📌 Project Overview
Medicine Alert IoT Project is an innovative system that reminds patients to take their medicine on time using an ESP8266 microcontroller, Blynk IoT platform, and various sensors. The system triggers an alert via a buzzer and LED and displays the reminder on an LCD screen. The schedule is controlled via a mobile app using Blynk.

---

## 🚀 Features
✅ Medicine reminder alerts using buzzer & LED 💡🔔  
✅ Blynk mobile app integration 📱  
✅ LCD display for real-time medicine schedule 📟  
✅ IR sensor-based hand detection for acknowledgment ✋  
✅ Configurable reminder times via the app ⏳

---

## 🛠 Required Components
| Component                | Quantity |
|--------------------------|----------|
| ESP8266 (NodeMCU)       | 1        |
| Buzzer                  | 1        |
| IR Sensor               | 1        |
| 16x2 I2C LCD Display    | 1        |
| LED                     | 1        |
| Resistors (220Ω)        | 2        |
| Jumper Wires            | As needed |
| Breadboard (or PCB)     | 1        |

---

## 🔌 Circuit Diagram

####circuit diagram link: (https://app.cirkitdesigner.com/project/aaa6dcc6-c725-4fbb-88c0-d720f7cd692f)
![circuit_image](https://github.com/user-attachments/assets/236936b7-e02e-4beb-bf75-cb63639b72f8)


---

## 📲 Blynk Configuration
### **1️⃣ Create a New Template in Blynk**
1. Open [Blynk Console](https://blynk.cloud/)
2. Create a new template → Name it "Medicine Alert IoT"
3. Add Datastreams as shown below:

| Name               | Pin  | Type   | Min | Max |
|--------------------|------|--------|-----|-----|
| Reminder Hours    | V0   | Integer | 0   | 24  |
| Reminder Minutes  | V1   | Integer | 0   | 60  |
| Buzzer Duration   | V2   | Integer | 0   | 60  |
| IR Hand Detection | V3   | String  | -   | -   |
| LCD Display       | V4   | String  | -   | -   |

### **2️⃣ Setup Blynk Mobile App**
1. Open the **Blynk app**
2. Create a new project and link it to the template
3. Add necessary widgets (Buttons, LCD, etc.)
4. Link widgets to the correct datastreams

---

## 📜 Code & Configuration
The main firmware code is inside the `firmware/main_code.ino` file.

### 🔹 **How to Configure the Code**
1. Open `firmware/main_code.ino`
2. Update the following lines with your own credentials:

```cpp
// Replace with your Blynk Template information
#define BLYNK_TEMPLATE_ID "Your_Blynk_Template_ID"
#define BLYNK_TEMPLATE_NAME "Your_Blynk_Template_Name"
#define BLYNK_AUTH_TOKEN "Your_Blynk_Auth_Token"

// Replace with your WiFi credentials
#define WIFI_SSID "Your_WiFi_Name"
#define WIFI_PASS "Your_WiFi_Password"
```

3. Upload the code to your ESP8266 board

---

## 📦 Required Libraries
Install the following libraries before running the code:
- **Blynk Library** → [Download Here](https://github.com/blynkkk/blynk-library)
- **ESP8266WiFi** (Default in Arduino IDE)
- **LiquidCrystal_I2C** → [Download Here](https://github.com/johnrickman/LiquidCrystal_I2C)
- **Wire Library** (Default in Arduino IDE)

---

## 🛠 Installation Steps
1. Clone the repository:
   ```sh
   git clone [https://github.com/mhdnazrul/Medicine-Alert-IoT-Project-using-ESP8266.git]
   ```
2. Open `firmware/main_code.ino` in **Arduino IDE**
3. Install required libraries
4. Configure WiFi & Blynk credentials
5. Upload the code to ESP8266
6. Power up the system and connect to Blynk
7. Enjoy your automated medicine reminder! 🎉

---

## 📸 Screenshots
Upload Blynk mobile app screenshots in `assets/blynk_screenshots/`
![Datastreams Image](https://github.com/user-attachments/assets/31a466f4-7b01-4ec4-9513-e80da8b98f2f)

![Dashboard Image](https://github.com/user-attachments/assets/7b56859c-2041-4ecc-a90c-29afe54ba0fe)
---

---

## 📝 License
This project is open-source and licensed under the MIT License.
