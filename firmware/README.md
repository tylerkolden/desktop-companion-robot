# Firmware workspace

`Companion/Companion.ino` is the first bench firmware. It provides serial diagnostics, I2C scanning, animated eyes, motion sensing, single-servo calibration saved in flash, and a deliberately small, manually armed 10-second gesture. No walking gait or browser control is included yet.

**Read [First power-on](../docs/FIRST-POWER-ON.md) before connecting servo power.** Firmware is not hardware-tested. Physical fit and calibration remain required.

## Locked build environment

| Dependency | Version |
|---|---|
| Arduino CLI | 1.5.1 |
| Espressif ESP32 board package | 3.3.0 |
| Adafruit BusIO | 1.17.4 |
| Adafruit Unified Sensor | 1.1.15 |
| Adafruit GFX Library | 1.12.6 |
| Adafruit SSD1306 | 2.5.17 |
| Adafruit MPU6050 | 2.2.6 |
| Adafruit PWM Servo Driver Library | 3.0.2 |

Board identifier: `esp32:esp32:XIAO_ESP32C3:CDCOnBoot=default`. USB CDC must be enabled for the serial console. The serial baud setting is 115200 with newline termination.

## Build from the repository

See [beginner setup](../docs/SETUP.md). From the repository root:

```powershell
.\robot.ps1 setup
.\robot.ps1 build
.\robot.ps1 ports
```

Tools and libraries live in ignored `.tools/` inside this checkout. `toolchain.psd1` pins versions. The script maps a temporary short path for Windows compilers and cleans it up afterward. No `build.local.psd1`, global PATH setup, or external author tool installation is used. The old `scripts/build.ps1 -Install` entry point forwards to the new setup and build commands.

For upload and serial commands, use [the command reference](../docs/COMMANDS.md). These require an explicitly identified COM port and physical servo-power disconnection. Upload compiles fresh first. The project does not automatically upload during setup/build or select a port. Arduino IDE remains an optional manual path with the versions above.

For standalone beginner examples, use [the lessons](../lessons/README.md) before this full reference firmware.

## Source structure and checks

- `Companion.ino`: device initialization, bounded serial parser, stored calibration and timed motion.
- `Face.h`: original OLED expressions and blinking.
- `Logic.h`: shared pulse-limit, timer, tap and orientation logic.
- `LogicTests.h`: compile-time regression tests for double taps, held-high events, bounce, timeout, clock rollover, limit validation and orientation hysteresis. Every sketch build evaluates these assertions.

Sensor sampling and animation use timers rather than long movement delays. Device initialization and I2C transfers are still synchronous. A full OLED frame takes roughly 25ms at 400kHz, so the nominal 25ms sensor / 20ms motion schedules are not hard real-time guarantees; display frames are limited to about 8fps. Software stop requires a responsive controller and bus; it does not cut servo power. A saved calibration is an operator decision, not a sensor-derived proof of safe travel.

## Timed bench output test

`test N` (N=0..3) requires stopped motion, an available PCA and at least 10us of existing headroom above the saved center. It requests center for 500ms and center+10us for 500ms, then pulse-off. It never starts at boot or saves calibration. Use one unloaded servo through the [lesson 5B wiring and supply sequence](../lessons/README.md#5b-introduce-one-servo-and-its-power-supply). `stop` or `scan` cancels it; other motion commands are rejected while it runs. `status` reports `timed_test`. Timing is cooperative, not a hardware power cutoff. No automatic shaft-position feedback is available; observe and record physical movement. Compile-time assertions cover phase boundaries, overdue stopping and clock rollover.
