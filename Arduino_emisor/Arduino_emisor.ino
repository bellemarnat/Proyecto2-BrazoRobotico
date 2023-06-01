#include <LiquidCrystal.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <EEPROM.h>

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

//variables para guardar posiciones en servomotores
struct ServoPosition {
  int servo1Angle;
  int servo2Angle;
  int servo3Angle;
  int servo4Angle;
};

const int maxPositions = 10;  // Número máximo de posiciones a grabar
ServoPosition positions[maxPositions];
int currentPosition = 0;  // Índice de la posición actual


// declaración de variable para servo motor
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

//botón para interrupción
const int buttonPin = 13;

//timer
unsigned long timerStartTime = 0;
const unsigned long timerDuration = 5000;  // 5 segundos

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

// Configurar pin de botón como entrada con resistencia de pull-down externa
  DDRB &= ~(1 << DDB5);   // Configurar pin 13 como entrada
  PORTB |= (1 << PORTB5); // Activar resistencia de pull-down en pin 13
  
  // Configurar y mover el servo motor a la posición inicial
  servo4.attach(motorPin);
  servo4.write(90);

//  configuración attach para guardar posiciones en la eeprom
  servo1.attach(8);
  servo2.attach(9);
  servo3.attach(10);
  //servo4.attach(12); no se si este attach interfiere con la linea 64 empleada para el timer
  Serial.begin(9600);
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
//Interrupción 
   if (buttonPressed) {
    // Mover el servo motor 90 grados
    servo4.write(0);

    // Iniciar el temporizador
    timerStartTime = millis();

    // Restablecer el estado del botón
    buttonPressed = false;
    
    // Envía el comando al receptor a través de la comunicación serial
    Serial.write("MoverServo");
}

if (millis() - timerStartTime >= timerDuration) {
    // Mover el servo motor a la posición inicial
    servo4.write(90);
  }
