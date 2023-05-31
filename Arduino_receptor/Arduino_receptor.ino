#include <Servo.h>
#include <SoftwareSerial.h>

Servo servo1; // Base
Servo servo2; // Brazo
Servo servo3; // Antebrazo
Servo servo4; // Pinza

SoftwareSerial mySerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  servo1.attach(8, 610, 2550);
  servo2.attach(9, 670, 2540);
  servo3.attach(10, 660, 2600);
  servo4.attach(11, 660, 2600);
}

void loop() {
  if (mySerial.available()) {
    char receivedChar;
    int posBase;
    int posBrazo;
    int posAntebrazo;
    int posPinza;
    
    if (mySerial.read() == 'B') {
      posBase = mySerial.parseInt();
      if (mySerial.read() == 'A') {
        posBrazo = mySerial.parseInt();
        if (mySerial.read() == 'C') {
          posAntebrazo = mySerial.parseInt();
          if (mySerial.read() == 'P') {
            posPinza = mySerial.parseInt();
            
            servo1.write(posBase);
            servo2.write(posBrazo);
            servo3.write(posAntebrazo);
            servo4.write(posPinza);
          }
        }
      }
    }
  }
}
