#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(4, 5); // RX, TX

#define JOY1_X 0 // Pin A0
#define JOY1_Y 1 // Pin A1
#define JOY2_X 2 // Pin A2
#define JOY2_Y 3 // Pin A3

LiquidCrystal_I2C lcd(0x27,16,2);


void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  Serial.begin(9600);
  mySerial.begin(9600); // Iniciar mySerial

  // Configurar el ADC
  ADMUX = (1 << REFS0); // use AVcc as the reference
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Habilitar el ADC configurando el preescalador a 128
  
 
}

void loop() {
  lcd.begin(16, 2);
  lcd.backlight();  
  lcd.setCursor(0,0);
  lcd.print("Bienvenido/a");
  
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

}

int readADC(int channel) {
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Seleccionar el canal del ADC
  ADCSRA |= (1 << ADSC); // Iniciar la conversión
  while (ADCSRA & (1 << ADSC)); // Esperar a que la conversión termine
  return ADC; // Leer el valor de la conversión
}
