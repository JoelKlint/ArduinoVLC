/* 
 Parts required:
 photoresistor
 10 kilohm resistor
 lamp

 Created 10 April 2018
 by Joel Klint
*/

char MESSAGE[10] = "20186066\n";

// Value where phototransistor identifies 1 or 0
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

  int sensorValue;
  int sensorLow = 0;
  int sensorHigh = 1023;
  sensorThreshold = 511;

  // calibrate for the first four seconds after program starts
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
  // For benchmarking
  unsigned long start_time = millis();
  unsigned long measure_time = 0;
  unsigned long bits_transmitted = 0;
  while((measure_time = millis() - start_time) < 10000) {
  
    // Loop over all chars
    for(int i=0; i < sizeof(MESSAGE); i++) {
      char send_char = MESSAGE[i];
  
      char received_char;
  
      // Loop over the bits in char
      for(int j=7; j >= 0; j--) {
        
        // Send the bit
        if(bitRead(send_char, j) == 1) {
          turnOnLamp();
        } else {
          turnOffLamp();
        }
  
        // Receive the bit
        int sensorValue = analogRead(A0);
        if(sensorValue < sensorThreshold) {
          bitWrite(received_char, j, 0);
        } else {
          bitWrite(received_char, j, 1);
        }
        bits_transmitted++;
      }
      Serial.print(received_char);
    }

  }

  Serial.print("Bitrate: ");
  measure_time = measure_time/1000;
  Serial.print(bits_transmitted / measure_time);
  Serial.println(" bps");
  delay(2000);
  
}

