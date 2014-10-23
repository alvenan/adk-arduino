#include "variant.h"
#include <stdio.h>
#include <adk.h>
#include <stdlib.h>


// Accessory descriptor. It's how Arduino identifies itself to Android.
char applicationName[] = "DueAdkAnalogLed"; // the app on your phone
char accessoryName[] = "Arduino Due"; // your Arduino board
char companyName[] = "Arduino-er";

// Make up anything you want for these
char versionNumber[] = "0.1";
char serialNumber[] = "1";
char url[] = "https://sites.google.com/site/alisonengineer/AndroidADK_0.1.apk?attredirects=0&d=1";

USBHost Usb;
ADK adk(&Usb, companyName, applicationName, accessoryName, versionNumber, url, serialNumber);

// Pin 13 has an LED connected on most Arduino boards.
int led = 13;

void setup() {
  Serial.begin(9600);
  cpu_irq_enable();

  pinMode(led, OUTPUT);
  //Indicate start of program
  digitalWrite(led, LOW);
  delay(2000);
  digitalWrite(led, HIGH);
  for (int i = 0; i <= 2; i++) {
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
    delay(250);
  }
}

#define RCVSIZE 128

void loop() {

  uint8_t buf[RCVSIZE];
  uint32_t nbread = 0;

  Usb.Task();

  if (adk.isReady()) {

    adk.read(&nbread, RCVSIZE, buf);
    if (nbread > 0) {

      uint8_t asciiChar[2];

      //Convert received uint8_t to human readable ASCII
      uint8_t lowNibble = buf[0] & 0x0F;
      uint8_t highNibble = (buf[0] & 0xF0) >> 4;

      if (lowNibble <= 0x09) {
        asciiChar[1] = '0' + lowNibble;
      } else {
        asciiChar[1] = 'A' + lowNibble - 0x0A;
      }

      if (highNibble <= 0x09) {
        asciiChar[0] = '0' + highNibble;
      } else {
        asciiChar[0] = 'A' + highNibble - 0x0A;
      }

      adk.write(2, asciiChar);

      uint8_t ledBrightness = buf[0];
      analogWrite(led, ledBrightness);
    }

  }else{
      analogWrite(led, 0);
  }

}
