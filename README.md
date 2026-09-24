# Smart Pill Dispenser

Arduino Uno coursework prototype by Team 20: Kriman Kaur, Dhru Patel, Puneet, and Charan. The [project report](Smart%20Pill%20Dispenser.pdf) describes the concept. This repository contains two **separate sketches**, not a tested, integrated medication device.

| Sketch | What the code does | Parts and connections visible in the sketch |
| --- | --- | --- |
| [`pill_dispenser.ino`](pill_dispenser.ino) | At 08:00, 14:00, and 20:00, asks for a recognized fingerprint, turns a stepper to a compartment, then opens a servo for three seconds. | DS3231 RTC; fingerprint reader on software serial pins 2/3; servo signal on pin 5; stepper control on pins 7/12/13/10. |
| [`gsm.ino`](gsm.ino) | Attempts to send an SMS reminder at 09:00, 14:00, and 21:00. | DS1307 RTC; GSM serial on pins 7/8. The modem model is not specified. |

## Reproducing the sketches

1. Select an Arduino Uno board in the Arduino IDE. Install **RTClib** and **Adafruit Fingerprint Sensor Library** for the dispenser sketch. `Stepper`, `Servo`, `SoftwareSerial`, and `Wire` come from the Arduino core or standard libraries.
2. Wire and power the modules for the **one sketch** you are testing, using its pin list above and the modules' own voltage/current requirements. The sketches reuse pin 7 and expect different RTC models, so they cannot simply be combined.
3. For the GSM sketch, install **TinyGSM**, identify the actual modem, and configure the matching `TINY_GSM_MODEM_*` definition required by that library before compiling. Replace the example APN and recipient in the source with your own test details. Without a known modem, this sketch has not been validated for an Arduino Uno.
4. Open the selected `.ino` file, compile for Uno, and test with a non-medication load while watching the serial output at 9600 baud. Fingerprint enrollment, RTC time setting, motor power, and physical compartment calibration are not supplied by these files.

The sketches keep their daily state only in RAM. They do not measure pill count, confirm that a pill was taken, detect jams, or provide verified delivery or medical reliability. Do not use this prototype to manage real medication.
