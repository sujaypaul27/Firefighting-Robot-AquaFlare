# 🛠️ AquaFlare — Hardware Assembly & Mechanical Setup

This guide provides step-by-step instructions for assembling the chassis, plumbing the fire extinguishing system, aligning sensors, and securing wiring.

---

## 📦 Bill of Materials (BOM)

| Component | Quantity | Specification / Notes |
|---|---|---|
| **Microcontroller** | 1 | Arduino Uno R3 (ATmega328P) |
| **Flame Sensors** | 3 | IR Flame Detection Sensor Modules (LM393 Comparator) |
| **Motor Driver** | 1 | L298N Dual H-Bridge Module (2A peak per channel) |
| **Chassis Kit** | 1 | 4WD / 2WD Transparent Acrylic Robot Chassis with mounting hardware |
| **DC Geared Motors** | 4 | Yellow TT Geared DC Motors (3-6V, 1:48 reduction ratio) |
| **Rubber Wheels** | 4 | 65mm diameter rubber tread wheels |
| **Servo Motor** | 1 | TowerPro SG90 9g Micro-Servo (with horn attachments) |
| **Water Pump** | 1 | 3V - 9V DC Submersible Mini Water Pump |
| **Relay Module** | 1 | 1-Channel 5V Relay Module with optocoupler isolation |
| **Water Reservoir** | 1 | Cylindrical cup/tank securely attached to chassis center |
| **Tubing** | 1 | Clear flexible silicone / vinyl hose ($\sim 6$mm outer diameter) |
| **Power Source** | 2 | 2x 18650 Li-ion batteries (7.4V - 8.4V) or 9V battery pack |
| **Wiring** | 1 Pack | Jumper wires (Male-to-Female, Male-to-Male), zip-ties |

---

## 🔧 Step-by-Step Mechanical Assembly

### 1. Chassis & Drive Motors Assembly
1. Mount the four TT gearmotors to the underside of the acrylic baseplate using the M3 screws and brackets.
2. Solder paired red/black wires to each motor:
   - Wire the front-left and rear-left motors in parallel to **OUT1** and **OUT2** of the L298N.
   - Wire the front-right and rear-right motors in parallel to **OUT3** and **OUT4** of the L298N.
3. Push the 65mm rubber wheels onto the motor output axles.

---

### 2. Sensor Placement & Angular Spacing
Sensor geometry is crucial for accurate fire triangulation:
- **Left Sensor:** Mount at the front-left bumper, tilted outwards at approximately **$40^\circ$ to $45^\circ$**.
- **Center Sensor:** Mount directly along the vehicle centerline, pointing **straight ahead ($0^\circ$)**.
- **Right Sensor:** Mount at the front-right bumper, tilted outwards at approximately **$40^\circ$ to $45^\circ$**.

> [!TIP]
> Ensure sensor photodiode windows are shielded from direct top light (ambient room lamps) so they detect ground-level and forward flames without false triggers.

---

### 3. Water Reservoir & Submersible Pump Installation
1. Fasten the reservoir cup to the rear section of the chassis using mounting brackets or hot-melt adhesive.
2. Drop the submersible mini pump into the bottom of the reservoir cup.
3. Attach one end of the clear vinyl tube to the pump outlet.
4. Route the tube forward along the chassis frame toward the front servo mount, ensuring sufficient slack so it does not pull when the servo oscillates.

---

### 4. Servo & Extinguishing Nozzle Horn Assembly
1. Mount the SG90 micro-servo firmly at the front centerline of the chassis.
2. Attach a servo horn arm and cable-tie the tip of the vinyl tube to the horn arm.
3. Power the servo to its **$90^\circ$ center position** before fastening the horn, ensuring it can travel freely from **$0^\circ$ to $180^\circ$** without striking chassis standoffs or pulling the water tube.

---

## ⚠️ Pre-Flight Safety Checks
- [ ] Confirm no water leaks near the Arduino or motor driver electronics.
- [ ] Verify that all grounds are common.
- [ ] Ensure the water pump is never operated dry for extended periods.
- [ ] Elevate chassis wheels off the surface before initial power-on to prevent unexpected movement.
