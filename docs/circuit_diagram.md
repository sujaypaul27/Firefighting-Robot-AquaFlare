# 🔌 AquaFlare — Circuit Diagram & Electrical Interfacing

This document outlines the complete circuit layout, electrical specifications, and wiring pinout for the **AquaFlare Autonomous Firefighting Robot**.

---

## ⚡ Power Distribution Architecture

Robotics platforms with inductive loads (DC motors, water pumps, servo coils) require careful electrical isolation to prevent voltage sag, brownouts, and Arduino micro-resets.

```
       +-------------------------------------------------------+
       |                  POWER ARCHITECTURE                   |
       +-------------------------------------------------------+

       [9V - 12V Li-ion Battery Pack / Dual 18650s]
                          |
        +-----------------+------------------+
        |                                    |
        v                                    v
 [L298N 12V Power Terminal]          [5V Buck Regulator / Arduino Vin]
        |                                    |
        +---> DC Drive Motors (x4)          +---> Arduino Uno 5V Rail
        |                                    |      |---> Flame Sensors (x3)
 [9V Submersible Pump Power]                 |      |---> SG90 Servo (5V)
        |                                    |      +---> Relay Module VCC
 [5V Relay Module COM -> NO Switch]          |
        |                                    |
        +------------------------------------+
                          |
                   [COMMON GROUND (GND)]
```

> [!IMPORTANT]
> **Common Ground Rule:** All ground terminals (Arduino `GND`, L298N `GND`, Relay `GND`, and Battery Negative) **MUST** be interconnected to establish a unified 0V reference potential.

---

## 📌 Complete Pin Assignment Matrix

| Subsystem | Component Pin | Arduino Uno Pin | Pin Type | Functional Description |
|---|---|---|---|---|
| **Sensors** | Left Flame Sensor (DO) | `A0` | Digital Input | Left directional flame detection (Active LOW) |
| | Center Flame Sensor (DO) | `A1` | Digital Input | Frontal flame detection (Active LOW) |
| | Right Flame Sensor (DO) | `A2` | Digital Input | Right directional flame detection (Active LOW) |
| | Sensor VCC (x3) | `5V` | Power Rail | +5V Regulated supply |
| | Sensor GND (x3) | `GND` | Ground Rail | 0V Reference |
| **Motor Driver** | L298N IN1 | `D2` | Digital Output | Left Motor Forward direction |
| | L298N IN2 | `D4` | Digital Output | Left Motor Reverse direction |
| | L298N ENA | `D5` | PWM Output | Left Motor Speed (PWM 0-255) |
| | L298N ENB | `D6` | PWM Output | Right Motor Speed (PWM 0-255) |
| | L298N IN3 | `D7` | Digital Output | Right Motor Forward direction |
| | L298N IN4 | `D8` | Digital Output | Right Motor Reverse direction |
| | L298N GND | `GND` | Ground | Common Ground |
| **Actuators** | SG90 Servo (Signal) | `D9` | PWM / Pulse | 180° Nozzle sweeping pulse control |
| | SG90 Servo (VCC) | `5V` | Power Rail | +5V DC |
| | SG90 Servo (GND) | `GND` | Ground Rail | Common Ground |
| | 5V Relay IN | `D12` | Digital Output | Controls 9V Water Pump (Active LOW) |
| | 5V Relay VCC | `5V` | Power Rail | +5V DC |
| | 5V Relay GND | `GND` | Ground Rail | Common Ground |
| **Diagnostics**| Onboard LED | `D13` | Digital Output | Active tracking & suppression indicator |

---

## 🔄 Actuator Wiring Details

### 1. Water Pump & 5V Relay Module
Submersible DC water pumps create high inductive kickback (back-EMF) when turned on and off. The 5V optocoupled relay provides complete galvanic isolation for the Arduino:

```
[9V Pump Battery (+)] ─────> [Relay COM Terminal]
                             [Relay NO Terminal] ─────> [Water Pump (+)]
[9V Pump Battery (-)] ───────────────────────────────> [Water Pump (-)]
                                                         |
                                                         v
                                                   (Common GND)
```

- **Relay `VCC`**: Connect to Arduino `5V`
- **Relay `GND`**: Connect to Arduino `GND`
- **Relay `IN`**: Connect to Arduino Pin `12`
- **Switch Action**: When Pin `12` is pulled `LOW`, the relay closes the `COM-NO` circuit, powering the 9V pump.

---

### 2. L298N Dual H-Bridge Driver
Controls differential drive across four DC gearmotors (left motors paired, right motors paired):

```
       +---------------------------------------------+
       |             L298N MOTOR DRIVER              |
       |                                             |
OUT 1  |-- [Left Motors (+)]        [12V Battery (+)]|-- 12V In
OUT 2  |-- [Left Motors (-)]             [Common GND]|-- GND
OUT 3  |-- [Right Motors (+)]      [5V Regulated Out]|-- (Unused if external 5V)
OUT 4  |-- [Right Motors (-)]                        |
       |                                             |
       |  ENA   IN1   IN2   IN3   IN4   ENB          |
       +---|-----|-----|-----|-----|-----|-----------+
           |     |     |     |     |     |
          D5    D2    D4    D7    D8    D6   (Arduino Pins)
```

---

### 3. Flame Sensor Triplet
Each flame sensor module features an IR receiver photodiode tuned to the 760 nm – 1100 nm spectral band (the characteristic wavelength of hydrocarbon flame emissions).

- **Left Sensor**: Positioned angled $\sim 45^\circ$ toward the left.
- **Center Sensor**: Positioned aimed directly ahead ($0^\circ$).
- **Right Sensor**: Positioned angled $\sim 45^\circ$ toward the right.
- **Sensitivity Trimmer**: Adjust onboard 10k potentiometer until sensor status LED is off in normal lighting, but triggers when exposed to flame within 1 meter.
