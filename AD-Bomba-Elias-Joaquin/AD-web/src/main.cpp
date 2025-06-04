/**
 * Nombres:
 * Elías Fernández Medina
 * Joaquín Jara García 
 * Grupo:
 * L1
 */
#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "secrets.h"
// handling communication between SinricPro Server and your devices.
#include <SinricPro.h>
// device suporting basic on / off commands.
#include <SinricProSwitch.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Wire.h>
#include <ctime>
//protypes functions

static void setupWiFi();
static void setupSerial();
static bool onPowerState(const String &deviceId, bool &state);
static void getForecast(const char *latitude,
                        const char *longitude,
                        const char *timezone,
                        const char *days);
static void setupI2c();
static void setupBme680();
static int getCurrentHour();
Adafruit_BME680 bme;
uint8_t dutyCustom = DUTY;
int hora = getCurrentHour();
int precipitacions[24];
void setup() {
  pinMode(AD_PIN, INPUT);
  pinMode(PULSADOR_PIN, INPUT);
  pinMode(MOSFET_PIN, OUTPUT);
  uint16_t lectura = analogRead(AD_PIN);
  pinMode(BUZZER_PIN, OUTPUT);
  setupSerial(); 
  setupWiFi();
  setupBme680();
  setupI2c();
  // create and add a switch to SinricPro
  SinricProSwitch& mySwitch = SinricPro[SWITCH_ID];

// set callback function for powerState request.
  mySwitch.onPowerState(onPowerState);

// startup SinricPro
  SinricPro.begin(APP_KEY, APP_SECRET);
   getForecast(LATITUDE, LONGITUDE, TIMEZONE, PREDICTION_DAYS);

}


void loop() {
    // put your main code here, to run repeatedly:
    hora = getCurrentHour();
  if (!bme.performReading())
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");




  uint8_t lecturaPulsador = digitalRead(PULSADOR_PIN);
  Serial.print(analogRead(AD_PIN));
  delay(MSG_DELAY_MS);
  SinricPro.handle();
  if(analogRead(AD_PIN)<HUMIDITY_THRESHOLD){//si la lectura es mayor que el umbral establecido(sensor húmedo), no riega
     digitalWrite(BUZZER_PIN, HIGH);
     analogWrite(MOSFET_PIN, 0);
  }else if(!analogRead(AD_PIN)<HUMIDITY_THRESHOLD){ //si la lectura es mayor que el umbral establecido(sensor seco), riega
    digitalWrite(BUZZER_PIN, LOW);
    analogWrite(MOSFET_PIN, DUTY);
  }
  if(lecturaPulsador==HIGH || sinricState){ //pulsar boton 1 o utilizar el sinricPro para regar
    analogWrite(MOSFET_PIN, DUTY+(255-DUTY));
  }else if((lecturaPulsador==LOW && analogRead(AD_PIN)<HUMIDITY_THRESHOLD) || !sinricState){ //si no están accionados, no riega
    analogWrite(MOSFET_PIN, 0);
  }
  if(bme.temperature >30){//control de temperatura, si la temp es más de 30 riega
    analogWrite(MOSFET_PIN, DUTY-25);
  }
    int siguienteHora = (hora + 1) % 24; //si es 24 se pone en la 0
  if(precipitacions[siguienteHora]>40){ //si quiere comprobarse el funcionamiento(en caso de no lluvia) cambiese el ">" por "<" y el 0 del analogWrite por un DUTY
    analogWrite(MOSFET_PIN, 0); //no riega si la posibilidad de lluvia es de más del 40%
  }
  SinricPro.handle();

}


void setupSerial() {
Serial.begin(SERIAL_BAUDS);
while (!Serial) ;
}


void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.setSleep(false); //disable wifi power saving modes    
  WiFi.setAutoReconnect(true);  // reconnection in case of drop
  WiFi.disconnect(true);        //disconnect from WiFi to set new WiFi connection

  //Initializes the WiFi library’s network settings and provides the current status.
  WiFi.begin(WIFI_SSID, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD); 
  
  while (WiFi.status() != WL_CONNECTED)   // check connection state
  {
    Serial.printf(".");
    delay(WIFI_RETRY_DELAY_MS);
  }
  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("device %s turned %s\r\n", deviceId.c_str(), state?"on":"off");
  digitalWrite(MOSFET_PIN, state);   // activamos o desactivamos la bomba de agua
  sinricState = state;
  return true; // indicate that callback handled correctly
}
void getForecast(const char *latitude,
                 const char *longitude,
                 const char *timezone,
                 const char *days)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;

    String url = "https://api.open-meteo.com/v1/forecast?";
    url += "latitude=";
    url += latitude;
    url += "&longitude=";
    url += longitude;
    url += "&hourly=precipitation_probability&timezone=";
    url += timezone;
    url += "&forecast_days=";
    url += days;

    http.begin(url);

    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();

      // Use https://arduinojson.org/v6/assistant to compute mininum capacity
      DynamicJsonDocument doc(2048);

      DeserializationError error = deserializeJson(doc, payload);

      if (error)
      {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      JsonObject hourly = doc["hourly"];
      JsonArray timeArray = hourly["time"];
      JsonArray precipitationArray = hourly["precipitation_probability"];
      Serial.println("Precipitation Probability:");
      for (size_t i = 0; i < precipitationArray.size(); i++)
      {
        String time = timeArray[i];
        int precipitation = precipitationArray[i];
        precipitacions[i] = precipitation;
        Serial.print("Time: ");
        Serial.print(time);
        Serial.print("Precipitation probability: ");
        Serial.println(precipitation);
      }
    
    }
    else
    {
      Serial.println("Error on HTTP request");
    }

    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }

}
void setupI2c()
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
}

void setupBme680()
{
  if (!bme.begin(0x76))   // default sensor address  0x76
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1)
      ;
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
}
int getCurrentHour(){
  std::time_t currentTime = std::time(nullptr);
  std::tm* localTime = std:: localtime(&currentTime);
  return localTime->tm_hour;
}