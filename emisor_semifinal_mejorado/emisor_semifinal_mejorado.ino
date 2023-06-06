// Importar bibliotecas necesarias
#include <SoftwareSerial.h>  // para comunicación serial
#include <LiquidCrystal_I2C.h>  // para manejar la pantalla LCD con módulo I2C

SoftwareSerial mySerial(4, 5); // RX, TX

// Definir los pines analógicos donde se conectan los joysticks
#define JOY1_X 0 // Joystick 1 eje X en pin A0
#define JOY1_Y 1 // Joystick 1 eje Y en pin A1
#define JOY2_X 2 // Joystick 2 eje X en pin A2
#define JOY2_Y 3 // Joystick 2 eje Y en pin A3

// Inicializar el LCD con la dirección I2C (0x27) y tamaño de 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Configurar e inicializar LCD
  lcd.begin(16, 2);
  lcd.backlight();

  // Configurar comunicaciones serie
  Serial.begin(9600);
  mySerial.begin(9600); 

  // Configurar el ADC para usar AVcc como referencia y un preescalador de 128
  ADMUX = (1 << REFS0);
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 

}

void loop() {
  // Reiniciar LCD y mostrar mensaje de bienvenida
  lcd.begin(16, 2);
  lcd.backlight();  
  lcd.setCursor(0,0);
  lcd.print("Bienvenido/a");
  
  // Leer los valores de los joysticks y guardarlos
  int joy1_x = readADC(JOY1_X);  
  int joy1_y = readADC(JOY1_Y);
  int joy2_x = readADC(JOY2_X);
  int joy2_y = readADC(JOY2_Y);

  // Imprimir los valores de los joysticks en la consola serie
  Serial.print("Joy1_X: ");
  Serial.print(joy1_x);
  Serial.print(" Joy1_Y: ");
  Serial.print(joy1_y);
  Serial.print(" Joy2_X: ");
  Serial.print(joy2_x);
  Serial.print(" Joy2_Y: ");
  Serial.println(joy2_y);

  // Enviar los valores de los joysticks a través de mySerial
  mySerial.print(joy1_x);
  mySerial.print(',');
  mySerial.print(joy1_y);
  mySerial.print(',');
  mySerial.print(joy2_x);
  mySerial.print(',');
  mySerial.print(joy2_y);

}

// Función para leer el valor de una entrada analógica
int readADC(int channel) { // Seleccionar el canal del ADC
  ADMUX = (ADMUX & 0xF0) | (channel & 0x0F); // Iniciar la conversión
  ADCSRA |= (1 << ADSC); // Esperar a que la conversión termine
  while (ADCSRA & (1 << ADSC)); // Leer y devolver el valor de la conversión
  return ADC; 
}
