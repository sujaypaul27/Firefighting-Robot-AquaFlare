/**
 * ======================================================================================
 * Project: AquaFlare — Autonomous Firefighting Robot
 * Description: Production-grade firmware for Arduino Uno based Firefighting Robot.
 *              Navigates autonomously using 3 directional IR flame sensors,
 *              approaches detected fire, and suppresses it using a 5V relay-driven 9V
 *              submersible pump and a 180-degree oscillating servo nozzle.
 * Author: Sujay Paul
 * Repository: https://github.com/sujaypaul27/Firefighting-Robot-AquaFlare
 * License: MIT
 * ======================================================================================
 */

#include <Servo.h>

// ======================================================================================
// PIN DEFINITIONS & HARDWARE MAPPING
// ======================================================================================

// Flame Sensors (IR Receiver Modules with LM393 Comparators - Active LOW)
const uint8_t PIN_FLAME_LEFT   = A0;  // Left directional flame sensor DO
const uint8_t PIN_FLAME_CENTER = A1;  // Center directional flame sensor DO
const uint8_t PIN_FLAME_RIGHT  = A2;  // Right directional flame sensor DO

// L298N Dual H-Bridge Motor Driver
const uint8_t PIN_MOTOR_L_IN1  = 2;   // Left Motor Direction 1
const uint8_t PIN_MOTOR_L_IN2  = 4;   // Left Motor Direction 2
const uint8_t PIN_MOTOR_L_ENA  = 5;   // Left Motor PWM Speed (remove jumper to use)
const uint8_t PIN_MOTOR_R_ENB  = 6;   // Right Motor PWM Speed (remove jumper to use)
const uint8_t PIN_MOTOR_R_IN3  = 7;   // Right Motor Direction 1
const uint8_t PIN_MOTOR_R_IN4  = 8;   // Right Motor Direction 2

// Actuators
const uint8_t PIN_SERVO_NOZZLE = 9;   // SG90 Micro-Servo PWM control pin
const uint8_t PIN_PUMP_RELAY   = 12;  // 5V Relay Module controlling 9V water pump
const uint8_t PIN_LED_STATUS   = 13;  // Onboard status & telemetry indicator LED

// ======================================================================================
// LOGIC CONFIGURATION & THRESHOLDS
// ======================================================================================

// Sensor & Relay Polarities
const uint8_t FLAME_DETECTED   = LOW;   // Typical flame modules output LOW on fire detection
const uint8_t FLAME_CLEAR      = HIGH;
const uint8_t RELAY_ACTIVE     = LOW;   // Standard optocoupled 5V relay boards are Active-LOW
const uint8_t RELAY_INACTIVE   = HIGH;

// Motor Speed Constants (0 - 255 PWM)
const uint8_t SPEED_CRUISE     = 190;   // Forward movement speed
const uint8_t SPEED_TURN       = 175;   // Steering and turning speed

// Servo Calibration Angles (Degrees)
const int SERVO_MIN_ANGLE      = 0;     // Full left nozzle deflection
const int SERVO_CENTER_ANGLE   = 90;    // Center neutral aim
const int SERVO_MAX_ANGLE      = 180;   // Full right nozzle deflection
const int SERVO_STEP_DELAY_MS  = 15;    // Delay between angle increments for smooth sweep

// Extinguishing Configuration
const int SPRAY_CYCLES         = 2;     // Number of back-and-forth 180-degree sweep passes
const unsigned long EXTINGUISH_COOLDOWN_MS = 1500; // Delay after extinguishing to re-check

// ======================================================================================
// GLOBAL INSTANCES & STATE
// ======================================================================================
Servo nozzleServo;

// Sensor reading cache
bool fireLeft   = false;
bool fireCenter = false;
bool fireRight  = false;

// ======================================================================================
// FUNCTION DECLARATIONS
// ======================================================================================
void readSensors();
void stopMotors();
void moveForward(uint8_t speedVal = SPEED_CRUISE);
void moveBackward(uint8_t speedVal = SPEED_CRUISE);
void turnLeft(uint8_t speedVal = SPEED_TURN);
void turnRight(uint8_t speedVal = SPEED_TURN);
void extinguishFire();
void sweepNozzle180();

// ======================================================================================
// SETUP
// ======================================================================================
void setup() {
  // Initialize Serial Monitor for telemetry and debugging
  Serial.begin(9600);
  Serial.println(F("=================================================="));
  Serial.println(F("       AquaFlare — Autonomous Firefighting Robot "));
  Serial.println(F("                 System Initializing...           "));
  Serial.println(F("=================================================="));

  // Configure Flame Sensor Pins (with pullups for noise immunity)
  pinMode(PIN_FLAME_LEFT, INPUT);
  pinMode(PIN_FLAME_CENTER, INPUT);
  pinMode(PIN_FLAME_RIGHT, INPUT);

  // Configure L298N Motor Driver Pins
  pinMode(PIN_MOTOR_L_IN1, OUTPUT);
  pinMode(PIN_MOTOR_L_IN2, OUTPUT);
  pinMode(PIN_MOTOR_L_ENA, OUTPUT);
  pinMode(PIN_MOTOR_R_ENB, OUTPUT);
  pinMode(PIN_MOTOR_R_IN3, OUTPUT);
  pinMode(PIN_MOTOR_R_IN4, OUTPUT);

  // Configure Relay and Status LED Pins
  pinMode(PIN_PUMP_RELAY, OUTPUT);
  digitalWrite(PIN_PUMP_RELAY, RELAY_INACTIVE); // Ensure pump is initially OFF
  pinMode(PIN_LED_STATUS, OUTPUT);
  digitalWrite(PIN_LED_STATUS, LOW);

  // Halt motors initially
  stopMotors();

  // Attach and align Servo Motor
  nozzleServo.attach(PIN_SERVO_NOZZLE);
  nozzleServo.write(SERVO_CENTER_ANGLE);
  delay(500);

  Serial.println(F("[SYSTEM READY] Autonomous patrol mode engaged."));
}

// ======================================================================================
// MAIN CONTROL LOOP
// ======================================================================================
void loop() {
  readSensors();

  // State 1: Fire detected directly in FRONT (Center sensor triggered)
  // Action: Stop immediately and initiate high-pressure extinguishing protocol
  if (fireCenter) {
    Serial.println(F("[EVENT] FIRE DETECTED AHEAD! Initiating suppression..."));
    digitalWrite(PIN_LED_STATUS, HIGH);
    stopMotors();
    extinguishFire();
    digitalWrite(PIN_LED_STATUS, LOW);
  }
  // State 2: Fire detected on the LEFT
  // Action: Pivot Left toward the heat signature
  else if (fireLeft && !fireRight) {
    Serial.println(F("[TRACKING] Fire signature on LEFT -> Turning Left"));
    digitalWrite(PIN_LED_STATUS, HIGH);
    turnLeft();
    delay(150); // Short pulse turn for precise alignment
    stopMotors();
  }
  // State 3: Fire detected on the RIGHT
  // Action: Pivot Right toward the heat signature
  else if (fireRight && !fireLeft) {
    Serial.println(F("[TRACKING] Fire signature on RIGHT -> Turning Right"));
    digitalWrite(PIN_LED_STATUS, HIGH);
    turnRight();
    delay(150); // Short pulse turn for precise alignment
    stopMotors();
  }
  // State 4: Fire detected by multiple side sensors or approaching from broad angle
  else if (fireLeft && fireRight) {
    Serial.println(F("[TRACKING] Multi-sensor trigger -> Advancing carefully"));
    digitalWrite(PIN_LED_STATUS, HIGH);
    moveForward(SPEED_TURN);
    delay(100);
    stopMotors();
  }
  // State 5: All clear - Standby / Monitoring
  else {
    digitalWrite(PIN_LED_STATUS, LOW);
    stopMotors();
  }

  delay(20); // Loop cadence
}

// ======================================================================================
// SENSOR ACQUISITION
// ======================================================================================
void readSensors() {
  fireLeft   = (digitalRead(PIN_FLAME_LEFT) == FLAME_DETECTED);
  fireCenter = (digitalRead(PIN_FLAME_CENTER) == FLAME_DETECTED);
  fireRight  = (digitalRead(PIN_FLAME_RIGHT) == FLAME_DETECTED);
}

// ======================================================================================
// MOTOR CONTROL PRIMITIVES (L298N DUAL H-BRIDGE)
// ======================================================================================
void stopMotors() {
  analogWrite(PIN_MOTOR_L_ENA, 0);
  analogWrite(PIN_MOTOR_R_ENB, 0);
  digitalWrite(PIN_MOTOR_L_IN1, LOW);
  digitalWrite(PIN_MOTOR_L_IN2, LOW);
  digitalWrite(PIN_MOTOR_R_IN3, LOW);
  digitalWrite(PIN_MOTOR_R_IN4, LOW);
}

void moveForward(uint8_t speedVal) {
  analogWrite(PIN_MOTOR_L_ENA, speedVal);
  analogWrite(PIN_MOTOR_R_ENB, speedVal);
  digitalWrite(PIN_MOTOR_L_IN1, HIGH);
  digitalWrite(PIN_MOTOR_L_IN2, LOW);
  digitalWrite(PIN_MOTOR_R_IN3, HIGH);
  digitalWrite(PIN_MOTOR_R_IN4, LOW);
}

void moveBackward(uint8_t speedVal) {
  analogWrite(PIN_MOTOR_L_ENA, speedVal);
  analogWrite(PIN_MOTOR_R_ENB, speedVal);
  digitalWrite(PIN_MOTOR_L_IN1, LOW);
  digitalWrite(PIN_MOTOR_L_IN2, HIGH);
  digitalWrite(PIN_MOTOR_R_IN3, LOW);
  digitalWrite(PIN_MOTOR_R_IN4, HIGH);
}

void turnLeft(uint8_t speedVal) {
  analogWrite(PIN_MOTOR_L_ENA, speedVal);
  analogWrite(PIN_MOTOR_R_ENB, speedVal);
  // Differential drive: Left wheel backward, Right wheel forward
  digitalWrite(PIN_MOTOR_L_IN1, LOW);
  digitalWrite(PIN_MOTOR_L_IN2, HIGH);
  digitalWrite(PIN_MOTOR_R_IN3, HIGH);
  digitalWrite(PIN_MOTOR_R_IN4, LOW);
}

void turnRight(uint8_t speedVal) {
  analogWrite(PIN_MOTOR_L_ENA, speedVal);
  analogWrite(PIN_MOTOR_R_ENB, speedVal);
  // Differential drive: Left wheel forward, Right wheel backward
  digitalWrite(PIN_MOTOR_L_IN1, HIGH);
  digitalWrite(PIN_MOTOR_L_IN2, LOW);
  digitalWrite(PIN_MOTOR_R_IN3, LOW);
  digitalWrite(PIN_MOTOR_R_IN4, HIGH);
}

// ======================================================================================
// FIRE SUPPRESSION & 180-DEGREE SPRAY MECHANISM
// ======================================================================================
void extinguishFire() {
  // Ensure robot is stationary
  stopMotors();
  delay(200);

  Serial.println(F("[PUMP] Activating Water Pump Relay..."));
  digitalWrite(PIN_PUMP_RELAY, RELAY_ACTIVE); // Turn on 9V water pump

  // Execute 180-degree spray coverage sweeps
  for (int cycle = 1; cycle <= SPRAY_CYCLES; cycle++) {
    Serial.print(F("[SPRAY] Sweeping pass "));
    Serial.print(cycle);
    Serial.print(F(" of "));
    Serial.println(SPRAY_CYCLES);

    sweepNozzle180();
  }

  // Deactivate water pump
  digitalWrite(PIN_PUMP_RELAY, RELAY_INACTIVE);
  Serial.println(F("[PUMP] Water pump turned OFF. Returning nozzle to center."));

  // Return nozzle to default forward-facing angle
  nozzleServo.write(SERVO_CENTER_ANGLE);
  delay(EXTINGUISH_COOLDOWN_MS);

  Serial.println(F("[CYCLE COMPLETE] Resuming scan for flare signatures."));
}

/**
 * Sweeps the nozzle servo smoothly across the full 180-degree arc:
 * 0 deg -> 180 deg -> 0 deg
 */
void sweepNozzle180() {
  // Sweep from 0 to 180 degrees
  for (int pos = SERVO_MIN_ANGLE; pos <= SERVO_MAX_ANGLE; pos += 2) {
    nozzleServo.write(pos);
    delay(SERVO_STEP_DELAY_MS);
  }

  delay(100);

  // Sweep back from 180 to 0 degrees
  for (int pos = SERVO_MAX_ANGLE; pos >= SERVO_MIN_ANGLE; pos -= 2) {
    nozzleServo.write(pos);
    delay(SERVO_STEP_DELAY_MS);
  }
}
