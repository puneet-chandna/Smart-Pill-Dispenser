#include <SoftwareSerial.h>
#include <TinyGsmClient.h>
#include <RTClib.h>

#define SerialMon Serial
SoftwareSerial SerialAT(7, 8);

#define APN "APN"
#define APN_USERNAME "APN_USERNAME"
#define APN_PASSWORD "APN_PASSWORD"

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
RTC_DS1307 rtc;

bool smsSent = false;

void setup() {
  SerialMon.begin(9600);
  SerialAT.begin(115200);
  delay(3000);
  
  if (!rtc.begin()) {
    SerialMon.println("Couldn't find RTC");
    while (1);
  }
}

void loop() {
  DateTime now = rtc.now();
  
  if ((now.hour() == 9 || now.hour() == 14 || now.hour() == 21) && now.minute() == 0) {
    if (!smsSent) {
      sendSMS();
      smsSent = true;
    }
  } else if (now.minute() != 0) {
    smsSent = false;
  }
  
  delay(10000);
}

void sendSMS() {
  modem.restart();
  
  if (!modem.waitForNetwork()) {
    return;
  }
  
  if (!modem.gprsConnect(APN, APN_USERNAME, APN_PASSWORD)) {
    return;
  }
  
  String phone = "+919704248005"; //this is a random phone no. (not mine) 
  String text = "Time to take pill";
  
  modem.sendSMS(phone, text);
  
  modem.gprsDisconnect();
}
