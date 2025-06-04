// =============================================
// PROGRAMA DEL DRIVER DEL ROBOT: EJERCICIOS
// =============================================
// TECNOLOGIA ELECTRONICA DE COMPUTADORES 
// UNIVERSIDAD DE OVIEDO
// (C) jC Anton 2024 
//==============================================
/**
 * Nombres:
 * Elías Fernández Medina
 * Joaquín Jara García
 * Grupo:
 * L1
 */
#include <Arduino.h>
#include "config.h"

// Prototypes
static void serialSetup();
static void motorSetup();
static void opticSetup();
static void avanza();
static void retrocede();
static void giraLeft();
static void giraRight();
static void stop();
static void setVelocity(uint8_t velocity);
static uint16_t readOptic(const uint8_t channel);



// Global variables
uint16_t PhotoLeft  = 0;  
uint16_t PhotoRight = 0; 
uint8_t velocity = DUTY;


//==============================================================
// Setup function
//==============================================================
void setup() 
{
  // Initialize serial communication
  serialSetup();
  // Initialized motor driver
  motorSetup();
  // Initialized optic sensors
  opticSetup();

  // fija una velocidad 
  setVelocity(velocity);             
}



//==============================================================
// Main loop
//==============================================================
void loop(){


// test de movimentos del robot: avance, atras, giros

  //avanza(); 
  //delay(4000);
  //retrocede(); 
  //delay(4000);
  //giraLeft(); 
  //delay(2000);
  //giraRight(); 
  //delay(2000);
  //stop();
  //delay(4000);
  

// test de velocidad

  setVelocity(velocity);


// test de lectura del estado de los sensores ópticos

  PhotoLeft  = readOptic(PHOTO_LEFT_PIN);
  PhotoRight = readOptic(PHOTO_RIGHT_PIN);
  //  Serial.print("Photo left = ");
  //  Serial.println(PhotoLeft);
  //  Serial.print("Photo right = ");
  //  Serial.println(PhotoRight);
  if(PhotoLeft>600 && PhotoRight>700){ 
    stop();
  }
  if(PhotoRight>700){
    giraLeft();
  }
  if(PhotoLeft>600){
    giraRight();
  }
  if(PhotoLeft<600 && PhotoRight<700){//cambiamos la velocidad en cuando vamos hacia adelante
    setVelocity(velocity+100); 
    avanza();
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
   // inicialización de los pines de control del motor
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

void opticSetup()
{
   // inicialización de los pines photodiodes
   pinMode(PHOTO_LEFT_PIN, INPUT);
     pinMode(PHOTO_RIGHT_PIN, INPUT);
   // resto de pines ...
}

void avanza() {
digitalWrite(AIN1_PIN, HIGH); // motor derecho
digitalWrite(AIN2_PIN, LOW);   
digitalWrite(BIN1_PIN, HIGH); // motor izquierdo
digitalWrite(BIN2_PIN, LOW);      
} 


void retrocede() { //no lo usamos
digitalWrite(AIN1_PIN, LOW); // motor derecho
digitalWrite(AIN2_PIN, HIGH);   
digitalWrite(BIN1_PIN, LOW); // motor izquierdo
digitalWrite(BIN2_PIN, HIGH);           
} 

void giraLeft() {
digitalWrite(AIN1_PIN, HIGH); // motor derecho
digitalWrite(AIN2_PIN, LOW);    //rueda dcha hacia atrás, izq hacia delante
digitalWrite(BIN1_PIN, LOW); // motor izquierdo
digitalWrite(BIN2_PIN, HIGH);    
} 

void giraRight() {
digitalWrite(AIN1_PIN, LOW); // motor derecho
digitalWrite(AIN2_PIN, HIGH);    //rueda izq hacia atrás, dcha hacia delante
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
analogWrite(PWMB_PIN, velocity);   // motor izquierdo; le subimos la velocidad a la rueda izq poruqe en nuestro coche va más lenta
                                    // usamos el modelo 4, si este código es usado para otro modelo, eliminar el +(constante)
}

uint16_t readOptic(const uint8_t channel)
{
    return(analogRead(channel));  
}
