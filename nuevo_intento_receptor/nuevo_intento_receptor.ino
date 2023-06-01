#include <Servo.h>
#include <SoftwareSerial.h>

Servo servo1; // Base
Servo servo2; // Brazo
Servo servo3; // Antebrazo
Servo servo4; // Pinza

SoftwareSerial mySerial(4, 5); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  servo1.attach(8);
  servo2.attach(9);
  servo3.attach(10);
  servo4.attach(11);
}

void loop() {
  if (mySerial.available() >= 8) {
    int analog1_value = mySerial.read() << 8; // Recibe los 8 bits superiores
    analog1_value |= mySerial.read(); // Recibe los 8 bits inferiores

    int analog2_value = mySerial.read() << 8;
    analog2_value |= mySerial.read();

    int analog3_value = mySerial.read() << 8;
    analog3_value |= mySerial.read();

    int analog4_value = mySerial.read() << 8;
    analog4_value |= mySerial.read();

    int posBase = map(analog1_value, 0, 1023, 0, 180);
    int posBrazo = map(analog2_value, 0, 1023, 0, 180);
    int posAntebrazo = map(analog3_value, 0, 1023, 0, 180);
    int posPinza = map(analog4_value, 0, 1023, 0, 180);

    servo1.write(posBase);
    servo2.write(posBrazo);
    servo3.write(posAntebrazo);
    servo4.write(posPinza);
  }
}
