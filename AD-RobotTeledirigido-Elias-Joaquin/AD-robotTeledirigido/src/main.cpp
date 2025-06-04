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
              switch (actionByte) {
                case 0x02:
                  retrocede();  // Retroceder
                  break;
                case 0x04:
                  giraLeft();   // Girar a la izquierda
                  break;
                case 0x08:
                  giraRight();  // Girar a la derecha
                  break;
                case 0x01:
                  avanza();     // Avanzar
                  break;
                case 0x00:
                  stop();       // Parar
                  break;
                default:
                  Serial.println("Comando desconocido");
                  break;
              }
              // Resetear el Ã­ndice para recibir la siguiente trama
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