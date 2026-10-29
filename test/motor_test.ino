/**
 * ======================================================================================
 * Project: AquaFlare — Autonomous Firefighting Robot
 * Utility: L298N Motor Driver Diagnostic & Polarity Verifier
 * Purpose: Iterates through Forward, Backward, Turn Left, Turn Right, and Stop
 *          to ensure DC motor polarity and wiring are correct.
 * Author: Sujay Paul
 * Repository: https://github.com/sujaypaul27/Firefighting-Robot-AquaFlare
 * ======================================================================================
 */

const uint8_t PIN_MOTOR_L_IN1 = 2;
const uint8_t PIN_MOTOR_L_IN2 = 4;
const uint8_t PIN_MOTOR_L_ENA = 5;
const uint8_t PIN_MOTOR_R_ENB = 6;
const uint8_t PIN_MOTOR_R_IN3 = 7;
const uint8_t PIN_MOTOR_R_IN4 = 8;

const uint8_t TEST_SPEED = 180;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_MOTOR_L_IN1, OUTPUT);
  pinMode(PIN_MOTOR_L_IN2, OUTPUT);
  pinMode(PIN_MOTOR_L_ENA, OUTPUT);
  pinMode(PIN_MOTOR_R_ENB, OUTPUT);
  pinMode(PIN_MOTOR_R_IN3, OUTPUT);
  pinMode(PIN_MOTOR_R_IN4, OUTPUT);

  Serial.println(F("========================================================="));
  Serial.println(F("           AquaFlare: L298N Motor Test Suite             "));
  Serial.println(F("========================================================="));
  Serial.println(F("Elevate chassis so wheels spin freely during this test!"));
  delay(2000);
}

void loop() {
  Serial.println(F("--> Test 1: Moving FORWARD for 2 seconds"));
  analogWrite(PIN_MOTOR_L_ENA, TEST_SPEED);
  analogWrite(PIN_MOTOR_R_ENB, TEST_SPEED);
  digitalWrite(PIN_MOTOR_L_IN1, HIGH);
  digitalWrite(PIN_MOTOR_L_IN2, LOW);
  digitalWrite(PIN_MOTOR_R_IN3, HIGH);
  digitalWrite(PIN_MOTOR_R_IN4, LOW);
  delay(2000);

  Serial.println(F("--> Stop for 1 second"));
  stopAll();
  delay(1000);

  Serial.println(F("--> Test 2: Moving BACKWARD for 2 seconds"));
  analogWrite(PIN_MOTOR_L_ENA, TEST_SPEED);
  analogWrite(PIN_MOTOR_R_ENB, TEST_SPEED);
  digitalWrite(PIN_MOTOR_L_IN1, LOW);
  digitalWrite(PIN_MOTOR_L_IN2, HIGH);
  digitalWrite(PIN_MOTOR_R_IN3, LOW);
  digitalWrite(PIN_MOTOR_R_IN4, HIGH);
  delay(2000);

  Serial.println(F("--> Stop for 1 second"));
  stopAll();
  delay(1000);

  Serial.println(F("--> Test 3: Turning LEFT (Differential Pivot) for 1.5 seconds"));
  analogWrite(PIN_MOTOR_L_ENA, TEST_SPEED);
  analogWrite(PIN_MOTOR_R_ENB, TEST_SPEED);
  digitalWrite(PIN_MOTOR_L_IN1, LOW);
  digitalWrite(PIN_MOTOR_L_IN2, HIGH);
  digitalWrite(PIN_MOTOR_R_IN3, HIGH);
  digitalWrite(PIN_MOTOR_R_IN4, LOW);
  delay(1500);

  Serial.println(F("--> Stop for 1 second"));
  stopAll();
  delay(1000);

  Serial.println(F("--> Test 4: Turning RIGHT (Differential Pivot) for 1.5 seconds"));
  analogWrite(PIN_MOTOR_L_ENA, TEST_SPEED);
  analogWrite(PIN_MOTOR_R_ENB, TEST_SPEED);
  digitalWrite(PIN_MOTOR_L_IN1, HIGH);
  digitalWrite(PIN_MOTOR_L_IN2, LOW);
  digitalWrite(PIN_MOTOR_R_IN3, LOW);
  digitalWrite(PIN_MOTOR_R_IN4, HIGH);
  delay(1500);

  Serial.println(F("--> Stop for 2 seconds. Loop restarting..."));
  stopAll();
  delay(2000);
}

void stopAll() {
  analogWrite(PIN_MOTOR_L_ENA, 0);
  analogWrite(PIN_MOTOR_R_ENB, 0);
  digitalWrite(PIN_MOTOR_L_IN1, LOW);
  digitalWrite(PIN_MOTOR_L_IN2, LOW);
  digitalWrite(PIN_MOTOR_R_IN3, LOW);
  digitalWrite(PIN_MOTOR_R_IN4, LOW);
}
