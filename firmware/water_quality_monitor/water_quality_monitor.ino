//e3354iot@edny.net 
//Info@2025 
#define BLYNK_PRINT Serial

#include "Secrets.h"
#include <WiFi.h> 
#include <WiFiClient.h> 
#include <BlynkSimpleEsp32.h>


// Blynk token must be configured locally; do not commit credentials.
char auth[] = BLYNK_AUTH_TOKEN; 
String receivedSrting; 

#include <LCD_I2C.h> 
LCD_I2C lcd(0x27); 
#define tds_sensor 35 
#define ph_sensor 34 
#define tur_sensor 39 
unsigned int ph_value,tds_value,tur_value; 
#include <OneWire.h> 
#include <DallasTemperature.h> 
// Data wire is plugged into port 2 on the Arduino 
#define ONE_WIRE_BUS 15 
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire); 
//DeviceAddress insideThermometer; 
int buttonread; 
float tempval,tempcutval=30,settemp=95,changetemp=6,t,tf,bpm; 
void setup() { 
  // put your setup code here, to run once: 
  Serial.begin(9600); 
  pinMode(tds_sensor, INPUT); 
  pinMode(ph_sensor, INPUT); 
  pinMode(tur_sensor, INPUT); 
  sensors.begin(); 
  lcd.begin(); // If you are using more I2C devices using the Wire library use lcd.begin(false) 
  lcd.backlight(); 
  lcd.setCursor(0, 0); 
  lcd.print("WATER "); // You can make spaces using well... spaces 
  lcd.setCursor(0, 1); // Or setting the cursor in the desired position. 
  lcd.print("QUALITY TEST"); 
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); 
  lcd.clear(); 
} 
void loop() { 
  Blynk.run(); 
  
  sensors.requestTemperatures(); 
  ph_value = analogRead(ph_sensor); 
  tds_value = analogRead(tds_sensor); 
  tur_value = analogRead(tur_sensor); 
  // 
  Serial.print("analog ph_value: "); 
  Serial.println(ph_value); 
  // Serial.print("analog tds_value: "); 
  // Serial.println(tds_value); 
  // 
  // Serial.print("analog tur_value: "); 
  // Serial.println(tur_value); 
  ph_value=map(ph_value,0,130,8,0); 
  tds_value=map(tds_value,0,3000,0,100); 
  tur_value=map(tur_value,0,4095,100,0); 
  // Serial.print("maping tur_value: "); 
  // Serial.println(tur_value); 
  tds_value=tds_value*10; 
  // Serial.print("maping tds_value: "); 
  // Serial.println(tds_value); 
  // Serial.print("ph_value: "); 
  // Serial.println(ph_value); 
  if(ph_value<=0||ph_value>=9) 
  { 
    ph_value=0; 
  } 
  if(tur_value<=15) 
  { 
    tur_value=0; 
  } 
  /* Serial.print("tds_value: "); 
  Serial.println(tds_value);*/ 
  Serial.print("ph_value: "); 
  Serial.println(ph_value); 
  tf = sensors.getTempCByIndex(0); // the index 0 refers to the first device 
  // tf=DallasTemperature::toFahrenheit(1); 
  // Serial.print("original Temp : "); 
  // Serial.println(tf);// Converts tempC to Fahrenheit 
  
  lcd.setCursor(0,0); 
  lcd.print("T:"); 
  if(tds_value<=9){
    lcd.print("00"); 
    lcd.print(tds_value);
  } 
  else if(tds_value<=99){
    lcd.print("0"); 
    lcd.print(tds_value);
  } 
  else if(tds_value<=999){
    lcd.print(""); 
    lcd.print(tds_value);
  } 

  delay(200); 
  lcd.setCursor(8,0); 
  lcd.print("PH:"); 
  if(ph_value<=9){
    lcd.print("00"); 
    lcd.print(ph_value);
  } 
  else if(ph_value<=99){
    lcd.print("0"); 
    lcd.print(ph_value);
  } 
  else if(ph_value<=999){
    lcd.print(""); 
    lcd.print(ph_value);
  } 
  lcd.setCursor(0,1); 
  lcd.print("TE:"); 
  if(tf<=9){
    lcd.print("00"); 
    lcd.print(tf);
  } 
  else if(tf<=99){
    lcd.print("0"); 
    lcd.print(tf);
  } 
  else if(tf<=999){
    lcd.print(""); 
    lcd.print(tf);
  } 
  lcd.setCursor(9,1); 
  lcd.print("TU:"); 
  if(tur_value<=9){
    lcd.print("00"); 
    lcd.print(tur_value);
  } 
  else if(tur_value<=99){
    lcd.print("0"); 
    lcd.print(tur_value);
  } 
  else if(tur_value<=999){
    lcd.print(""); 
    lcd.print(tur_value);
  } 
  if(buttonread==1) { 
    if(tds_value>=600&&tds_value<=750||tur_value>=11&&tur_value<=60) { 
      Blynk.virtualWrite(V4,"BAD WATER "); 
      Blynk.logEvent("msg","BAD WATER"); 
      Serial.println("bad water "); 
    } 
    else if(tds_value>=200&&tds_value<=550) { 
      Blynk.logEvent("msg","GOOD WATER"); 
      Blynk.virtualWrite(V4,"GOOD WATER "); 
      Serial.println("GOOD WATER "); 
    } 
    else if(tur_value>=90&&tur_value<=100) { 
      Blynk.logEvent("msg","BAD WATER"); 
      Blynk.virtualWrite(V4,"BAD WATER "); 
      Serial.println("BAD WATER "); 
    } 
    else { 
      Blynk.virtualWrite(V4," "); 
    } 
    /* Blynk.virtualWrite(V0,"TDS: "+String(tds_value)+" mg/litre"); 
    Blynk.virtualWrite(V1,"TEMP: "+String(tf)+" °C"); 
    Blynk.virtualWrite(V2,"TUR: "+String(tur_value)+" NTU"); 
    Blynk.virtualWrite(V3,"PH: "+String(ph_value));*/ 
    
    Blynk.virtualWrite(V0,ph_value); 
    Blynk.virtualWrite(V1,tds_value); 
    Blynk.virtualWrite(V2,tur_value); 
    Blynk.virtualWrite(V3,tf); 
    Serial.println("sensor value send blynk "); 
  } 
  
  delay(500); 
} 
BLYNK_WRITE(V5) { 
  int button = param.asInt(); 
  Serial.print(" Button value :"); 
  Serial.println(button); 
  buttonread=button; 
} 
BLYNK_WRITE(V6) { 
  String received = param.asStr(); 
  Serial.print(" Button value :"); 
  Serial.println(received); 
  receivedSrting=received; 
  Blynk.virtualWrite(V7,receivedSrting); 
  Blynk.logEvent("msg",receivedSrting); 
  delay(3000 ); 
  Blynk.virtualWrite(V7," "); 
}