#include "Wire.h"
#include <Stepper.h>

float m = 80;
float ax = 20;
float g = 9.8;
float R=0.29;
float f=0.01;
float k=0.5;
float Vv=0;
float v=20;
float r;
float Fr;
float Cp = 10;
const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
int minVal = 265;
int maxVal = 402;
double x;
double y;
double z;
int targetPosition;

// Nombre de pas pour une révolution complète du moteur pas à pas
const int stepsPerRevolution = 200;

// Création d'une instance de la classe Stepper avec les broches de contrôle du moteur
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

// Tableau des valeurs associées à chaque position (pas) du moteur
const int positionValues[] = {1, 2, 3, 4};

// Variable pour stocker la position actuelle du moteur
int currentPosition = 0;
// Déclaration correcte de la fonction moveStepper
void moveStepper(int targetPosition) {
  if (targetPosition < 0 || targetPosition >= stepsPerRevolution) {
    Serial.println("Position invalide.");
    return;
  }

  Serial.print("Déplacement vers la position: ");
  Serial.println(targetPosition);
  while (currentPosition != targetPosition) {
    if (currentPosition < targetPosition) {
      myStepper.step(9.5);
      currentPosition++;
    } else {
      myStepper.step(-9.5);
      currentPosition--;
    }

    delay(100);
    Serial.print("Position actuelle: ");
    Serial.println(currentPosition);
    Serial.print("Pas actuel: ");
    Serial.println(positionValues[currentPosition]);
  }
}

void setup() {
  myStepper.setSpeed(60);
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);
  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  Serial.print("AngleX= ");
  Serial.println(x);
  delay(400);
  Fr=m*g*sin(x)+f*m*g+k*((v+Vv)*(v+Vv));
  r = ((m*ax + Fr)*R)/Cp;
    if (Cp!=0 and r<=58.7446272 and r >=47.205504) { 
      targetPosition =3;}
    if  (Cp!=0 and r<=47.205504 and r>=41.4359424) { 
      targetPosition =2;}
    if  (Cp!=0 and r<=41.4359424 and r>=35.9286336) { 
      targetPosition =1;}
    if  (Cp!=0 and r<=35.9286336) { 
      targetPosition =0; }     
    if (Cp ==0 and v ==0) { targetPosition = 0;}
  moveStepper(targetPosition);

  // Affichage de la position actuelle et de la valeur du pas actuel
  Serial.print("Position actuelle: ");
  Serial.println(currentPosition);
  Serial.print("Pas actuel: ");
  Serial.println(positionValues[currentPosition]);

  // Attente de 2 secondes avant le prochain mouvement
  delay(2000);
}
