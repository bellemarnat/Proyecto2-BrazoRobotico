#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 bps
  mySerial.begin(9600);
}

int readADC(int pin) {
  ADMUX = (ADMUX & 0xF8) | (pin & 0x07); // Seleccionar el canal de entrada
  ADCSRA |= (1 << ADSC); // Iniciar la conversión
  while (ADCSRA & (1 << ADSC)); // Esperar a que la conversión termine
  return ADC; // Devolver el valor leído
}


void loop() {
  int pot1_value = readADC(0); // Potenciometro de 10k
  int pot2_value = readADC(1); // Potenciometro de 5k
  int pot3_value = readADC(2); // Potenciometro de 10k
  int pot4_value = readADC(3); // Potenciometro de 5k

  mySerial.write(pot1_value >> 8); // Envia los 8 bits superiores
  mySerial.write(pot1_value & 0xFF); // Envia los 8 bits inferiores

  mySerial.write(pot2_value >> 8);
  mySerial.write(pot2_value & 0xFF);

  mySerial.write(pot3_value >> 8);
  mySerial.write(pot3_value & 0xFF);

  mySerial.write(pot4_value >> 8);
  mySerial.write(pot4_value & 0xFF);

  delay(100);
}
