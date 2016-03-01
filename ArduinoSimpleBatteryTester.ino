#include <Time.h>

#define TERMINAL_VOLTAGE 0.2
#define V_METER A0
#define R_LOAD 2.2
#define PIN_LED 13

float voltage = 0;
float joules = 0;

uint8_t hours = 0;
uint8_t mins = 0;

uint8_t lastSecond = 0;
time_t startTime = 0;

bool batteryAttached = false;
bool testComplete = false;

void setup() {                
  pinMode(V_METER, INPUT);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  Serial.begin(9600);
}

void loop() {
  
  if (batteryAttached) {
  
    if (testComplete) {
      digitalWrite(PIN_LED, LOW);
    }else {
      
      time_t t = now() - startTime; 
      uint8_t currentSecond = second(t);
    
      if (currentSecond != lastSecond) {
    
        hours = hour(t);
        mins = minute(t);
      
        voltage = 5.0 * ((float) analogRead(V_METER)) / 1024.0;
      
        float current = voltage / R_LOAD;
        joules += voltage * current;
        float wattHours = (joules / 3600.0) * 1000.0;
      
        Serial.print(hours);
        Serial.print(":");
        Serial.print(mins);
        Serial.print(":");
        Serial.print(currentSecond);
        Serial.print(";");
        Serial.print(voltage);
        Serial.print(";");
        Serial.print(current);
        Serial.print(";");
        Serial.print(joules);
        Serial.print(";");
        Serial.print(wattHours);
        Serial.println("");
        lastSecond = currentSecond;
      
        if (voltage < TERMINAL_VOLTAGE) {
          testComplete = true;
        }
      
      }
    }
  }else {
    
    voltage = 5.0 * ((float) analogRead(V_METER)) / 1024.0;
    
    if (voltage > TERMINAL_VOLTAGE) {
      batteryAttached = true;
      digitalWrite(PIN_LED, HIGH);
    }
    
  }
  
}
