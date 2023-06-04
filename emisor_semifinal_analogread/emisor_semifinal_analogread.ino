#include <SoftwareSerial.h>

#define JOY1_X A0
#define JOY1_Y A1
#define JOY2_X A2
#define JOY2_Y A3

SoftwareSerial mySerial(4, 5); // RX, TX


void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(JOY1_X, INPUT);
  pinMode(JOY1_Y, INPUT);
  pinMode(JOY2_X, INPUT);
  pinMode(JOY2_Y, INPUT);
  
}

int readADC(int channel) {
  ADMUX &= 0xF0;
  ADMUX |= channel;
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC));
  return ADC;
}

void loop() {
  int joy1_x = analogRead(JOY1_X);
  int joy1_y = analogRead(JOY1_Y);
  int joy2_x = analogRead(JOY2_X);
  int joy2_y = analogRead(JOY2_Y);

  int posBase = map(joy1_x, 0, 1023, 0, 180);
  int posBrazo = map(joy1_y, 0, 1023, 0, 180);
  int posAntebrazo = map(joy2_x, 0, 1023, 0, 180);
  int posPinza = map(joy2_y, 0, 1023, 0, 180);

  mySerial.print(posBase);
  mySerial.print(",");
  mySerial.print(posBrazo);
  mySerial.print(",");
  mySerial.print(posAntebrazo);
  mySerial.print(",");
  mySerial.println(posPinza);
  
  Serial.print(posBase);
  Serial.print(",");
  Serial.print(posBrazo);
  Serial.print(",");
  Serial.print(posAntebrazo);
  Serial.print(",");
  Serial.println(posPinza);
  delay(100);
}