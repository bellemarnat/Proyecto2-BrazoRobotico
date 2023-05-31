#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define JOY1_X A0
#define JOY1_Y A1
#define JOY2_X A2
#define JOY2_Y A3

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Pines para el LCD (RS, E, D4, D5, D6, D7)
SoftwareSerial mySerial(10, 11); // RX, TX

uint8_t pinservo1 = 8;
uint8_t pinservo2 = 9;
uint8_t pinservo3 = 10;
uint8_t pinservo4 = 11;

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 bps
  mySerial.begin(9600);
  
  lcd.begin(16, 2);
  lcd.print("Iniciando...");
  delay(1000);

  // Configura los pines del joystick como entrada
  DDRC &= ~(1 << DDC0);
  DDRC &= ~(1 << DDC1);
  DDRC &= ~(1 << DDC2);
  DDRC &= ~(1 << DDC3);

  // Configura el número de columnas y filas del LCD
  lcd.begin(16, 2);
}

// Reads an ADC channel
int readADC(int channel) {
  ADMUX &= 0xF0;           // Borrar el canal antiguo leido 
  ADMUX |= channel;        // Defines the new ADC channel to be read
  ADCSRA |= (1 << ADSC);   // Starts a new conversion
  while (ADCSRA & (1 << ADSC));  // Wait until the conversion is done
  return ADC;             // Returns the ADC value of the chosen channel
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("EEPROM: ");

  int joy1_x = readADC(JOY1_X);
  int joy1_y = readADC(JOY1_Y);
  int joy2_x = readADC(JOY2_X);
  int joy2_y = readADC(JOY2_Y);

  // Mapea los valores del joystick a las posiciones del servo
  int posBase = map(joy1_x, 0, 1023, 0, 360);
  int posBrazo = map(joy1_y, 0, 1023, 0, 180);
  int posAntebrazo = map(joy2_x, 0, 1023, 0, 180);
  int posPinza = map(joy2_y, 0, 1023, 0, 180);

  // Envía los valores de posición a través de la comunicación serial
  mySerial.print("B");
  mySerial.print(posBase);
  mySerial.print("A");
  mySerial.print(posBrazo);
  mySerial.print("C");
  mySerial.print(posAntebrazo);
  mySerial.print("P");
  mySerial.println(posPinza);

  lcd.setCursor(0, 1);
  lcd.print("Base: ");
  lcd.print(posBase);
  lcd.print(" Brazo: ");
  lcd.print(posBrazo);

  delay(100);
  lcd.clear();
}
