# Robot workshop: one lesson, one program

Start here. Each lesson has its own page and standalone sketch. Test one function, record evidence, then build on it. **Companion is not part of this course.** It remains an optional experimental application; the old lessons are archived for reference.

| Lesson | Program to upload | Hardware | What you prove |
|---|---|---|---|
| [01. Meet the controller](plans/01-controller.md) | `Basics` | XIAO only | Explain the difference between building, uploading and running a program. |
| [02. Gravity detective](plans/02-accelerometer.md) | `Accelerometer` | XIAO + GY-521 | Identify acceleration along X, Y and Z, including gravity, in m/s². |
| [03. Turn detective](plans/03-gyroscope.md) | `Gyroscope` | Same GY-521 wiring | Distinguish rotation rate (degrees per second) from tilt angle. |
| [04. The chip thermometer](plans/04-temperature.md) | `Temperature` | Same GY-521 wiring | Read Celsius and distinguish chip temperature from room or body temperature. |
| [05. Pixels tell a story](plans/05-oled.md) | `Display` | XIAO + OLED only | Connect a successful I2C exchange to a visible change in screen pixels. |
| [06. Signals become movement](plans/06-servo.md) | `Servo` | XIAO + PCA9685 + one SG90 | Separate driver communication, motor power and observed shaft movement. |
| [07. The team test](plans/07-integration.md) | `Integration` | All tested modules + one SG90 | Combine individually tested functions while identifying which observation proves each one works. |

The MPU6050 contains three measuring functions: acceleration, rotation rate and chip temperature. Lessons 2–4 use the same physical board but different programs; do not buy three sensors.

Every page follows **mission → starting setup → diagram/pinout → prediction → upload → monitor commands → investigation → evidence → troubleshooting → handoff**. This uses explicit modeling, guided practice, retrieval questions and a check for understanding before the next lesson. Allow 20–30 minutes and repeat any uncertain observation.

## Two places to type

At `PS ...>` run `.\robot.ps1 ...` commands. After `Connecting to COM...`, type the short robot commands and press Enter. `help` redisplays instructions. `read` gives one result, `start` streams, `stop` ends activity. `test` runs a timed non-motor observation or prints motor preparation; the motor lessons require a separate `run`. **No automatic motion on boot.** Ctrl+C closes the monitor; it does not stop a motor by itself.

## Starting from an assembled robot

Unplug the motor supply and USB before changing wires. Follow lesson 1's bare-board setup, then each page's keep/remove/add instructions. Do not unplug modules while powered. A returning learner may select a standalone function directly using its stated setup. Keep motor power off before any upload, reset or monitor opening.

## Teacher and maintainer notes

- [Setup](../docs/SETUP.md), [command reference](../docs/COMMANDS.md), [experiment record](../templates/EXPERIMENT.md).
- The canonical page source is `scripts/build_lessons.py`; regenerate after `scripts/build_wiring.py`, then rebuild the book.
- Each sketch chooses devices at compile time. Shared command handling and bounded motor timing live in `lessons/lib/RobotLessons/src/`; a sensor sketch neither includes the PCA driver nor writes motor outputs.
- For Arduino IDE, install the repository's `lessons/lib/RobotLessons` as a custom library and the pinned dependencies from `toolchain.psd1`. The repository PowerShell workflow supplies the library automatically.
- [Legacy lessons](legacy.md) preserve historical experiments, not the recommended sequence. Hello/Scanner/Face/Motion/Companion remain buildable for compatibility.
- After lesson 7: [measure parts](../docs/MEASUREMENTS.md), [print fit gauges](../cad/README.md), then [plan improvements](../docs/PLAN.md).

## Sensor references

- [MPU6050 product specification](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet.pdf): on-chip temperature measures the die.
- [Adafruit Arduino example](https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro/arduino): acceleration m/s², gyro rad/s, temperature Celsius. Our gyro lesson converts rad/s to deg/s.
