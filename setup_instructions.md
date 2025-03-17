# Blynk Configuration Guide

1. **Create a Blynk Template**
   - Open Blynk Console
   - Create a new template and note the Template ID, Name, and Auth Token

2. **Datastream Setup**
   - Add these virtual pins:
     - `V0` → Reminder Hours (Integer, Min: 0, Max: 24)
     - `V1` → Reminder Minutes (Integer, Min: 0, Max: 60)
     - `V2` → Buzzer Duration (Integer, Min: 0, Max: 60)
     - `V3` → IR Hand Detection (String)
     - `V4` → LCD Display (String)

3. **Upload Code**
   - Modify `BLYNK_AUTH_TOKEN`, `WiFi` credentials
   - Upload `main_code.ino` to ESP8266

4. **Test the Project**
   - Open Serial Monitor
   - Check if time updates correctly
   - Set Reminder Time via Blynk App
   - Wait for the alarm to trigger