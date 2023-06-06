// Importar las bibliotecas necesarias
#include <SoftwareSerial.h>  // para comunicación serial
#include <Servo.h>  // para controlar servos
#include <EEPROM.h>  // para leer/escribir en la memoria EEPROM

// Declarar los servos
Servo Servo1;
Servo Servo2;
Servo Servo3;
Servo Servo4;

// Definir los pines para los botones
const int buttonPin = 2; // Pin del botón para controlar el Servo4
const int guardarButtonPin = 3; // Botón para guardar las posiciones de los servos
const int restaurarButtonPin = 12; // Botón para restaurar las posiciones de los servos

const int servoPin = 11; // Pin al que está conectado el Servo4
bool servoMoved = false;  // Indica si el servo se ha movido
unsigned long startTime = 0;  // Guarda el momento en que el servo comienza a moverse
int servoAngle4 = 0; // Angulo del Servo4

SoftwareSerial mySerial(4, 5); // RX, TX

// Valores de los servos
int val1, val2, val3, val4;
int prev_val1, prev_val2, prev_val3, prev_val4;  // Valores previos de los servos

// Estructura para guardar las posiciones de los servos
struct ServoPositions {
  int servo1Pos;
  int servo2Pos;
  int servo3Pos;
  int servo4Pos;
};

ServoPositions savedPositions;  // Guardar las posiciones de los servos

// guardar las posiciones de los servos en la memoria EEPROM
void guardarPosiciones() {
  savedPositions.servo1Pos = Servo1.read();
  savedPositions.servo2Pos = Servo2.read();
  savedPositions.servo3Pos = Servo3.read();
  savedPositions.servo4Pos = Servo4.read();

  int address = 0;
  EEPROM.put(address, savedPositions);
}

// restaurar las posiciones de los servos desde la memoria EEPROM
void restaurarPosiciones() {
  int address = 0;
  EEPROM.get(address, savedPositions);

  Servo1.write(savedPositions.servo1Pos);
  Servo2.write(savedPositions.servo2Pos);
  Servo3.write(savedPositions.servo3Pos);
  Servo4.write(savedPositions.servo4Pos);
}

// ejecutar cuando se presiona el botón de mover el Servo4
void myInterruptFunction() {
  if (!servoMoved) {
    servoAngle4 = 60; // Mover a 60 grados
    Servo4.write(servoAngle4);
    servoMoved = true; // Indicar que el servo se ha movido
    startTime = millis();  // Registrar el momento en que el servo comenzó a moverse
  }
}

void setup() {
  // Iniciar la comunicación serial
  Serial.begin(9600);
  mySerial.begin(9600);

  // pines a los que están conectados los servos
  Servo1.attach(8);
  Servo2.attach(9);
  Servo3.attach(10);
  Servo4.attach(11);

  // Inicializar las posiciones de los servos
  prev_val1 = prev_val2 = prev_val3 = prev_val4 = 90;

  // Configurar los pines de los botones como entradas con resistencias de pull-up
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(guardarButtonPin, INPUT_PULLUP);
  pinMode(restaurarButtonPin, INPUT_PULLUP);

  // Adjuntar la interrupción al pin del botón
  attachInterrupt(digitalPinToInterrupt(buttonPin), myInterruptFunction, FALLING);
  Servo4.write(0); // Inicializar la posición del Servo4

  // Configurar el timer para la interrupción
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
  // para verificar funcionamiento correcto:
  // Si hay datos disponibles en el puerto serial, leerlos
  if (mySerial.available()) {
    String input = mySerial.readStringUntil('\n');
    Serial.print("Entrada recibida: ");
    Serial.println(input); // Imprimir la entrada recibida

    int index = 0;
    int delimiterIndex;

    // Dividir la entrada en valores separados por comas
    while ((delimiterIndex = input.indexOf(',')) != -1) {
      String value = input.substring(0, delimiterIndex);
      input = input.substring(delimiterIndex + 1);
      assignValue(index, value);  // Asignar cada valor a su respectivo servo
      index++;
    }
    assignValue(index, input);  // Asignar el último valor
    printValues();  // Imprimir los valores actuales
    smoothServoMove(); // Mover los servos de manera suave
  }

  // Si se presiona el botón de guardar, guardar las posiciones de los servos
  if (digitalRead(guardarButtonPin) == LOW) {
    guardarPosiciones();
    delay(500); // tiempo para evitar rebotes
  }

  // Si se presiona el botón de restaurar, restaurar las posiciones de los servos
  if (digitalRead(restaurarButtonPin) == LOW) {
    restaurarPosiciones();
    delay(500); // tiempo para evitar rebotes
  }
}

// Función para asignar un valor a su respectivo servo
void assignValue(int index, String value) {
  int val = value.toInt();
  val = constrain(val, 0, 1023);  // Restringe valores entre 0 y 1023

  switch (index) {
    case 0:
      val1 = map(val, 0, 1023, 0, 180);  // Mapear a grados para el servo
      break;
    case 1:
      val2 = map(val, 0, 1023, 0, 180);  // Mapear a grados para el servo
      break;
    case 2:
      val3 = map(val, 0, 1023, 0, 180);  // Mapear a grados para el servo
      break;
    case 3:
      val4 = map(val, 0, 1023, 0, 180);  // Mapear a grados para el servo
      if (!servoMoved) {
        servoAngle4 = map(val4, 0, 1023, 0, 180);
      }
      Servo4.write(servoAngle4);
      break;
  }
}

// imprimir los valores actuales
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

// mover los servos de manera suave
void smoothServoMove() {
  int steps = 100; // Definir el número de pasos
  int delayTime = 10; // Tiempo de retraso entre cada paso

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

// Interupción del timer para el Servo4
ISR(TIMER2_COMPA_vect) {
  // Verificar si ha pasado el tiempo necesario para volver a la posición inicial
  if (servoMoved && (millis() - startTime >= 10000)) {
    Servo4.write(0);  // Volver a la posición inicial (0 grados)
    servoMoved = false;  // Reiniciar estado del servo
  }
}
