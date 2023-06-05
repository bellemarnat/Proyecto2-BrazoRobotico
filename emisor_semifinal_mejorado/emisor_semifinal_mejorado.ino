#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(4, 5); // RX, TX
const int buttonPin = 2; //Pin para el botón,
//en Arduino Uno los pines que funcionan para realizar interrupciones son los pines 2 y 3
Servo Servo4;
const int servoPin = 11; // Servo de la pinza
bool servoMoved = false;
unsigned long startTime = 0; // Timer comienza en 0
int servoAngle4 = 0;


#define JOY1_X 0 // Pin A0
#define JOY1_Y 1 // Pin A1
#define JOY2_X 2 // Pin A2
#define JOY2_Y 3 // Pin A3

void myInterruptFunction() {
  // Código a ejecutar cuando ocurra la interrupción
  // Puede ser cualquier instrucción o llamada a función
  if (!servoMoved) {
    servoAngle4 = 45;
    Servo4.write(servoAngle4);
    servoMoved = true;
    startTime = millis();  // Guardar el tiempo actual
  }
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); // Iniciar mySerial

  // Configurar el ADC
  ADMUX = (1 << REFS0); // use AVcc as the reference
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar el ADC configurando el preescalador a 128
  
  pinMode(buttonPin, INPUT_PULLUP);
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
  int joy1_x = readADC(JOY1_X);
  int joy1_y = readADC(JOY1_Y);
  int joy2_x = readADC(JOY2_X);
  int joy2_y = readADC(JOY2_Y);

  Serial.print("Joy1_X: ");
  Serial.print(joy1_x);
  Serial.print(" Joy1_Y: ");
  Serial.print(joy1_y);
  Serial.print(" Joy2_X: ");
  Serial.print(joy2_x);
  Serial.print(" Joy2_Y: ");
  Serial.println(joy2_y);

  // Enviar los valores a través de mySerial
  mySerial.print(joy1_x);
  mySerial.print(',');
  mySerial.print(joy1_y);
  mySerial.print(',');
  mySerial.print(joy2_x);
  mySerial.print(',');
  mySerial.println(joy2_y); // Asegúrate de que los valores estén separados por una coma y que el último valor termine con un salto de línea

  if (!servoMoved) {
  servoAngle4 = map(potValue4, 0, 1023, 0, 180);
  }
  Servo4.write(servoAngle4);
}

int readADC(int channel) {
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Seleccionar el canal del ADC
  ADCSRA |= (1 << ADSC); // Iniciar la conversión
  while (ADCSRA & (1 << ADSC)); // Esperar a que la conversión termine
  return ADC; // Leer el valor de la conversión
}
ISR(TIMER2_COMPA_vect) {
  // Verificar si ha pasado el tiempo necesario para volver a la posición inicial
  if (servoMoved && (millis() - startTime >= 10000)) {
    Servo4.write(0);  // Volver a la posición inicial (0 grados)
    servoMoved = false;  // Reiniciar el estado del servo
  }
}
