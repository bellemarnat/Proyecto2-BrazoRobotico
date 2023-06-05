#include <SoftwareSerial.h>
#include <Servo.h>

Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;

bool servoMoved = false;
unsigned long startTime = 0;
int servoAngle4 = 0;

SoftwareSerial mySerial(4, 5); // RX, TX

int val1, val2, val3, val4;
int prev_val1, prev_val2, prev_val3, prev_val4;

}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Servo1.attach(8);
  Servo2.attach(9);
  Servo3.attach(10);
  Servo4.attach(11);
  
  
  prev_val1 = prev_val2 = prev_val3 = prev_val4 = 90; // Servo initial position
  
  
}

void loop() {
  if (mySerial.available()) {
    String input = mySerial.readStringUntil('\n');
    Serial.print("Entrada recibida: ");
    Serial.println(input); // Imprimir la entrada directamente

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

    smoothServoMove(); // Move servos in a smooth way
  }
}

void assignValue(int index, String value) {
  int val = value.toInt();
  val = constrain(val, 0, 1023);  // Restringe los valores entre 0 y 1023

  switch (index) {
    case 0:
      val1 = map(val, 0, 1023, 0, 180);  // Map to 0 - 180 degrees for Servo
      break;
    case 1:
      val2 = map(val, 0, 1023, 0, 180);  // Map to 0 - 180 degrees for Servo
      break;
    case 2:
      val3 = map(val, 0, 1023, 0, 180);  // Map to 0 - 180 degrees for Servo
      break;
    case 3:
      val4 = map(val, 0, 1023, 0, 180);  // Map to 0 - 180 degrees for Servo
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
}

void smoothServoMove() {
  int steps = 100; // Define the number of steps
  int delayTime = 10; // Delay between each step
  
  for (int i = 0; i <= steps; i++) {
    Servo1.write(prev_val1 + ((val1 - prev_val1) * i / steps));
    Servo2.write(prev_val2 + ((val2 - prev_val2) * i / steps));
    Servo3.write(prev_val3 + ((val3 - prev_val3) * i / steps));
    Servo4.write(prev_val4 + ((val4 - prev_val4) * i / steps));
    delay(delayTime);
  }
  
  prev_val1 = val1;
  prev_val2 = val2;
  prev_val3 = val3;
  prev_val4 = val4;
}

