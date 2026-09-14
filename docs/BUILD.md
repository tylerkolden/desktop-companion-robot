# Staged build instructions

These instructions cover preparation and the planned sequence. **Firmware and printed parts are not released yet.** Do not interpret future stages as already tested or flash the upstream sketch unchanged: it drives servos directly, whereas this robot uses a PCA9685.

## 1. Receive and inspect

- Use the shopping-list receipt checks. Confirm four positional servos, horns and screws.
- Verify installed headers on the controller, OLED, MPU6050 and driver.
- Record actual dimensions in MEASUREMENTS.md, including connector clearance.
- Retain the existing battery separately; it is not used in V1.
- Confirm computer connection, USB data cable, a clear work surface and the A1 filament/nozzle.

## 2. Prepare the power connection

With the wall adapter unplugged, connect a short red/black 20-AWG pair between its supplied barrel-to-terminal adapter and PCA9685 servo-power terminal. Follow + and - markings. Strip only enough insulation to seat inside each terminal; keep all strands captured, with no exposed conductor bridging contacts. Gently pull each wire after tightening. Provide strain relief so the cord cannot pull on the PCB.

Before connecting the driver, use the multimeter to establish adapter voltage and polarity as described in WIRING.md. Disconnect power again before final wiring. Do not route servo current through breadboard contacts or Dupont signal jumpers.

## 3. Establish USB programming

Use Arduino IDE and the Espressif ESP32 board package, selecting **XIAO_ESP32C3**. Follow the [Seeed setup guide](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/) if the board is not listed or needs bootloader recovery. Record the exact package and IDE versions once a build works; no versions are certified by this repo yet.

Required future firmware dependencies: Adafruit SSD1306, Adafruit GFX, Adafruit MPU6050, Adafruit Unified Sensor, Adafruit BusIO, and Adafruit PWM Servo Driver. Install dependencies through Library Manager when developing the diagnostics. `ESP32Servo` is not required for PCA9685 control.

**Next implementation milestone:** add and compile an I2C scan sketch in `firmware/`. Until then there is no project sketch to upload.

## 4. Bring up low-current modules

Keep servo power off and all servos disconnected. Follow the logic wiring table. Test controller serial output, then add OLED, MPU6050 and PCA9685 one at a time. Record scanned addresses. Show a static face; read acceleration while turning the sensor slowly. Confirm SDA/SCL idle voltage is compatible with 3.3V logic. Stop and diagnose missing devices before proceeding.

## 5. Calibrate a single servo

Requires the future calibration sketch. Remove legs and horns, connect one servo to channel 0, and use 50Hz PWM with a nominal 1500us neutral command. Energize servo power only with stable controller logic and known commands. Remove power before attaching a horn. Increase travel in small steps while unloaded; stop at binding or persistent buzzing. Store center, direction and conservative limits per servo. Repeat for all four channels.

Never command every servo to extreme positions as a first test. Keep limbs clear during startup and leave an accessible means to unplug servo power.

## 6. Print fit samples

Requires future CAD. Print the OLED mount and one servo cradle first, at 100% scale. Starting profile: A1, actual nozzle size, suitable PLA preset, approximately 0.20mm layers if using a 0.4mm nozzle. These are starting settings, not validated production settings. Check screen opening, header clearance, servo ears, horn access, wire exits and screw engagement. Record failures and adjust CAD before the full shell.

## 7. Assemble and test V1

Requires accepted fit samples and a BOM with final screw lengths. Mount electronics without loading the OLED glass or bending headers. Place the IMU rigidly and record which axis points upward. Fit calibrated legs, retaining USB/terminal access and cord strain relief. Start with stand/sit and small gestures. Test on the floor or inside a bounded tray; V1 has no desk-edge sensor. Cord drag may prevent reliable walking, so expressive movement is the first goal.

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
