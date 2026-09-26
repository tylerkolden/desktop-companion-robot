# Start here: your first robot

Wiring reference: [full pinout atlas and verification limits](wiring/README.md). Use the stage matching your lesson; do not connect every pictured module at once.

You do not need prior coding or electronics experience. Build with an adult, ask questions, and change one thing at a time. This is a developing training project, not a fully validated robot kit. Begin with a tiny working program and work toward an expressive desktop companion.

## What you need today

- A Windows x64 computer, internet for setup, and a few GB of available disk space (the ESP32 tools are large).
- This repository downloaded or cloned; [setup instructions](SETUP.md) explain both.
- A XIAO ESP32C3 and a USB data cable for the first hardware lesson.
- An adult for electrical connections, moving parts, and printer operation.

You can complete setup and compile every sketch without connecting hardware. Arduino IDE, VS Code, and Codex are optional interfaces. The repository's PowerShell commands perform setup, compilation, uploads, and serial diagnostics.

## Where to begin

1. Follow [computer setup](SETUP.md).
2. If the board may already be programmed, follow [identify and inspect](COMMANDS.md#check-an-existing-program) before uploading anything.
3. Start [the lessons](../lessons/README.md). Each includes a prediction, action, expected result, troubleshooting, and a record to keep.
4. Read [Andrew's book](book/README.md) alongside the experiments.
5. Use [wiring](WIRING.md), [first power-on](FIRST-POWER-ON.md), and [printing](../cad/README.md) when the lesson reaches those stages.

## What success means

| Stage | Evidence you collect |
|---|---|
| Controller | Repeating Hello message from the identified board |
| Communication | Actual I2C addresses, with modules added one at a time |
| Display | Two visibly different expressions |
| Sensor | Readings change predictably with orientation |
| One servo | Small unloaded motion and documented power-off procedure |
| Mechanics | Measured parts and printed gauge-fit observations |
| Integration | Repeated checks with saved calibration and supported chassis |

The current course uses Basics, Accelerometer, Gyroscope, Temperature, Display, Servo and Integration. Each has its own lesson page and source file. All use the same monitor commands. The first five never initialize motor outputs. Servo and Integration require explicit preparation and a run command, with motor power off before uploads or monitor opening. Companion and the older sketches are historical options, not course prerequisites.

## Keep the record honest

Use [the experiment template](../templates/EXPERIMENT.md). Put personal notes in `notes/`; only deliberately selected records should be committed. A clean compilation does not prove hardware works. A responding I2C address does not prove correct sensor orientation. A valid STL does not prove a part fits.

Known unfinished work: physical validation, final enclosure/legs/fasteners, walking, Wi-Fi controls, and cordless power. See [the plan](PLAN.md). Batteries stay out of V1.
