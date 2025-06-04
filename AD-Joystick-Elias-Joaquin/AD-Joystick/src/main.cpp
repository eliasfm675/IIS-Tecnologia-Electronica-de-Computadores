/**
 * Nombres:
 * Elías Fernández Medina
 * Joaquín Jara García
 * Grupo:
 * L1
 */
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <config.h>
#define BUFFER_SIZE 8

// Prototypes
static void serialSetup();          // micro <-> consola serie
static void BluetoothSerialSetup(); // micro <-> HM10
static void serialSetup();
static void motorSetup();
static void avanza();
static void retrocede();
static void giraLeft();
static void giraRight();
static void stop();
static void setVelocity(uint8_t velocity);
static void scaleVelocity(int radio);

// Global variables
SoftwareSerial BluetoothSerial(NANO_RX_PIN, NANO_TX_PIN);
// pin TX del modulo BT HM-10 <-> conectado a pin RX (11) del Arduino Nano

uint8_t velocity = DUTY;
#define BUFFER_SIZE 8
char buffer[BUFFER_SIZE];  // Buffer para almacenar los 8 bytes de la trama
uint8_t byteIndex = 0;  // Ãndice para el buffer

//==============================================================
// Setup loop
//==============================================================
void setup()    {
  BluetoothSerialSetup();
  serialSetup();
  motorSetup();
  setVelocity(velocity);   

}


//==============================================================
// Main loop
//==============================================================

void loop() {
  // Revisamos si hay datos disponibles desde el mÃ³dulo Bluetooth
  if (BluetoothSerial.available()) {
    // Leemos el byte y lo almacenamos en el buffer
    buffer[byteIndex] = BluetoothSerial.read();
    //Serial.write(buffer[byteIndex]);
    byteIndex++;

    // Si hemos recibido 8 bytes completos (completa la trama)
    if (byteIndex == BUFFER_SIZE) {
      // Mostrar la trama recibida en formato hexadecimal para depuraciÃ³n

      // Verificar el cuarto byte (Ã­ndice 6) para decidir la acciÃ³n
        char actionByte = buffer[6];  // El cuarto byte en la trama (Ã­ndice 6) indica la acciÃ³n
         // Ejecutar la acciÃ³n correspondiente segÃºn el valor del cuarto byte
        Serial.write(actionByte);
        int angle = (actionByte >> 3) * 15;  // Los primeros 5 bits multiplicados por 15
        int radio = actionByte & 0x07;        // Los últimos 3 bits para el radio

        // Mostrar el ángulo y el radio para depuración
        Serial.print("Ángulo: ");
        Serial.println(angle);
        Serial.print("° | Radio: ");
        Serial.println(radio);
        // Ajustar la velocidad del robot según el radio (por ejemplo, entre 0 y 255)
        scaleVelocity(radio);  // Escalar el radio a un valor de velocidad
        // Determinar la dirección del movimiento en función del ángulo

        if(radio == 0){
          stop();  // Detenerse
        }
        if (angle > -165 && angle < 45 && angle != 0) {
          //avanza();  // Avanzar
          giraRight();
        } else if (angle >= 45 && angle < 135) {
          //giraRight();  // Girar a la derecha
          avanza();
        } else if (angle >= 135 && angle < 225) {
          //retrocede();  // Retroceder
          giraLeft();
        } else if (angle < -165 && angle>-225){
          retrocede();
        }
        
         // Resetear el índice para recibir la siguiente trama
      byteIndex = 0;
      } 
        
      }
             

      
    }
  

       
    



void serialSetup()
{
  Serial.begin(SERIAL_BAUDS);
  while (!Serial)
    ; // Wait until serial is ready
}
void motorSetup()
{
   // inicializaciÃ³n de los pines de control del motor
   pinMode(AIN1_PIN,OUTPUT);
   pinMode(AIN2_PIN,OUTPUT);
   pinMode(BIN1_PIN,OUTPUT);
   pinMode(BIN2_PIN,OUTPUT);
   // resto de pines ...
   pinMode(PWMB_PIN,OUTPUT);
   pinMode(PWMA_PIN,OUTPUT);
   // activa driver
   digitalWrite(STBY_PIN, HIGH);
}

void BluetoothSerialSetup()
{
  BluetoothSerial.begin(SERIAL_BAUDS);
  while (!BluetoothSerial)
    ; // Wait until serial is ready
}
void avanza() {
digitalWrite(AIN1_PIN, HIGH); // motor derecho
digitalWrite(AIN2_PIN, LOW);   
digitalWrite(BIN1_PIN, HIGH); // motor izquierdo
digitalWrite(BIN2_PIN, LOW);      
} 


void retrocede() {
digitalWrite(AIN1_PIN, LOW); // motor derecho
digitalWrite(AIN2_PIN, HIGH);   
digitalWrite(BIN1_PIN, LOW); // motor izquierdo
digitalWrite(BIN2_PIN, HIGH);           
} 

void giraLeft() {
digitalWrite(AIN1_PIN, HIGH); // motor derecho
digitalWrite(AIN2_PIN, LOW);    
digitalWrite(BIN1_PIN, LOW); // motor izquierdo
digitalWrite(BIN2_PIN, LOW);    
} 

void giraRight() {
digitalWrite(AIN1_PIN, LOW); // motor derecho
digitalWrite(AIN2_PIN, LOW);    
digitalWrite(BIN1_PIN, HIGH); // motor izquierdo
digitalWrite(BIN2_PIN, LOW);        
} 

void stop() {
  digitalWrite(AIN1_PIN, LOW); // motor derecho
digitalWrite(AIN2_PIN, LOW);    
digitalWrite(BIN1_PIN, LOW); // motor izquierdo
digitalWrite(BIN2_PIN, LOW);           
} 


void setVelocity (uint8_t velocity) {
analogWrite(PWMA_PIN, velocity);  // motor derecho
analogWrite(PWMB_PIN, velocity);  // motor izquierdo; le subimos la velocidad a la rueda izq poruqe en nuestro coche va mÃ¡s lenta
                                    // usamos el modelo 4, si este cÃ³digo es usado para otro modelo, eliminar el +(constante)
}
// Función para escalar la velocidad según el radio
void scaleVelocity(int radio) {

  // Proporción lineal de velocidad según el radio
  if (radio == 0) {
    setVelocity(0);  // Sin velocidad si el radio es 0
  } else if (radio == 1) {
      setVelocity(32);  // Radio 1: Baja velocidad
  } else if (radio == 2) {
      setVelocity(64);  // Radio 2: Velocidad media
  } else if (radio == 3) {
      setVelocity(120);  // Radio 3: Velocidad media-alta 
  } else if(radio == 4){
      setVelocity(160);  // Radio 4: Alta velocidad
  }else {
      setVelocity(255); // Radio 5-7: Máxima velocidad
  }

}