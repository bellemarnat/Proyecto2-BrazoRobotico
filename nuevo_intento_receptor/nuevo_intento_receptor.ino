#include <Servo.h>
#include <SoftwareSerial.h>

Servo servo1; // Base
Servo servo2; // Brazo
Servo servo3; // Antebrazo
Servo servo4; // Pinza

SoftwareSerial mySerial(4, 5); // RX, TX

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);



  servo1.attach(8);
  servo2.attach(9);
  servo3.attach(10);
  servo4.attach(11);
}

int readADC(int pin) {
  /*ADCSRA |= (1 << ADEN);        // Habilitar ADC
  ADMUX = (0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (pin & 0x07); // Configurar referencia y pin
  ADCSRA |= (1 << ADSC);        // Iniciar conversión
  while (ADCSRA & (1 << ADSC)); // Esperar a que la conversión termine
  return ADC;                   // Retornar el valor leído*/
ADMUX = (1<<REFS0);
ADCSRA = (1<< ADEN)|(1<< ADPS0) |(1<< ADPS1)|(1<< ADPS2);

ADMUX &= 0xF0;
ADMUX |= 0b00000; 
ADCSRA |= (1 << ADSC);
while (ADCSRA & (1 << ADSC));
return ADC;   
int pot_value0 = ADC;

}

void loop() {
  
  if (mySerial.available() >= 8) {
    char receivedChar = mySerial.read();
    if (receivedChar == 'B') {
      int pot1_value = mySerial.read();
      int pot2_value = mySerial.read();
      int pot3_value = mySerial.read();
      int pot4_value = mySerial.read();

      int analog1_value = readADC(A0); // Potenciometro de 10k
      int analog2_value = readADC(A1); // Potenciometro de 5k
      int analog3_value = readADC(A2); // Potenciometro de 10k
      int analog4_value = readADC(A3); // Potenciometro de 5k

      int posBase = map(analog1_value, 0, 1023, 0, 360);
      int posBrazo = map(analog2_value, 0, 1023, 0, 180);
      int posAntebrazo = map(analog3_value, 0, 1023, 0, 180);
      int posPinza = map(analog4_value, 0, 1023, 0, 180);

      servo1.write(posBase);
      servo2.write(posBrazo);
      servo3.write(posAntebrazo);
      servo4.write(posPinza);
    }
  }
}
