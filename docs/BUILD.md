# Staged build instructions

Wiring reference: [full pinout atlas and verification limits](wiring/README.md). Use the stage matching your lesson; do not connect every pictured module at once.

**Legacy Companion reference.** New learners should use [the current standalone course](../lessons/README.md). Commands in this document describe the older application, not the new Servo/Integration sketches.

These instructions cover preparation and the planned sequence. **Bench firmware and fit gauges are included; full robot CAD is not.** New learners start with [START-HERE.md](START-HERE.md) and the standalone lessons before [FIRST-POWER-ON.md](FIRST-POWER-ON.md). No successful physical tests have been recorded. Do not flash the upstream sketch unchanged: it drives servos directly, whereas this robot uses a PCA9685.

## 1. Receive and inspect

- Use the shopping-list receipt checks. Confirm four positional servos, horns and screws.
- Verify installed headers on the controller, OLED, MPU6050 and driver.
- Record actual dimensions in MEASUREMENTS.md, including connector clearance.
- Retain the existing battery separately; it is not used in V1.
- Confirm computer connection, USB data cable, a clear work surface and the A1 filament/nozzle.

## 2. Prepare the power connection

<!-- wiring:start -->
**Parts for this power checkpoint:** a regulated 5V wall supply, its low-voltage connector and prepared wires, and a multimeter. For the motor connection, also identify the SG90 servo and PCA9685 driver. The supply powers the motor through the driver; the meter checks its output before connection. Identify these parts before following the diagram.

### Wiring visuals and pinouts

**Before assembling these connections: close the monitor and unplug the XIAO USB cable. Keep the newly introduced 5V supply unplugged from the wall until the voltage-check step. On a repeat session, unplug it before changing wires.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![SERVO wiring: labelled endpoints and separate power paths](wiring/images/servo.png)

**Identify the green two-screw block on the PCA9685:** its + / V+ terminal accepts the regulated 5V supply positive wire; its - / GND terminal accepts the negative wire. They are not both V+. This block is not VCC, and it is different from the separate barrel-to-terminal connector on the supply cable. Confirm the actual board markings with power unplugged; the illustration does not establish left/right orientation. Functional servo pinout: ground, power, signal. Read the driver row labels before inserting the plug. **Keep all four XIAO logic wires connected**, as shown in this complete diagram: 3V3/VCC, GND/GND, D4/SDA and D5/SCL. The shared-ground wire is one of these four wires, not an extra fifth wire. Channel 0 is the three-pin output group beside the printed 0 among groups 0 through 15. One plug spans GND, V+ and signal within that group, not three neighboring channel numbers. The green block is not channel 0. Verify actual markings; left/right orientation is not established by this diagram. Use only channel 0 for this experiment. USB and adapter grounds meet at PCA GND; adapter positive never meets XIAO 5V or 3V3. Grounded OE is not a reset interlock: unplug servo power for uploads, resets and horn fitting.

| From / pin | To / pin | Meaning |
|---|---|---|
| Adapter positive | PCA green block: + / V+ | Regulated 5V; direct power wire |
| Adapter negative | PCA green block: - / GND | Direct power wire |
| XIAO 3V3 | PCA VCC | Logic power, separate from V+ |
| XIAO GND | PCA GND | Common reference |
| XIAO D4 / GPIO6 | PCA SDA | I2C data |
| XIAO D5 / GPIO7 | PCA SCL | I2C clock |
| PCA channel 0 GND | SG90 ground | Usually brown/black; verify |
| PCA channel 0 V+ | SG90 power | Usually red; verify |
| PCA channel 0 PWM | SG90 signal | Usually orange/yellow; verify |

![METER wiring: labelled endpoints and separate power paths](wiring/images/meter.png)

Probe-jack pinout is functional, not the physical socket position of every meter variant. Select DC voltage before probing. Verify barrel size and polarity against the actual adapter; do not assume center-positive from a generic picture.

| From / pin | To / pin | Meaning |
|---|---|---|
| Black probe / COM jack | Adapter negative output | Voltage check only |
| Red probe / V-ohm jack | Adapter positive output | DC volts; never current mode |

<!-- wiring:end -->

With the wall adapter unplugged, connect a short red/black 20-AWG pair between its supplied barrel-to-terminal adapter and PCA9685 servo-power terminal. Follow + and - markings. Strip only enough insulation to seat inside each terminal; keep all strands captured, with no exposed conductor bridging contacts. Gently pull each wire after tightening. Provide strain relief so the cord cannot pull on the PCB.

Before connecting the driver, use the multimeter to establish adapter voltage and polarity as described in WIRING.md. Disconnect power again before final wiring. Do not route servo current through breadboard contacts or Dupont signal jumpers.

## 3. Establish USB programming

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![USB wiring: labelled endpoints and separate power paths](wiring/images/usb.png)

The pinout for this stage is USB-C socket only: **all XIAO header pins are unused**.

| From / pin | To / pin | Meaning |
|---|---|---|
| Computer USB port | XIAO USB-C socket | Data cable; all headers empty |

<!-- wiring:end -->

Use the repository's [setup](SETUP.md) and [commands](COMMANDS.md). `robot.ps1` installs the pinned ESP32 package and builds for XIAO_ESP32C3 with USB CDC enabled. Arduino IDE with board package **3.3.0** is an optional alternative. Follow the [Seeed setup guide](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) for board-specific recovery, checking the actual model. Exact dependency versions are in [firmware/README.md](../firmware/README.md); physical hardware validation remains pending.

Firmware dependencies: Adafruit SSD1306, Adafruit GFX, Adafruit MPU6050, Adafruit Unified Sensor, Adafruit BusIO, and Adafruit PWM Servo Driver. Use the pinned versions in the firmware guide. `ESP32Servo` is not required for PCA9685 control.

Open `firmware/Companion/Companion.ino`. Its `scan` and `status` commands cover the first hardware checks.

## 4. Bring up low-current modules

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![OLED wiring: labelled endpoints and separate power paths](wiring/images/oled.png)

Functional pinout: VCC, GND, SDA, SCL. The supplied display photo has **GND / VCC / SCL / SDA** across its top header when viewed from the front. Our illustration groups wires by function, not that physical order.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | AITRIP SSD1306 OLED VCC | 3.3V logic |
| XIAO GND | AITRIP SSD1306 OLED GND | Shared ground |
| XIAO D4 / GPIO6 | AITRIP SSD1306 OLED SDA | I2C data |
| XIAO D5 / GPIO7 | AITRIP SSD1306 OLED SCL | I2C clock |

![IMU wiring: labelled endpoints and separate power paths](wiring/images/imu.png)

Functional pinout: VCC, GND, SCL, SDA, XDA, XCL, AD0, INT. Leave XDA/XCL/INT open; verify AD0 default through scanning. Exact delivered header order is unconfirmed. Check regulator/pull-ups at 3.3V; do not substitute 5V to fix detection.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | GY-521 / MPU6050 VCC | 3.3V logic |
| XIAO GND | GY-521 / MPU6050 GND | Shared ground |
| XIAO D4 / GPIO6 | GY-521 / MPU6050 SDA | I2C data |
| XIAO D5 / GPIO7 | GY-521 / MPU6050 SCL | I2C clock |

![PCA wiring: labelled endpoints and separate power paths](wiring/images/pca.png)

The green two-screw block is a separate power input: one terminal is + / V+, the other is - / GND. It stays empty during this communication-only test; the driver electronics receive power through VCC. Follow the board markings, not a guessed left/right position. Functional pinout: VCC = logic 3.3V; V+ = separate output-power input; GND = common ground; SDA/SCL = I2C; OE = active-low output enable. Exact clone header order is unconfirmed. Verify OE pull-down; ground OE only if floating. Use only VCC, GND, SDA and SCL for this communication test. Leave V+ and the numbered output connectors empty.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | Teyleten PCA9685 VCC | 3.3V logic |
| XIAO GND | Teyleten PCA9685 GND | Shared ground |
| XIAO D4 / GPIO6 | Teyleten PCA9685 SDA | I2C data |
| XIAO D5 / GPIO7 | Teyleten PCA9685 SCL | I2C clock |

<!-- wiring:end -->

Keep servo power off and all servos disconnected. Follow the logic wiring table. Test controller serial output, then add OLED, MPU6050 and PCA9685 one at a time. Record scanned addresses. Show a static face; read acceleration while turning the sensor slowly. Confirm SDA/SCL idle voltage is compatible with 3.3V logic. Stop and diagnose missing devices before proceeding.

## 5. Calibrate a single servo

<!-- wiring:start -->
**Parts for this power checkpoint:** a regulated 5V wall supply, its low-voltage connector and prepared wires, and a multimeter. For the motor connection, also identify the SG90 servo and PCA9685 driver. The supply powers the motor through the driver; the meter checks its output before connection. Identify these parts before following the diagram.

### Wiring visuals and pinouts

**Before assembling these connections: close the monitor and unplug the XIAO USB cable. Keep the newly introduced 5V supply unplugged from the wall until the voltage-check step. On a repeat session, unplug it before changing wires.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![SERVO wiring: labelled endpoints and separate power paths](wiring/images/servo.png)

**Identify the green two-screw block on the PCA9685:** its + / V+ terminal accepts the regulated 5V supply positive wire; its - / GND terminal accepts the negative wire. They are not both V+. This block is not VCC, and it is different from the separate barrel-to-terminal connector on the supply cable. Confirm the actual board markings with power unplugged; the illustration does not establish left/right orientation. Functional servo pinout: ground, power, signal. Read the driver row labels before inserting the plug. **Keep all four XIAO logic wires connected**, as shown in this complete diagram: 3V3/VCC, GND/GND, D4/SDA and D5/SCL. The shared-ground wire is one of these four wires, not an extra fifth wire. Channel 0 is the three-pin output group beside the printed 0 among groups 0 through 15. One plug spans GND, V+ and signal within that group, not three neighboring channel numbers. The green block is not channel 0. Verify actual markings; left/right orientation is not established by this diagram. Use only channel 0 for this experiment. USB and adapter grounds meet at PCA GND; adapter positive never meets XIAO 5V or 3V3. Grounded OE is not a reset interlock: unplug servo power for uploads, resets and horn fitting.

| From / pin | To / pin | Meaning |
|---|---|---|
| Adapter positive | PCA green block: + / V+ | Regulated 5V; direct power wire |
| Adapter negative | PCA green block: - / GND | Direct power wire |
| XIAO 3V3 | PCA VCC | Logic power, separate from V+ |
| XIAO GND | PCA GND | Common reference |
| XIAO D4 / GPIO6 | PCA SDA | I2C data |
| XIAO D5 / GPIO7 | PCA SCL | I2C clock |
| PCA channel 0 GND | SG90 ground | Usually brown/black; verify |
| PCA channel 0 V+ | SG90 power | Usually red; verify |
| PCA channel 0 PWM | SG90 signal | Usually orange/yellow; verify |

<!-- wiring:end -->

Use the bench firmware's `cal`, `jog`, `stop` and `save` commands documented in FIRST-POWER-ON.md. Remove legs and horns and start with one servo on channel 0. Energize servo power only with stable controller logic and known commands. Remove power before attaching a horn. Increase travel in small steps while unloaded; stop at binding or persistent buzzing. Store center, direction and conservative limits per servo. Repeat for all four channels.

Never command every servo to extreme positions as a first test. Keep limbs clear during startup and leave an accessible means to unplug servo power.

## 6. Print fit samples

Print the included [fit gauges](../cad/README.md) first, at 100% scale. Starting profile: A1, actual nozzle size, suitable PLA preset, approximately 0.20mm layers if using a 0.4mm nozzle. These are starting settings, not validated production settings. Check the PCB/body outlines and record fit. Structural mounts, screen opening and screw engagement are checked in the next CAD revision before the full shell.

## 7. Assemble and test V1

This future stage requires accepted structural parts and a BOM with final screw lengths. Mount electronics without loading the OLED glass or bending headers. Place the IMU rigidly and record which axis points upward. Fit calibrated legs, retaining USB/terminal access and cord strain relief. The current firmware offers only the limited supported gesture; stand/sit and walking require further development and tests. Test on the floor or inside a bounded tray; V1 has no desk-edge sensor. Cord drag may prevent reliable walking, so expressive movement is the first goal.

Run the acceptance checklist in PLAN.md. Record observations, voltage during movement and any resets in TEST-LOG.md. A basic multimeter may miss fast voltage dips; do not treat a steady display as proof that transients are absent.

## Troubleshooting

| Symptom | First checks |
|---|---|
| No USB port | Data-capable cable, direct host port, board/boot procedure |
| Blank OLED | 3.3V/GND, SDA/SCL labels, actual I2C address, correct SSD1306 geometry |
| MPU missing | Pin order, address, module supply behavior and soldered header quality |
| Servos stationary | Separate V+ supply, common ground, OE, channel and plug direction |
| Resets during movement | Power distribution, loose terminals, binding, supply droop |
| Servo buzzing or heating | Cut servo power; reduce travel and inspect horn alignment |
| Robot tips or moves backwards | Per-leg inversion, center calibration and mass distribution |

Do not change hardware and firmware simultaneously during diagnosis; record each change.
