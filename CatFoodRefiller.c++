// Load Cell -> Amplifier Library
#include "HX711.h" 

// Servo Library ESP32
#include <ESP32_Servo.h>

// Servo Object Buat Control Servo
Servo myservo;

// Blynk Setup
#define BLYNK_TEMPLATE_ID "TMPL6a926nfD-"
#define BLYNK_TEMPLATE_NAME "hkk"
#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Pin Load Cell 
#define DOUT  23
#define CLK  22

// Setup Blynk Auth, Wifi SSID dan PASS
char auth[] = "wvf6yH-qqnH43FVXNwoSl6m7EB4EAC7q";
char ssid[] = "Nomi";
char pass[] = "hardanti";

// Pin Setup Ultrasonik
const int trigPin = 17;
const int echoPin = 16;

// Setup Variable Storer Ultrasonik
int durationPulse;
int distanceCm;
int pos = 90; // --> Ubah posisi servo myservo.write(pos);

// Setup Servo pins
int servoPin = 32;

// Setup Scale Load Cell w HX711 Amplifier
HX711 scale(DOUT, CLK);

float calibration_factor = 241475.00; //- Inisiasi Cal Factor Awal
 

void setup() {

  // Serial console setup
  Serial.begin(9600);

  // Blynk Setup
  Blynk.begin(auth, ssid, pass);

  // Load Cell Setup 
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); 
  Serial.print("Zero factor: "); 
  Serial.println(zero_factor);

  // Ultrasonik Setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Servo Setup
  myservo.attach(servoPin);
  myservo.write(pos); 
}
 
void loop() {
  // Run Blynk in Loop
  Blynk.run();

  // Adjusting Calibration Factor
  scale.set_scale(calibration_factor); 

  // Enable Ultrasonic Function
  digitalWrite(trigPin, HIGH);
  delay(100);
  digitalWrite(trigPin, LOW);

  durationPulse = pulseIn(echoPin, HIGH);
  distanceCm = 0.017 * durationPulse;

  // Serial Print LoadCell Value
  Serial.print("Reading: ");
  float weight = scale.get_units();
  Serial.print(weight, 3);
  Serial.print(" kg"); 
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();

  // Serial Print Ultrasonik
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println("cm");
  delay(100);

  // Copy Current LoadCell Value in Blynk
  Blynk.virtualWrite(V1, weight);

  // Copy Current Ultrasonik Value in Blynk
  Blynk.virtualWrite(V2, distanceCm);  

  if(weight <= 0.180 && distanceCm <= 2){
    pos = 0;
    myservo.write(pos);
    delay(3000);
    pos = 90;
    myservo.write(pos);
  }else{
    pos = 90;
    myservo.write(pos);
  }
}

// Button Condition Handler
BLYNK_WRITE(V3) {
  int buttonState = param.asInt();
  
  // If Logic Handler
  if(buttonState = 1){
    pos = 0;
    myservo.write(pos);
    delay(3000);
    pos = 90;
    myservo.write(pos);
    Blynk.virtualWrite(V3, 0);
  }
}