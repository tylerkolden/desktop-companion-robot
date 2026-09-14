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

## Build on Windows

Install Arduino CLI 1.5.1 from its official release, then from the repository root run:

```powershell
.\scripts\build.ps1 -Install
```

`-Install` downloads the exact board package and libraries. Omit it on subsequent builds. The script accepts `-ArduinoCli` and `-ConfigFile` to use a separate local toolchain. Build binaries go into ignored `build/Companion/`.

```powershell
arduino-cli board list
# Replace COM7 with the detected XIAO port. Disconnect servo power before upload.
arduino-cli upload --fqbn esp32:esp32:XIAO_ESP32C3:CDCOnBoot=default --port COM7 --input-dir build/Companion firmware/Companion
arduino-cli monitor --port COM7 --config baudrate=115200
```

If using a custom CLI config, pass the same `--config-file` to every CLI command. On Windows keep the toolchain and checkout paths short: deeply nested paths can cause GCC to report missing `bits/c++config.h` even when the file exists. Arduino IDE users can install the listed versions manually and open the sketch normally.

## Source structure and checks

- `Companion.ino`: device initialization, bounded serial parser, stored calibration and timed motion.
- `Face.h`: original OLED expressions and blinking.
- `Logic.h`: shared pulse-limit, timer, tap and orientation logic.
- `LogicTests.h`: compile-time regression tests for double taps, held-high events, bounce, timeout, clock rollover, limit validation and orientation hysteresis. Every sketch build evaluates these assertions.

Sensor sampling and animation use timers rather than long movement delays. Device initialization and I2C transfers are still synchronous. A full OLED frame takes roughly 25ms at 400kHz, so the nominal 25ms sensor / 20ms motion schedules are not hard real-time guarantees; display frames are limited to about 8fps. Software stop requires a responsive controller and bus; it does not cut servo power. A saved calibration is an operator decision, not a sensor-derived proof of safe travel.
