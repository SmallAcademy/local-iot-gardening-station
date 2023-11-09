#include <Wire.h>
#include <AHT20.h>

AHT20 AHT;
#define PIN_SOIL D1
#define PIN_PUMP D2

// variables
int moistureThreshold = 50; // Moisture level needed for the plant
int averageDuration = 120; // Number of records to make an average
bool toPlotter = true; // show data in plotter (true or false)

long moistureAvr = 0;
long moistureReadingCounter = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_SOIL, INPUT);
  AHT.begin();
}

void loop() {

  delay(1000);
  long now = millis();
    
  // Temperature and humidity
  float humi, temp;
  int ret = AHT.getSensor(&humi, &temp);
  humi = humi * 100;

  // Soil moisture
  int rawADC = analogRead(PIN_SOIL);
  int moistPct = map(rawADC, 2500, 2000, 0, 100);
  if (moistPct > 100) {
    moistPct = 100;
  } else if (moistPct < 0) {
    moistPct = 0;
  }
  moistureReadingCounter++;
  moistureAvr = moistureAvr + moistPct;

  // show readings
  if (toPlotter == false) {
    Serial.println("Temperature: " + String(temp) + "°C");
    Serial.println("Air humidity: " + String(humi) + "%");
    Serial.println("Soil moisture: " + String(moistPct) + "%");
    Serial.println("Counter: " + String(moistureReadingCounter));
    Serial.println("");
  } else {
    Serial.print("Temperature(°C):");
    Serial.print(temp);
    Serial.print(",");
    Serial.print("Air_Humidity(%):");
    Serial.print(humi);
    Serial.print(",");
    Serial.print("Soil_Moisture(%):");
    Serial.println(moistPct);
  }

  // take moisture reading for a given number of times and calculate the average
  if (moistureReadingCounter >= averageDuration ) {
    moistPct = (int) (moistureAvr / moistureReadingCounter);
    if (toPlotter == false) {
      Serial.println("=====> Soil moisture average: " + String(moistPct) + "%");
      Serial.println("");
    }
    if (moistPct < moistureThreshold ) {
      digitalWrite(PIN_PUMP, HIGH);
      delay(5000);
      digitalWrite(PIN_PUMP, LOW);
    }
    moistureReadingCounter = 0;
    moistureAvr = 0;
  }

}
