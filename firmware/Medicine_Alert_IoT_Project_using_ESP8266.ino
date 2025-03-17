#define BLYNK_TEMPLATE_ID "TMPL6fTBnpNWw"
#define BLYNK_TEMPLATE_NAME "Medicine Alert IoT Project using ESP8266"
#define BLYNK_AUTH_TOKEN "qtIuVU7EH4LW4tbvbqEG1OG2vTk_zR9g"

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "RTClib.h"

// WiFi Credentials
char ssid[] = "Creative House";
char pass[] = "Creative2025";

// LCD I2C Address (16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RTC Module
RTC_DS3231 rtc;

// Buzzer, LED, IR Sensor Pins
#define BUZZER    D7
#define LED       D6
#define IR_SENSOR D5

// Blynk Virtual Pins
#define VPIN_HOUR V0
#define VPIN_MIN  V1
#define VPIN_BUZZER_TIME V2
#define VPIN_HAND_DETECT V3
#define VPIN_TERMINAL    V4

// Global Variables
int hourInterval = 0, minInterval = 10;  // ডিফল্ট ১০ মিনিট
int buzzerTime = 5; // ডিফল্টে ৫ সেকেন্ড বাজার বাজবে
bool alarmTriggered = false;
unsigned long alarmStartMillis = 0;

// DS3231 থেকে সময় পড়ার জন্য একটি DateTime অবজেক্ট
DateTime nextDoseTime;
bool doseScheduled = false;  // পরবর্তী ডোজ সময় সেট করা হয়েছে কি না

// IR Sensor স্ট্যাটাস অপ্টিমাইজেশনের জন্য
String lastHandStatus = "";

// পরবর্তী ডোজ সময় নির্ধারণের ফাংশন
void scheduleNextDose() {
  DateTime now = rtc.now();
  // এখনকার সময় + hourInterval ঘণ্টা + minInterval মিনিট
  nextDoseTime = now + TimeSpan(0, hourInterval, minInterval, 0);
  doseScheduled = true;
  Serial.println("Next Dose Scheduled at: " + String(nextDoseTime.hour()) + ":" + String(nextDoseTime.minute()));
}

// Blynk Write Functions
BLYNK_WRITE(VPIN_HOUR) {
  hourInterval = param.asInt();
  scheduleNextDose(); // নতুন ঘন্টা ইন্টারভাল পেলে পরবর্তী ডোজ সময় আপডেট
}

BLYNK_WRITE(VPIN_MIN) {
  minInterval = param.asInt();
  scheduleNextDose(); // নতুন মিনিট ইন্টারভাল পেলে পরবর্তী ডোজ সময় আপডেট
}

BLYNK_WRITE(VPIN_BUZZER_TIME) {
  buzzerTime = param.asInt();
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, pass);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(IR_SENSOR, INPUT);

  lcd.begin(16, 2);
  lcd.backlight();

  // RTC Initialize
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // *** ম্যানুয়ালি RTC সময় সেট করতে চাইলে (শুধুমাত্র একবার): ***
  // উদাহরণ: ২০২৫-০৩-১৮, রাত ২০:২৪:০০
  // rtc.adjust(DateTime(2025, 3, 18, 20, 24, 0));
 rtc.adjust(DateTime(2025, 3, 18, 20, 55, 0));
  // RTC পাওয়ার হারালে, কম্পাইল টাইম সেট করবে:
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
  Blynk.run();

  // বর্তমান সময় RTC থেকে নেওয়া
  DateTime now = rtc.now();

  // LCD তে বর্তমান সময় দেখানো
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());

  // প্রথমবার ডোজ সময় নির্ধারিত না থাকলে সেট করে নাও
  if(!doseScheduled) {
    scheduleNextDose();
  }

  // LCD তে পরবর্তী ডোজ সময় দেখানো
  lcd.setCursor(0, 1);
  lcd.print("Next Dose: ");
  if(nextDoseTime.hour() < 10) lcd.print("0");
  lcd.print(nextDoseTime.hour());
  lcd.print(":");
  if(nextDoseTime.minute() < 10) lcd.print("0");
  lcd.print(nextDoseTime.minute());

  // Blynk Terminal এ তথ্য পাঠানো
  String timeInfo = "Current Time: " 
                    + String(now.hour()) + ":" + String(now.minute())
                    + " | Next Dose: "
                    + String(nextDoseTime.hour()) + ":" + String(nextDoseTime.minute());
  Blynk.virtualWrite(VPIN_TERMINAL, timeInfo);

  // যদি বর্তমান সময় >= nextDoseTime এবং অ্যালার্ম এখনো ট্রিগার হয়নি
  if ((now.unixtime() >= nextDoseTime.unixtime()) && !alarmTriggered) {
    alarmTriggered = true;
    alarmStartMillis = millis();
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
    Serial.println("Alarm Triggered!");
  }

  // IR Sensor এর মান
  int sensorValue = digitalRead(IR_SENSOR);
  String currentHandStatus = (sensorValue == LOW) ? "Hand Detected" : "No Hand Detected";

  // Blynk Virtual Pin-এ হাত সনাক্তকরণ স্ট্যাটাস শুধু পরিবর্তন হলে আপডেট
  if(currentHandStatus != lastHandStatus) {
    Blynk.virtualWrite(VPIN_HAND_DETECT, currentHandStatus);
    lastHandStatus = currentHandStatus;
  }

  // যদি হাত সনাক্ত হয় এবং অ্যালার্ম চালু থাকে
  if(sensorValue == LOW && alarmTriggered) {
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    alarmTriggered = false;

    // পরবর্তী ডোজ সময়কে আবার ইন্টারভাল যোগ করে সেট করি
    nextDoseTime = nextDoseTime + TimeSpan(0, hourInterval, minInterval, 0);
    Serial.println("Hand Detected! Next Dose Rescheduled.");
  }
  else {
    // নির্দিষ্ট buzzerTime পেরিয়ে গেলে অ্যালার্ম বন্ধ
    if(alarmTriggered && (millis() - alarmStartMillis > buzzerTime * 1000)) {
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED, LOW);
      alarmTriggered = false;

      // পরবর্তী ডোজ সময়ে ইন্টারভাল যোগ
      nextDoseTime = nextDoseTime + TimeSpan(0, hourInterval, minInterval, 0);
      Serial.println("Buzzer Timeout! Next Dose Rescheduled.");
    }
  }

  delay(1000); // ১ সেকেন্ড বিরতি
}
