/**
 * ======================================================================================
 * Project: AquaFlare — Autonomous Firefighting Robot
 * Utility: Flame Sensor Calibration & Diagnostic Sketch
 * Purpose: Calibrate the potentiometers on Left (A0), Center (A1), and Right (A2)
 *          flame sensors by streaming real-time digital and analog readings.
 * Author: Sujay Paul
 * Repository: https://github.com/sujaypaul27/Firefighting-Robot-AquaFlare
 * ======================================================================================
 */

const uint8_t PIN_FLAME_LEFT   = A0;
const uint8_t PIN_FLAME_CENTER = A1;
const uint8_t PIN_FLAME_RIGHT  = A2;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_FLAME_LEFT, INPUT);
  pinMode(PIN_FLAME_CENTER, INPUT);
  pinMode(PIN_FLAME_RIGHT, INPUT);

  Serial.println(F("========================================================="));
  Serial.println(F("     AquaFlare: 3-Channel Flame Sensor Calibration      "));
  Serial.println(F("========================================================="));
  Serial.println(F("Instructions:"));
  Serial.println(F("1. Use a small screwdriver to adjust the blue trimmer on each module."));
  Serial.println(F("2. Ensure LED is OFF under ambient room lighting."));
  Serial.println(F("3. Introduce a candle or lighter at 30-50 cm; LED must illuminate (LOW)."));
  Serial.println(F("---------------------------------------------------------"));
  delay(1500);
}

void loop() {
  int dLeft   = digitalRead(PIN_FLAME_LEFT);
  int dCenter = digitalRead(PIN_FLAME_CENTER);
  int dRight  = digitalRead(PIN_FLAME_RIGHT);

  int aLeft   = analogRead(PIN_FLAME_LEFT);
  int aCenter = analogRead(PIN_FLAME_CENTER);
  int aRight  = analogRead(PIN_FLAME_RIGHT);

  Serial.print(F("[LEFT]   DO: "));
  Serial.print(dLeft == LOW ? F("FIRE! ") : F("CLEAR "));
  Serial.print(F(" (Analog: "));
  Serial.print(aLeft);
  Serial.print(F(") | "));

  Serial.print(F("[CENTER] DO: "));
  Serial.print(dCenter == LOW ? F("FIRE! ") : F("CLEAR "));
  Serial.print(F(" (Analog: "));
  Serial.print(aCenter);
  Serial.print(F(") | "));

  Serial.print(F("[RIGHT]  DO: "));
  Serial.print(dRight == LOW ? F("FIRE! ") : F("CLEAR "));
  Serial.print(F(" (Analog: "));
  Serial.print(aRight);
  Serial.println(F(")"));

  delay(300);
}
