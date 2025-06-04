#ifndef __CONFIG_H
#define __CONFIG_H
#include <Arduino.h>

const uint8_t AIN1_PIN = 2; // pines control motor derecho
const uint8_t AIN2_PIN =  3;//
const uint8_t BIN1_PIN =  4; // pines control motor izquierdo
const uint8_t BIN2_PIN  = 5; //
const uint8_t PWMA_PIN =  9; // velocidad motor derecho   PWM
const uint8_t PWMB_PIN  = 10; // velocidad motor izquierdo PWM
const uint8_t STBY_PIN  =  13; // habilitación de motores (Standby)

const uint8_t PHOTO_LEFT_PIN  =  A0; //   pin del Arduino conectado al sensor óptico Izquierdo
const uint8_t PHOTO_RIGHT_PIN =  A1;//   pin del Arduino conectado al sensor óptico Derecho

const uint8_t DUTY = 150; //con 64 va lento pero bien //85 no va del todo bien

const uint8_t NANO_RX_PIN = 11;
const uint8_t NANO_TX_PIN = 12;
const unsigned long SERIAL_BAUDS = 9600;
#endif // __CONFIG_H