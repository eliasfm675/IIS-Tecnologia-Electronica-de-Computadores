#ifndef __CONFIG_H
#define __CONFIG_H

#include <Arduino.h>
const unsigned long SERIAL_BAUDS = 9600; // Bauds for the serial communication
const int MSG_DELAY_MS = 200;      // Milliseconds to wait between messages

const int AD_PIN = 32;  //pin AD

const int BUZZER_PIN = 13;  //pin buzzer
const int HUMIDITY_THRESHOLD = 3000;  // umbral humedad

const int RELE_PIN = 17;  // pin rele
const int MOSFET_PIN = 17;  // pin MOSFET
const int PULSADOR_PIN = 16;  // pin pulsador bomba

const int WIFI_RETRY_DELAY_MS = 2000;
const int DUTY = 127;

// Weather Prediction :::::::::::::::::::::::::
const char* LATITUDE        = "52.52";
const char* LONGITUDE       = "13.41";
const char* TIMEZONE        = "Europe%2FMadrid";
const char* PREDICTION_DAYS = "1";
bool sinricState = false;

// I2C ::::::::::::::::::::::::::::::::::::::::
const uint8_t I2C_SDA_PIN = 21;
const uint8_t I2C_SCL_PIN = 22;

// BME680 :::::::::::::::::::::::::::::::::::::
const uint8_t BME680_ADDR = 0x76;

#endif // __CONFIG_H