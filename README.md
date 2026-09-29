# Desktop Companion Robot

Wiring reference: [full pinout atlas and verification limits](docs/wiring/README.md). Use the stage matching your lesson; do not connect every pictured module at once.

**Current learning path:** [seven standalone lessons](lessons/README.md), each with its own program and page. Test acceleration, rotation rate, chip temperature, OLED and servo control separately; use Integration for the team test.

A personal, solder-free desktop robot inspired by Nickson Kiprotich's TN-24: an OLED face, four expressive legs, and motion sensing. Build a corded V1 first, then develop a cordless V2.

**Status: owner reports all seven electronics lessons completed successfully. Printable fit gauges are available; physical print fit, full body/leg CAD and assembled robot behavior remain unverified.** This is an early build in progress, not a completed robot kit. Full body/leg CAD and walking behavior remain pending.

## Learn together

**New to robotics? Start with [the beginner guide](docs/START-HERE.md), then follow [the hands-on lessons](lessons/README.md).** No previous coding experience is assumed. Work with an adult, predict what will happen, test one thing, and record the result.

Read [Andrew and the Tiny Robot with Big Ideas](docs/book/README.md) alongside the experiments. The book's first edition describes an Arduino IDE path; the current repository also provides a complete Windows terminal path. Codex and Arduino IDE are optional interfaces, not prerequisites for the terminal lessons.

## Run from this repository

From PowerShell in this folder, on Windows x64:

```powershell
.\robot.ps1 setup
.\robot.ps1 build -Sketch Basics
.\robot.ps1 ports
```

Setup downloads pinned tools into this checkout's ignored `.tools/` folder. No author-specific paths or global Arduino CLI installation are required. These three commands do not upload or move hardware. If your board may already be programmed, [inspect it first](docs/COMMANDS.md#check-an-existing-program).

- [Setup and prerequisites](docs/SETUP.md)
- [Command reference](docs/COMMANDS.md)
- [Troubleshooting](docs/TROUBLESHOOTING.md)
- [Codex teaching workflow](docs/CODEX-WORKFLOW.md)
- [Experiment template](templates/EXPERIMENT.md) and [contribution process](CONTRIBUTING.md)
- [Reproduce the book and CAD](docs/REPRODUCE.md)

The repository is currently private; new users need GitHub access or an authorized copy. Tool binaries are downloaded, not committed. The final printable body and legs are still unfinished; this is a teaching project in development.

## Build something now

Finished the seven lessons? Start [Build checkpoint 1: fit a servo](docs/BUILD-CHECKPOINT-01.md). Print one small gauge before designing a leg.

- All additional V1 parts are reported in hand. Begin with checks rather than more electronics purchases.
- Start with [First power-on](docs/FIRST-POWER-ON.md): upload the bench firmware, check the OLED and motion sensor, then calibrate one unloaded servo.
- Print the [two fit gauges](cad/README.md) to check your hardware dimensions before printing a complete body.
- Record `status` / `scan` output and fit results so the next enclosure revision can match the actual parts.

## Start here

1. Review the [Amazon shopping list](docs/SHOPPING-LIST.md) before ordering.
2. Follow the [staged build instructions](docs/BUILD.md) and the new first-power-on guide. Full enclosure assembly remains a later stage.
3. Use the [wiring specification](docs/WIRING.md) and [measurement sheet](docs/MEASUREMENTS.md).
4. Track progress against the [V1/V2 plan](docs/PLAN.md) and record results in the [test log](docs/TEST-LOG.md).
5. See the [upstream study and sources](docs/UPSTREAM.md) for what exists in TN-24 and what we need to implement.

## Is the shopping list complete?

**Bench electronics and tools: selected, with receipt checks. Full physical robot: not yet.**

The remaining procurement decisions are filament, enclosure screws/nuts and retention hardware after CAD, and a USB power source if the robot must operate away from a computer. The selected adapter's picture shows a screw-terminal accessory, but its written package list is ambiguous; verify it arrives. Do not buy duplicate electronics already owned.

## Design requirements

| Area | V1 decision |
|---|---|
| Assembly | No user soldering; installed headers, plug-in cables, screw terminals |
| Controller | Pre-soldered Seeed XIAO ESP32C3 |
| Face | AITRIP 0.96-inch SSD1306, 128 x 64, I2C |
| Movement | Four SG90 positional servos through a PCA9685 board |
| Sensing | Pre-soldered GY-521 MPU6050 |
| Power | USB for controller; separate regulated 5V/4A supply for servos; shared ground |
| Printing | Bambu Lab A1; editable source CAD and STL exports planned |
| Purchasing | Amazon only |
| V2 | Cordless, with a newly qualified battery/charger/regulator system |

V1 has **two power leads during development**. A one-cord arrangement is not yet designed. The controller remains usable for later Wi-Fi features; camera or voice AI is outside V1.

```mermaid
flowchart LR
  USB[USB power and programming] --> MCU[XIAO ESP32C3]
  MCU -->|3.3V logic and I2C| OLED[OLED face]
  MCU -->|3.3V logic and I2C| IMU[MPU6050]
  MCU -->|3.3V logic and I2C| PWM[PCA9685]
  PSU[5V 4A wall supply] -->|V+ and GND| PWM
  PWM --> LEGS[Four SG90 servos]
```

All modules share ground. The servo supply does not feed the XIAO's 5V or 3V3 pins in this arrangement.

## Repository layout

- `docs/`: shopping, wiring, assembly, measurement, validation, and roadmap.
- `firmware/`: bench diagnostics, calibration, animated face and a small gesture demo.
- `cad/`: editable fit gauges and STL exports; full robot models are pending.
- `scripts/`: repeatable firmware build and STL validation.
- `lessons/`: standalone introductory sketches and guided experiments.
- `robot.ps1` and `toolchain.psd1`: one command entry point and pinned dependencies.
- `templates/`: experiment records; keep personal drafts in ignored `notes/`.

## Development approach

Work on short feature branches and preserve a tested baseline. Separate display, sensing, servo calibration, behavior, and configuration. Every physical revision needs a documented fit test; every firmware release needs recorded board/library versions and a reproducible build command. Tag a V1 release only after the acceptance checklist passes.

## Attribution and reuse

Inspired by [tech-nickk/TN24](https://github.com/tech-nickk/TN24) and the [TN-24 V2 article](https://www.electromaker.io/project/view/tn-24-v20-cute-desktop-companion-robot). This is an independent personal project, not an official TN-24 release. No upstream source or product imagery is copied into this repository. No project-wide reuse license has been selected; decide that before a public release. The article's license label and the repository's missing license file are recorded in the upstream study.
