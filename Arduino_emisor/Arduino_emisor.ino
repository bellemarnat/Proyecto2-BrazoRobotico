#include <SoftwareSerial.h>

#define JOY1_X 0
#define JOY1_Y 1
#define JOY2_X 2
#define JOY2_Y 3

SoftwareSerial mySerial(0, 1); // RX, TX

void setup() {
  mySerial.begin(9600);
  
  DDRC &= ~(1 << DDC0);
  DDRC &= ~(1 << DDC1);
  DDRC &= ~(1 << DDC2);
  DDRC &= ~(1 << DDC3);
}

int readADC(int channel) {
  ADMUX &= 0xF0;
  ADMUX |= channel;
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}

void loop() {
  int joy1_x = readADC(JOY1_X);
  int joy1_y = readADC(JOY1_Y);
  int joy2_x = readADC(JOY2_X);
  int joy2_y = readADC(JOY2_Y);

  int posBase = map(joy1_x, 0, 1023, 0, 180);
  int posBrazo = map(joy1_y, 0, 1023, 0, 180);
  int posAntebrazo = map(joy2_x, 0, 1023, 0, 180);
  int posPinza = map(joy2_y, 0, 1023, 0, 180);

  mySerial.print("B");
  mySerial.println(posBase);
  mySerial.print("A");
  mySerial.println(posBrazo);
  mySerial.print("C");
  mySerial.println(posAntebrazo);
  mySerial.print("P");
  mySerial.println(posPinza);

  delay(20);
}
