#include <SoftwareSerial.h>
const int irPin = 8;  // Broche à laquelle est connectée la sortie digitale du capteur IR
const int ledPin = 9;
int irValue = HIGH;
int previousIrState = HIGH;
int countFallingEdges = 0;
unsigned long samplingDuration = 2000;  // Durée d'échantillonnage en millisecondes
SoftwareSerial bluetooth(10, 11); // RX, TX

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(irPin, INPUT);
  Serial.begin(9600); 
  bluetooth.begin(9600); 
}

void loop() {
  unsigned long startTime = millis();  // Temps de début d'échantillonnage

  while (millis() - startTime < samplingDuration) {
    irValue = digitalRead(irPin);

    // Si le signal infrarouge passe de HIGH à LOW, incrémenter le compteur
    if (irValue == LOW && previousIrState == HIGH) {
      countFallingEdges++;
    }

    previousIrState = irValue;

    // Si la sortie digitale du capteur infrarouge est HIGH, allume la LED
    if (irValue == HIGH) {
      digitalWrite(ledPin, HIGH);
    } else {
      // Sinon, éteint la LED
      digitalWrite(ledPin, LOW);
    }
  }

  // Calcul de la vitesse angulaire en tr/min
  double vitesseAngulaire = (countFallingEdges) * 60000.0 / (double)(samplingDuration) * 8;
  // Conversion de la vitesse angulaire en vitesse linéaire (m/s)
  double vitesseLineaire = vitesseAngulaire * (2 * PI * 1) / 60; // Supposons que le rayon de la roue est de 1 mètre pour l'exemple
  double vitesseKmH = vitesseLineaire * 3.6; // Conversion de m/s en km/h

  // Affiche le nombre de fronts descendants et la vitesse angulaire pendant la durée d'échantillonnage
  String data = "Vitesse vélo : " + String(vitesseKmH) + " km/h\n";

  Serial.print(data);       
  bluetooth.print(data);    

  // Réinitialise le compteur pour la prochaine mesure
  countFallingEdges = 0;

  delay(1);
}
