#include <SoftwareSerial.h>
const int windSensor1Pin = A0;
const int windSensor2Pin = A1;


float vitesseEtalonage1 = 9.4;
float tensionEtalonage1 = 0.36;
float  vitesseEtalonage2 = 9.4;
float tensionEtalonage2 = 1.23;

SoftwareSerial bluetooth(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);      
  bluetooth.begin(9600);   
}

void loop() {
  // Lecture des valeurs des capteurs
  float tensionArduino1 = tensionEtalonage1 * (1023.0 / 5.0);
  float vitesse1 = analogRead(windSensor1Pin) * (vitesseEtalonage1 / tensionArduino1);

  float tensionArduino2 = tensionEtalonage2 * (1023.0 / 5.0);
  float vitesse2 = analogRead(windSensor2Pin) * (vitesseEtalonage2 / tensionArduino2);

  float vitesse1KmH = vitesse1 * 3.6;
  float vitesse2KmH = vitesse2 * 3.6;


  float vitesseDifference = vitesse1KmH - vitesse2KmH;

  Serial.print("Vitesse captée: ");
  Serial.print(vitesseDifference);
  Serial.println(" km/h");

  String data ="Vitesse captée: " + String(vitesseDifference) + " km/h\n";
  bluetooth.print(data);

  delay(1000);
}
