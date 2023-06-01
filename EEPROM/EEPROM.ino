#include <EEPROM.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

const int potentiometerPin1 = A0;
const int potentiometerPin2 = A1;
const int potentiometerPin3 = A2;
const int potentiometerPin4 = A3;

struct ServoPosition {
  int servo1Angle;
  int servo2Angle;
  int servo3Angle;
  int servo4Angle;
};

const int maxPositions = 10;  // Número máximo de posiciones a grabar
ServoPosition positions[maxPositions];
int currentPosition = 0;  // Índice de la posición actual

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C de la pantalla LCD

void setup() {
  lcd.begin(16, 2);  // Inicializar la pantalla LCD
  lcd.print("Servo Recorder");  // Mostrar título en la primera línea
  lcd.setCursor(0, 1);
  lcd.print("Ready to record");  // Mostrar mensaje de estado en la segunda línea

  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);
  servo4.attach(12);

  Serial.begin(9600);
}

void loop() {
  // Leer los valores de los potenciómetros
  int servo1Angle = analogRead(potentiometerPin1);
  int servo2Angle = analogRead(potentiometerPin2);
  int servo3Angle = analogRead(potentiometerPin3);
  int servo4Angle = analogRead(potentiometerPin4);

  // Mapear los valores de los potenciómetros al rango de ángulos del servo (0-180)
  servo1Angle = map(servo1Angle, 0, 1023, 0, 180);
  servo2Angle = map(servo2Angle, 0, 1023, 0, 180);
  servo3Angle = map(servo3Angle, 0, 1023, 0, 180);
  servo4Angle = map(servo4Angle, 0, 1023, 0, 180);

  // Mover los servomotores a los ángulos correspondientes
  servo1.write(servo1Angle);
  servo2.write(servo2Angle);
  servo3.write(servo3Angle);
  servo4.write(servo4Angle);

  // Verificar si se debe grabar una posición
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'g') {
      grabarPosicion();
    }
  }
}

void grabarPosicion() {
  // Obtener los ángulos de los servomotores
  int servo1Angle = analogRead(potentiometerPin1);
  int servo2Angle = analogRead(potentiometerPin2);
  int servo3Angle = analogRead(potentiometerPin3);
  int servo4Angle = analogRead(potentiometerPin4);

  // Guardar la posición actual en el arreglo
  positions[currentPosition].servo1Angle = servo1Angle;
  positions[currentPosition].servo2Angle = servo2Angle;
  positions[currentPosition].servo3Angle = servo3Angle;
  positions[currentPosition].servo4Angle = servo4Angle;

  // Incrementar el índice de la posición actual
  currentPosition++;

  // Verificar si se ha alcanzado el número máximo de posiciones
  if (currentPosition >= maxPositions) {
    lcd.clear();
    lcd.print("Max positions");
    lcd.setCursor(0, 1);
    lcd.print("reached");
    delay(2000);  // Mostrar mensaje por 2 segundos
    lcd.clear();
    lcd.print("Servo Recorder");
    lcd.setCursor(0, 1);
    lcd.print("Ready to record");
    currentPosition = maxPositions - 1;
  } else {
    lcd.clear();
    lcd.print("Position ");
    lcd.print(currentPosition + 1);
    lcd.setCursor(0, 1);
    lcd.print("Recorded");
    delay(2000);  // Mostrar mensaje por 2 segundos
    lcd.clear();
    lcd.print("Servo Recorder");
    lcd.setCursor(0, 1);
    lcd.print("Ready to record");
  }
}

