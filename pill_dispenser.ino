#include <Stepper.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <RTClib.h>

const int stepsPerRevolution = 2048;
const int stepperPin1 = 7;
const int stepperPin2 = 12;
const int stepperPin3 = 13;
const int stepperPin4 = 10;
const int servoPin = 5;
const int dispenseDuration = 3000;
const int rxPin = 2;
const int txPin = 3;

Stepper stepper(stepsPerRevolution, stepperPin1, stepperPin2, stepperPin3, stepperPin4);
Servo servo;
RTC_DS3231 rtc;
SoftwareSerial mySerial(rxPin, txPin);
Adafruit_Fingerprint fingerprint = Adafruit_Fingerprint(&mySerial);

const int medicationHours[] = {8, 14, 20};
bool dispensedMedication[3] = {false, false, false};
int lastCheckDay = -1;

void setup() {
  stepper.setSpeed(10);
  
  servo.attach(servoPin);
  servo.write(0);
  
  Serial.begin(9600);
  while (!Serial);
  
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC module. Check your connections.");
    while (1);
  }
  
  mySerial.begin(57600);
  
  if (fingerprint.begin() != FINGERPRINT_OK) {
    Serial.println("Fingerprint sensor not found");
    while (1);
  }
  
  Serial.println("Fingerprint sensor initialized");
  Serial.println("Pill dispenser ready");
}

void loop() {
  DateTime now = rtc.now();
  
  if (now.day() != lastCheckDay) {
    for (int i = 0; i < 3; i++) {
      dispensedMedication[i] = false;
    }
    lastCheckDay = now.day();
    Serial.println("New day - medication schedule reset");
  }
  
  int currentHour = now.hour();
  int scheduleIndex = -1;
  
  for (int i = 0; i < 3; i++) {
    if (currentHour == medicationHours[i] && !dispensedMedication[i]) {
      scheduleIndex = i;
      Serial.println("It's time for medication. Please scan fingerprint.");
      break;
    }
  }
  
  if (scheduleIndex >= 0) {
    if (getFingerprintID() > 0) {
      dispensePills(scheduleIndex);
      dispensedMedication[scheduleIndex] = true;
    }
  }
  
  delay(1000);
}

int getFingerprintID() {
  uint8_t p = fingerprint.getImage();
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  
  p = fingerprint.image2Tz();
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  
  p = fingerprint.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    Serial.println("Fingerprint not recognized");
    return -1;
  }
  
  Serial.print("Fingerprint ID #"); 
  Serial.println(fingerprint.fingerID);
  return fingerprint.fingerID;
}

void dispensePills(int scheduleIndex) {
  Serial.print("Dispensing medication for schedule #");
  Serial.println(scheduleIndex + 1);
  
  int compartmentSteps = stepsPerRevolution / 4 * scheduleIndex;
  stepper.step(compartmentSteps);
  delay(500);
  
  servo.write(90);
  delay(dispenseDuration);
  servo.write(0);
  
  stepper.step(-compartmentSteps);
  
  Serial.println("Medication dispensed successfully");
}
