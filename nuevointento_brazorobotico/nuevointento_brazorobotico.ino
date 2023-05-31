// Arduino emisor

#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define JOY1_X A0
#define JOY1_Y A1
#define JOY2_X A2
#define JOY2_Y A3

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Pines para el LCD (RS, E, D4, D5, D6, D7)

Servo servo1; // Base
Servo servo2; // Brazo
Servo servo3; // Antebrazo
Servo servo4; // Pinza

uint8_t pinservo1 = 8;
uint8_t pinservo2 = 9;
uint8_t pinservo3 = 10;
uint8_t pinservo4 = 11;

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 bps

  lcd.begin(16, 2);
  lcd.print("Iniciando...");
  delay(1000);

  servo1.attach(pinservo1, 610, 2550); // revisar esto, no deberia tener tantos parametros
  servo2.attach(pinservo2, 670, 2540);
  servo3.attach(pinservo3, 660, 2600);
  servo4.attach(pinservo4, 660, 2600);

  // Configura los pines del joystick como entrada
  DDRC &= ~(1 << DDC0);
  DDRC &= ~(1 << DDC1);
  DDRC &= ~(1 << DDC2);
  DDRC &= ~(1 << DDC3);

  // Configura los pines del servo como salida
  DDRB |= (1 << DDB0);
  DDRB |= (1 << DDB1);
  DDRB |= (1 << DDB2);
  DDRB |= (1 << DDB3);

  // Configura el número de columnas y filas del LCD
  lcd.begin(16, 2);
}

// Reads an ADC channel
int readADC(int channel) {
  ADMUX &= 0xF0;           // Clear the older channel that was read
  ADMUX |= channel;        // Defines the new ADC channel to be read
  ADCSRA |= (1 << ADSC);   // Starts a new conversion
  while (ADCSRA & (1 << ADSC));  // Wait until the conversion is done
  return ADC;             // Returns the ADC value of the chosen channel
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("EEPROM: ");
  lcd.print(EEPROM.read(0));

  int joy1_x = readADC(JOY1_X);
  int joy1_y = readADC(JOY1_Y);
  int joy2_x = readADC(JOY2_X);
  int joy2_y = readADC(JOY2_Y);

  // Mapea los valores del joystick a las posiciones del servo
  int posBase = map(joy1_x, 0, 1023, 0, 360);
  int posBrazo = map(joy1_y, 0, 1023, 0, 180);
  int posAntebrazo = map(joy2_x, 0, 1023, 0, 180);
  int posPinza = map(joy2_y, 0, 1023, 0, 180);

  servo1.write(posBase);
  servo2.write(posBrazo);
  servo3.write(posAntebrazo);
  servo4.write(posPinza);

  // Envía los valores de posición a través de la comunicación serial
  Serial.print("B");
  Serial.print(posBase);
  Serial.print("A");
  Serial.print(posBrazo);
  Serial.print("C");
  Serial.print(posAntebrazo);
  Serial.print("P");
  Serial.println(posPinza);

  lcd.setCursor(0, 1);
  lcd.print("Base: ");
  lcd.print(posBase);
  lcd.print(" Brazo: ");
  lcd.print(posBrazo);

  delay(100);
  lcd.clear();
}
