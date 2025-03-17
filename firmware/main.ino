#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "RTClib.h"

// Replace with your Blynk credentials
#define BLYNK_TEMPLATE_ID "Your_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Your_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "Your_BLYNK_AUTH_TOKEN"

// Replace with your WiFi credentials
char ssid[] = "Your_WiFi_Name";
char pass[] = "Your_WiFi_Password";

LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

#define BUZZER D7
#define LED D6
#define IR_SENSOR D5

#define V0_HOUR_INTERVAL V0
#define V1_MIN_INTERVAL V1
#define V2_BUZZER_TIME V2
#define V3_HAND_DETECTED V3
#define V4_TERMINAL V4

int hourInterval, minInterval, buzzerTime;
bool alarmTriggered = false;
unsigned long alarmTime = 0;

BLYNK_WRITE(V0_HOUR_INTERVAL) { hourInterval = param.asInt(); }
BLYNK_WRITE(V1_MIN_INTERVAL) { minInterval = param.asInt(); }
BLYNK_WRITE(V2_BUZZER_TIME) { buzzerTime = param.asInt(); }

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(IR_SENSOR, INPUT);

  lcd.begin(16, 2);
  lcd.backlight();

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (rtc.lostPower())
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop()
{
  Blynk.run();
  DateTime now = rtc.now();

  int nextHour = (now.hour() + hourInterval) % 24;
  int nextMinute = (now.minute() + minInterval) % 60;

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(now.hour());
  lcd.print(":");
  lcd.print(now.minute());

  lcd.setCursor(0, 1);
  lcd.print("Next Dose: ");
  lcd.print(nextHour);
  lcd.print(":");
  lcd.print(nextMinute);

  if (now.hour() == nextHour && now.minute() == nextMinute && !alarmTriggered)
  {
    alarmTriggered = true;
    alarmTime = millis();
    digitalWrite(BUZZER, HIGH);
    digitalWrite(LED, HIGH);
  }

  if (digitalRead(IR_SENSOR) == LOW)
  {
    lcd.setCursor(0, 1);
    lcd.print("Hand Detected  ");
    digitalWrite(BUZZER, LOW);
    digitalWrite(LED, LOW);
    alarmTriggered = false;
  }
}