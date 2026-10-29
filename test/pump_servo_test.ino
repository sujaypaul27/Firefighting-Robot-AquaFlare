/**
 * ======================================================================================
 * Project: AquaFlare — Autonomous Firefighting Robot
 * Utility: Water Pump Relay & 180-Degree Servo Sweep Diagnostic
 * Purpose: Tests relay switching for the 9V submersible water pump and verifies
 *          full 180-degree smooth oscillation of the SG90 nozzle servo.
 * Author: Sujay Paul
 * Repository: https://github.com/sujaypaul27/Firefighting-Robot-AquaFlare
 * ======================================================================================
 */

#include <Servo.h>

const uint8_t PIN_SERVO_NOZZLE = 9;
const uint8_t PIN_PUMP_RELAY   = 12;

const uint8_t RELAY_ACTIVE     = LOW;
const uint8_t RELAY_INACTIVE   = HIGH;

Servo nozzleServo;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_PUMP_RELAY, OUTPUT);
  digitalWrite(PIN_PUMP_RELAY, RELAY_INACTIVE);

  nozzleServo.attach(PIN_SERVO_NOZZLE);
  nozzleServo.write(90); // Center

  Serial.println(F("========================================================="));
  Serial.println(F("     AquaFlare: Pump Relay & Servo Sweep Test Suite      "));
  Serial.println(F("========================================================="));
  Serial.println(F("WARNING: Ensure water reservoir has sufficient liquid"));
  Serial.println(F("         and nozzle is directed safely into a container!"));
  delay(3000);
}

void loop() {
  Serial.println(F("[1] Activating Pump Relay for 2 seconds (Static nozzle at 90 deg)..."));
  digitalWrite(PIN_PUMP_RELAY, RELAY_ACTIVE);
  delay(2000);

  Serial.println(F("[2] Initiating 180-Degree Sweep while Pump is ACTIVE..."));
  // 0 to 180 degrees sweep
  for (int pos = 0; pos <= 180; pos += 2) {
    nozzleServo.write(pos);
    delay(15);
  }
  delay(100);

  // 180 to 0 degrees return sweep
  for (int pos = 180; pos >= 0; pos -= 2) {
    nozzleServo.write(pos);
    delay(15);
  }
  delay(100);

  Serial.println(F("[3] Turning OFF Pump Relay and centering servo..."));
  digitalWrite(PIN_PUMP_RELAY, RELAY_INACTIVE);
  nozzleServo.write(90);

  Serial.println(F("[4] Inactive cooldown for 5 seconds. Repeating..."));
  delay(5000);
}
