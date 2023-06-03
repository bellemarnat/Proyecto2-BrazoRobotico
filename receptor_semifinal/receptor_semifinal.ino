#include <SoftwareSerial.h>
#include <Servo.h>

Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;

SoftwareSerial mySerial(4, 5); // RX, TX
int val1, val2, val3, val4;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Servo1.attach(8);
  Servo2.attach(9);
  Servo3.attach(10);
  Servo4.attach(11);

}

void loop() {
  if (mySerial.available()) {
    String input = mySerial.readStringUntil('\n');

    int index = 0;
    int delimiterIndex;

    while ((delimiterIndex = input.indexOf(',')) != -1) {
      String value = input.substring(0, delimiterIndex);
      input = input.substring(delimiterIndex + 1);
      assignValue(index, value);
      index++;
    }
    assignValue(index, input);

    printValues();
  }
}

void assignValue(int index, String value) {
  // Convierte la cadena a un entero y asigna el valor a la variable correspondiente
  
  switch (index) {
    case 0:
      val1 = value.toInt();
      break;
    case 1:
      val2 = value.toInt();
      break;
    case 2:
      val3 = value.toInt();
      break;
    case 3:
      val4 = value.toInt();
      break;
  }
}

void printValues() {
  Serial.print("val1 = ");
  Serial.print(val1);
  Serial.print(", val2 = ");
  Serial.print(val2);
  Serial.print(", val3 = ");
  Serial.print(val3);
  Serial.print(", val4 = ");
  Serial.println(val4);
  Servo1.write(val1);
  Servo2.write(val2);
  Servo3.write(val3);
  Servo4.write(val4);
}