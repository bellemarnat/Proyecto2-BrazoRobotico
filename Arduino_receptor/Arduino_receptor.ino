// ******************************************************** Arduino receptor

#include <Servo.h> // procesar datos 

Servo servo1; // Base
Servo servo2; // Brazo
Servo servo3; // Antebrazo
Servo servo4; // Pinza

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 bps

  servo1.attach(8, 610, 2550);
  servo2.attach(9, 670, 2540);
  servo3.attach(10, 660, 2600);
  servo4.attach(11, 660, 2600);
}

void loop() {
  if (Serial.available() >= 10) {
    char receivedChar;
    int posBase;
    int posBrazo;
    int posAntebrazo;
    int posPinza;

    // Espera a recibir el marcador "B" para la posición de la base
    if (Serial.read() == 'B') {
      posBase = Serial.parseInt();
      // Espera a recibir el marcador "A" para la posición del brazo
      if (Serial.read() == 'A') {
        posBrazo = Serial.parseInt();
        // Espera a recibir el marcador "C" para la posición del antebrazo
        if (Serial.read() == 'C') {
          posAntebrazo = Serial.parseInt();
          // Espera a recibir el marcador "P" para la posición de la pinza
          if (Serial.read() == 'P') {
            posPinza = Serial.parseInt();

            // Mueve los servos a las posiciones recibidas
            servo1.write(posBase);
            servo2.write(posBrazo);
            servo3.write(posAntebrazo);
            servo4.write(posPinza);
          }
        }
      }
    }
  }
}

/** conectar los pines 1 (Tx) y 0 (Rx) de los dos Arduinos para establecer la comunicación serial entre ellos. 
El Arduino Uno con el código del emisor enviará las posiciones de los servos a través de la comunicación serial,
y el Arduino Uno con el código del receptor recibirá las posiciones y moverá los servos en consecuencia. 

Guia de conecciones: 

Materiales necesarios:
- 2 placas Arduino Uno
- 4 servomotores
- Joystick analógico
- Pantalla LCD
- Cables de conexión

**Conexiones en el Arduino Uno (Emisor):**
1. Conecta el pin VCC del joystick al pin 5V del Arduino Uno.
2. Conecta el pin GND del joystick al pin GND del Arduino Uno.
3. Conecta el pin X del joystick (eje X) al pin analógico A0 del Arduino Uno.
4. Conecta el pin Y del joystick (eje Y) al pin analógico A1 del Arduino Uno.
5. Conecta el pin Z del joystick (botón) al pin digital 7 del Arduino Uno.
6. Conecta los pines de control de los servomotores (8, 9, 10, 11) a los pines correspondientes del Arduino Uno.
7. Conecta los pines RS, E, D4, D5, D6, D7 de la pantalla LCD a los pines 12, 11, 5, 4, 3, 2 del Arduino Uno respectivamente.


**Conexiones en el Arduino Uno (Receptor):**
1. Conecta los pines de control de los servomotores (8, 9, 10, 11) a los pines correspondientes del Arduino Uno.

**Conexión entre los Arduinos:**
1. Conecta el pin 1 (Tx) del Arduino Uno (emisor) al pin 0 (Rx) del Arduino Uno (receptor).
2. Conecta el pin 0 (Rx) del Arduino Uno (emisor) al pin 1 (Tx) del Arduino Uno (receptor).

Una vez que hayas realizado todas las conexiones, puedes cargar los códigos en los respectivos Arduinos y alimentarlos con una fuente de alimentación adecuada. 
Asegúrate de que ambos Arduinos estén correctamente configurados para la comunicación serial a 9600 bps.

Con estas conexiones, el Arduino Uno (emisor) leerá los valores del joystick, controlará los servomotores y enviará las posiciones a través de la comunicación serial. 
El Arduino Uno (receptor) recibirá las posiciones a través de la comunicación serial y controlará los servomotores según las posiciones recibidas.

Recuerda revisar las especificaciones y pines de los componentes que estás utilizando, ya que pueden variar según la marca y modelo. 
También es importante verificar la compatibilidad de voltajes entre los componentes y proporcionar una fuente de alimentación adecuada.

**/



