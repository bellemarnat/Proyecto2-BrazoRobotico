#include <Servo.h>
#include <SoftwareSerial.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

SoftwareSerial mySerial(2,3); // RX, TX

void setup() {
  mySerial.begin(9600);
  
  servo1.attach(8);
  servo2.attach(9);
  servo3.attach(10);
  servo4.attach(11);
}

void loop() {
  if (mySerial.available()) {
    char receivedChar = mySerial.read();
    int pos = mySerial.parseInt();

    switch (receivedChar) {
      case 'B':
        servo1.write(pos);
        break;
      case 'A':
        servo2.write(pos);
        break;
      case 'C':
        servo3.write(pos);
        break;
      case 'P':
        servo4.write(pos);
        break;
    }
  }
}