const int pinMic = 13;                    // Pin digital del micrófono (actualizado al pin 13)
const int numLeds = 11;                   // Total de LEDs (del 2 al 12 son 11 pines)
const int pines[numLeds] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; // Pines del 2 al 12

int indiceActual = 0;           // Controla qué LED se enciende o apaga
bool encendiendo = true;        // Estado: true = encendiendo, false = apagando
unsigned long ultimoAplauso = 0;
const int tiempoEspera = 300;   // Antirrebote en milisegundos

void setup() {
  pinMode(pinMic, INPUT_PULLUP); // Micrófono con pull-up interno
  
  // Configuramos los 11 LEDs como salida y los apagamos
  for (int i = 0; i < numLeds; i++) {
    pinMode(pines[i], OUTPUT);
    digitalWrite(pines[i], LOW);
  }
  
  Serial.begin(9600);
  Serial.println("¡Sistema listo! Aplaude para encender y luego para apagar en secuencia.");
}

void loop() {
  // Detecta el aplauso de forma limpia
  if (digitalRead(pinMic) == LOW && (millis() - ultimoAplauso) > tiempoEspera) {
    ultimoAplauso = millis();
    
    if (encendiendo) {
      // FASE 1: Encender en secuencia (del pin 2 al 12)
      digitalWrite(pines[indiceActual], HIGH);
      Serial.print("¡Aplauso! Encendiendo pin: ");
      Serial.println(pines[indiceActual]);
      
      indiceActual++;
      
      // Si ya encendimos el último, cambiamos el sentido a apagar
      if (indiceActual >= numLeds) {
        indiceActual = numLeds - 1; // Nos posicionamos en el último LED (pin 12)
        encendiendo = false;        // Cambiamos el modo a apagar
        Serial.println("--- Secuencia de encendido completa. El siguiente aplauso comenzará a apagar ---");
      }
      
    } else {
      // FASE 2: Apagar en secuencia (del pin 12 al 2)
      digitalWrite(pines[indiceActual], LOW);
      Serial.print("¡Aplauso! Apagando pin: ");
      Serial.println(pines[indiceActual]);
      
      indiceActual--;
      
      // Si ya apagamos el primero, reiniciamos el ciclo para volver a encender
      if (indiceActual < 0) {
        indiceActual = 0;           // Nos posicionamos en el primer LED (pin 2)
        encendiendo = true;         // Cambiamos el modo a encender
        Serial.println("--- Todos apagados. Listo para volver a encender ---");
      }
    }
  }
}