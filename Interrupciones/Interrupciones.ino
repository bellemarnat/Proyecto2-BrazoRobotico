#include <Servo.h>

const int buttonPin = 2;
const int motorPin = 9;

volatile bool buttonPressed = false;

Servo motor;
unsigned long timerStartTime = 0;
const unsigned long timerDuration = 5000;  // 5 segundos

void setup() {
  // Configurar pin de botón como entrada con resistencia de pull-down externa
  DDRB &= ~(1 << DDB2);  // Configurar pin 2 como entrada
  PORTB |= (1 << PORTB2); // Activar resistencia de pull-down en pin 2

  // Configurar pin de motor como salida
  DDRB |= (1 << DDB1);  // Configurar pin 9 como salida

  // Configurar y mover el servo motor a la posición inicial
  motor.attach(motorPin);
  motor.write(90);

  Serial.begin(9600);

  // Configurar la interrupción para el cambio de estado en el pin del botón
  PCICR |= (1 << PCIE0);                     // Habilitar interrupciones por cambio de estado en el puerto B
  PCMSK0 |= (1 << PCINT2);                    // Habilitar interrupciones para PB2 (pin 2)
}

void loop() {
  // Tu código principal aquí
  // Por ejemplo, verificar el estado del botón
  if (buttonPressed) {
    // Mover el servo motor 90 grados
    motor.write(0);

    // Iniciar el temporizador
    timerStartTime = millis();

    // Restablecer el estado del botón
    buttonPressed = false;
  }

  // Verificar si ha pasado el tiempo del temporizador
  if (millis() - timerStartTime >= timerDuration) {
    // Mover el servo motor a la posición inicial
    motor.write(90);
  }
}

// Rutina de interrupción para el cambio de estado en el pin del botón
ISR(PCINT0_vect) {
  // Verificar el estado del botón y actualizar la bandera correspondiente
  if (!(PINB & (1 << PINB2))) {
    buttonPressed = true;
  }
}
