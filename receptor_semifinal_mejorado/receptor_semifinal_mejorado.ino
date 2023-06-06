#include <SoftwareSerial.h>
#include <Servo.h>
#include <EEPROM.h>

Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;

const int buttonPin = 2; //Pin para el botón que moviliza pinza con interrupción
const int guardarButtonPin = 3;
const int restaurarButtonPin = 12;

const int servoPin = 11; // Servo de la pinza
bool servoMoved = false;
unsigned long startTime = 0; // Timer comienza en 0
int servoAngle4 = 0;

SoftwareSerial mySerial(0, 1); // RX, TX

int val1, val2, val3, val4;
int prev_val1, prev_val2, prev_val3, prev_val4;

struct ServoPositions {
  int servo1Pos;
  int servo2Pos;
  int servo3Pos;
  int servo4Pos;
};

ServoPositions savedPositions;
void guardarPosiciones() {
  savedPositions.servo1Pos = Servo1.read();
  savedPositions.servo2Pos = Servo2.read();
  savedPositions.servo3Pos = Servo3.read();
  savedPositions.servo4Pos = Servo4.read();

  int address = 0;
  EEPROM.put(address, savedPositions);}

  void restaurarPosiciones() {
  int address = 0;
  EEPROM.get(address, savedPositions);

  Servo1.write(savedPositions.servo1Pos);
  Servo2.write(savedPositions.servo2Pos);
  Servo3.write(savedPositions.servo3Pos);
  Servo4.write(savedPositions.servo4Pos);
}
  
void myInterruptFunction() {
  // Código a ejecutar cuando ocurra la interrupción
  // Puede ser cualquier instrucción o llamada a función
  if (!servoMoved) {
    servoAngle4 = 60;
    Servo4.write(servoAngle4);
    servoMoved = true;
    startTime = millis();  // Guardar el tiempo actual
  }
}


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  Servo1.attach(8);
  Servo2.attach(9);
  Servo3.attach(10);
  Servo4.attach(11);
  
  
  prev_val1 = prev_val2 = prev_val3 = prev_val4 = 90; // Servo initial position

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(guardarButtonPin, INPUT_PULLUP);
  pinMode(restaurarButtonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), myInterruptFunction, FALLING);
  Servo4.write(0); 

  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 249;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  TIMSK2 |= (1 << OCIE2A);
  sei();
  
  
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

  if (digitalRead(guardarButtonPin) == LOW) {
    guardarPosiciones();
    delay(500);
  }

  if (digitalRead(restaurarButtonPin) == LOW) {
    restaurarPosiciones();
    delay(500);
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
      if (!servoMoved) {
        servoAngle4 = map(val4, 0, 1023, 0, 180);
      }
      Servo4.write(servoAngle4);
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

ISR(TIMER2_COMPA_vect) {
  // Verificar si ha pasado el tiempo necesario para volver a la posición inicial
  if (servoMoved && (millis() - startTime >= 10000)) {
    Servo4.write(0);  // Volver a la posición inicial (0 grados)
    servoMoved = false;  // Reiniciar el estado del servo
  }
}

