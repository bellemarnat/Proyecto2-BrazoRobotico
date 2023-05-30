#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// Pines para el LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pines para la comunicación serial por software
SoftwareSerial mySerial(10, 9); // RX, TX

Servo servo1; // Base
Servo servo2; // Brazo
Servo servo3; // Antebrazo
Servo servo4; // Pinza

// Pines para los servos
int pinservo1 = 8;
int pinservo2 = 7;
int pinservo3 = 6;
int pinservo4 = 5;

// Pines para los potenciómetros
int potPin1 = A0;
int potPin2 = A1;
int potPin3 = A2;
int potPin4 = A3;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Iniciando...");
  mySerial.begin(9600);
  servo1.attach(pinservo1, 610, 2550);
  servo2.attach(pinservo2, 670, 2540);
  servo3.attach(pinservo3, 660, 2600);
  servo4.attach(pinservo4, 660, 2600);
  pinMode(pinservo1, OUTPUT); 
  pinMode(pinservo2, OUTPUT);
  pinMode(pinservo3, OUTPUT);
  pinMode(pinservo4, OUTPUT);
  iniciar();
  delay(2000);
}

void loop() {
  // Leer los potenciómetros y mover los servos
  servo1.write(map(analogRead(potPin1), 0, 1023, 0, 180));
  servo2.write(map(analogRead(potPin2), 0, 1023, 0, 180));
  servo3.write(map(analogRead(potPin3), 0, 1023, 0, 180));
  servo4.write(map(analogRead(potPin4), 0, 1023, 0, 180));

  // Leer la EEPROM y mostrar los datos en el LCD
  lcd.setCursor(0, 0);
  lcd.print("EEPROM: ");
  lcd.print(EEPROM.read(0));

  // Leer la comunicación serial y actuar en consecuencia
  if (mySerial.available()) {
    char c = mySerial.read();
    // Tu código para manejar la comunicación serial va aquí
  }

  // Tu código para manejar las interrupciones va aquí
}

void iniciar() {
  pinza(0, 0);
  delay(200);
  antebrazo(160, 160);
  delay(200);
  brazo(120, 120);
  delay(200);
  base(80, 80);
  delay(200);
  delay(5000);
}

void base(int desde, int hasta) {
  // ... El resto de tu código ...
}

void brazo(int desde, int hasta)```cpp
  // ... El resto de tu código ...
}

void antebrazo(int desde, int hasta) {
  // ... El resto de tu código ...
}

void pinza(int desde, int hasta) {
  // ... El resto de tu código ...
}

// Manejar las interrupciones de cambio de pin
void handlePinChange() {
  // Tu código para manejar el cambio de pin va aquí
}

// Manejar las interrupciones del temporizador
void handleTimerInterrupt() {
  // Tu código para manejar la interrupción del temporizador va aquí
}
