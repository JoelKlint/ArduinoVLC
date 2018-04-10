/*
  Arduino Starter Kit example
 Project 6  - Light Theremin

 This sketch is written to accompany Project 6 in the
 Arduino Starter Kit

 Parts required:
 photoresistor
 10 kilohm resistor
 piezo

 Created 13 September 2012
 by Scott Fitzgerald

 http://www.arduino.cc/starterKit

 This example code is part of the public domain
*/

char MESSAGE[10] = "20186066\n";

// variable to hold sensor value
int sensorValue;
// variable to calibrate low value
int sensorLow = 1023;
// variable to calibrate high value
int sensorHigh = 0;
// variable which sais if phototransistor recieves one or zero
int sensorThreshold = 511;
// LED pin
const int ledPin = 13;

void turnOnLamp() {
  digitalWrite(ledPin, HIGH);
}
void turnOffLamp() {
  digitalWrite(ledPin, LOW);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup()");
  
  // Make the LED pin an output
  pinMode(ledPin, OUTPUT);

  turnOnLamp();

  // calibrate for the first five seconds after program runs
  while (millis() < 2000) {
    // record the maximum sensor value
    sensorValue = analogRead(A0);
    if (sensorValue > sensorHigh) {
      sensorHigh = sensorValue;
    }
    // record the minimum sensor value
    if (sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
  }

  turnOffLamp();
  
  while (millis() < 4000) {
    sensorValue = analogRead(A0);
    if (sensorValue > sensorHigh) {
      sensorHigh = sensorValue;
    }
    // record the minimum sensor value
    if (sensorValue < sensorLow) {
      sensorLow = sensorValue;
    }
  }

  sensorThreshold = (sensorLow + sensorHigh) / 2;
  
  // Make the LED blink, signaling end of calibration
  for(int i=0; i < 20; i++) {
    turnOnLamp();
    delay(50);
    turnOffLamp();
    delay(50);
  }
  delay(1000);
  Serial.print("Sensor low: ");
  Serial.println(sensorLow);
  Serial.print("Sensor high: ");
  Serial.println(sensorHigh);
  Serial.print("Sensor threshold: ");
  Serial.println(sensorThreshold);
  Serial.println("setup() finished");
}

void loop() {
  // Loop over all chars
  for(int i=0; i < sizeof(MESSAGE); i++) {
    char send_char = MESSAGE[i];

    char received_char;

    // Loop over the bits in char
    for(int j=7; j >= 0; j--) {
      
      // Send the bit
      if(bitRead(send_char, j) == 1) {
        turnOnLamp();
        //Serial.print("1");
      } else {
        turnOffLamp();
        //Serial.print("0");
      }

      // Receive the bit
      sensorValue = analogRead(A0);
      if(sensorValue < sensorThreshold) {
        bitWrite(received_char, j, 0);
        //Serial.print("0");
      } else {
        bitWrite(received_char, j, 1);
        //Serial.print("1");
      }
      //Serial.println(sensorValue);
      
      //delay(10);
    }
    Serial.print(received_char);
  }
}

