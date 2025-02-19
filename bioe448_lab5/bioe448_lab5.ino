#include <ArduinoBLE.h>
BLEService newService("180A"); // creating the service
BLEByteCharacteristic readChar("2A57", BLERead);
BLEByteCharacteristic writeChar("2A58", BLEWrite);

const int trigPin = 8;
const int echoPin = 10;
long duration;
int distanceCm, distanceInch;

void setup() {

  // peripheral pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // LED pins
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  Serial.begin(9600);

  while(!Serial);
  if (!BLE.begin()){
    Serial.println("Waiting for ArduinoBLE");
    while(1);
  }

  BLE.setLocalName("VJP");
  BLE.setAdvertisedService(newService);
  newService.addCharacteristic(readChar);
  newService.addCharacteristic(writeChar);
  BLE.addService(newService);

  readChar.writeValue(0);
  writeChar.writeValue(0);

  BLE.advertise();
  Serial.println("Bluetooth device active");

}

void loop() {

  // Sending trigger pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // good practice to send low state to peripheral first
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  // Calibrating sensor
  distanceCm = duration / 58;
  distanceInch = duration / 148;
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm/");
  Serial.print(distanceInch);
  Serial.println(" in");

  // //LED feedback for distance
  //   if (distanceInch > 10) {
  //   digitalWrite(3, HIGH);
  //   digitalWrite(4, LOW);
  // } 
  // else {
  //   digitalWrite(4, HIGH);
  //   digitalWrite(3, LOW);   
  // }

  delay(1000);

  readChar.writeValue(distanceCm);

  // if (writeChar.written()) {
  //   if (writeChar.value()) {
  //   }
  // }
 
}
