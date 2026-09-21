# Learn robotics one experiment at a time

Use the [setup guide](../docs/SETUP.md) first. Run commands from the repository root in PowerShell, or ask Codex to run them. Each standalone sketch is in `lessons/sketches/<Name>/<Name>.ino`, so Arduino IDE users can open it directly too.

Read [command meanings](../docs/COMMANDS.md) before uploading. Every upload replaces the previous program. `COM7` below is a placeholder for your identified XIAO. Before each session: adult present, battery and servo supply disconnected, USB unplugged while changing wires. Introductory lessons use only the named modules and never the servo board. They do not shut down previously configured PCA outputs.

## 0. Identify before overwriting

**Question:** is this board already running Companion?

**Predict:** if it is, help/status will produce recognizable text. An unknown or silent program is inconclusive.

Run `./robot.ps1 ports` before and after connecting the bare XIAO by data cable. Read [the existing-program check](../docs/COMMANDS.md#check-an-existing-program). Use the actual new port:

```powershell
.\robot.ps1 status -Port COM7 -ServoPowerDisconnected
```

**Pass:** identify a specific port and describe the response, without assuming a board name proves its firmware. **Record:** port and text output. **If stuck:** check data cable and competing monitors. Do not upload just to answer a port question.

## 1. Hello: a program lives on a board

**Parts:** XIAO and USB data cable only. **Book:** chapters 4 and 12.

**Concept:** source code is the recipe; compilation translates it; uploading stores the translated program; the serial console lets it talk back.

**Predict:** what will the following sketch say, and how often? Open [Hello.ino](sketches/Hello/Hello.ino) and identify the message and delay.

```powershell
.\robot.ps1 build -Sketch Hello
.\robot.ps1 upload -Sketch Hello -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

**Expected:** `Hello from Andrew! This is the Hello lesson.` repeats about once a second. Ctrl+C closes the monitor. A successful compile alone is not the pass condition; the actual message must arrive from the board.

**Change one thing:** predict the effect of changing `1000` to `2000`; close monitor, edit, upload, observe, and explain. Then restore the original or commit your intentional change. **If stuck:** verify selected port, USB CDC board setting, and cable. **Record:** exact message, observed interval, and sketch revision.

## 2. Scanner: devices have addresses

**Parts:** XIAO + OLED only, then optionally repeat separately with GY-521. **Book:** chapters 9–10.

**Adult wiring check with USB off:** XIAO 3V3 → module VCC; GND → GND; D4/GPIO6 → SDA; D5/GPIO7 → SCL. Inspect labels, not the order in a picture. Use [the wiring specification](../docs/WIRING.md). Keep servos and PCA disconnected. GY-521 clone supply/pull-up checks still apply; do not increase bus voltage to solve a missing response.

**Predict:** OLED normally acknowledges at 0x3C or 0x3D; MPU normally at 0x68 or 0x69. These are addresses, not pin numbers.

```powershell
.\robot.ps1 upload -Sketch Scanner -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

**Expected:** scan text repeats about every three seconds, including actual responding addresses. **Pass:** identify the address of one connected module. **Experiment:** unplug USB, remove that module, reconnect and observe the difference. Never remove jumpers while powered. **If stuck:** inspect crossed SDA/SCL, missing ground, and breadboard contact groups with power off. **Record:** module, observed address, and what a scan cannot prove (display function, sensor orientation, or servo power).

## 3. Face: pixels make expressions

**Parts:** XIAO + OLED, same checked wiring. **Book:** chapters 5 and 13.

**Concept:** coordinates locate pixels, drawing changes a memory image, and sending that image changes the screen. A timer alternates expressions without a long delay in the main loop.

**Predict:** inspect [Face.ino](sketches/Face/Face.ino). What changes when the height is 4 instead of 24?

```powershell
.\robot.ps1 upload -Sketch Face -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

**Expected:** eyes alternate roughly every second; serial text says sleepy/awake. **Pass:** observe both distinct expressions. **Challenge:** change one eye's x coordinate slightly; predict the direction before uploading. Keep rectangles within the 128×64 display. **If stuck:** return to Scanner; address detection and drawing are separate checks. **Record:** code change and visual result, not just compile success.

## 4. Motion: gravity is a clue

**Parts:** XIAO + GY-521 only, with adult-checked 3.3V wiring. **Book:** chapter 6.

**Concept:** accelerometer readings have axes and units. A stationary supported sensor can report gravity; it need not read zero. The gyroscope measures rotation rate, but this lesson prints only acceleration.

**Predict:** with sensor +Z pointing up, Z should be around +9.8m/s². No claim of exact calibration is implied.

```powershell
.\robot.ps1 upload -Sketch Motion -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

**Expected:** three labelled acceleration values every half-second. Slowly rotate the secured board without stressing wires. **Pass:** describe a repeatable change in axes when orientation changes. **Challenge:** predict another orientation, then test. Do not shake, drop, or tug the assembly. **If stuck:** use Scanner and check physical axis markings. **Record:** orientation, readings, units, and noise/variation.

## 5. Integration and one unloaded servo

**Prerequisites:** earlier hardware checks recorded; adult has reviewed [power and meter instructions](../docs/WIRING.md). **Book:** chapters 7–8, 11, 14–15.

Use the existing [Companion](../firmware/Companion/Companion.ino) rather than a beginner full-range sweep. Power everything off before adding modules. Follow [first power-on](../docs/FIRST-POWER-ON.md) exactly. Upload with servo power off:

```powershell
.\robot.ps1 upload -Sketch Companion -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

First inspect `help`, `status`, and `scan`. The adult controls when the verified servo supply is connected, using the guide's sequence. Start with one unloaded channel and tiny `jog` commands. Do not attach legs, arm demo, or widen limits just to complete a lesson.

**Pass:** observed controlled unloaded movement, correct connector orientation, known supply polarity, and documented pulse-off versus power-off behavior. **If stuck:** unplug servo power and inspect; a compile or I2C scan does not validate servo current paths. **Record:** channel, commanded pulses, response, and supply measurements. Full calibration and mounted geometry are later steps.

## 6. Measure, print, revise

**Prerequisites:** actual parts available, printer supervision. **Book:** chapter 16. Use [measurement sheet](../docs/MEASUREMENTS.md) and [fit-gauge instructions](../cad/README.md).

**Predict:** choose which gauge clearance should fit. Print at 100% scale with the actual nozzle/material profile, compare without force, and record the result. **Pass:** measured dimensions and observed fit. A manifold STL check is not a physical pass. Full enclosure and leg design remain future work; do not invent final fastener sizes.

## 7. Improve one thing and explain it

**Book:** chapters 17–20. Start from a working baseline. Create one issue-sized change: eye expression, helpful serial message, or clarified instruction. Write a pass condition. Change one thing, test, and record a limitation. Use [the contribution guide](../CONTRIBUTING.md).

**Graduation evidence:** explain how a sensor reading becomes a face response; identify the separate servo power path; show a useful failed experiment and what it taught you. Walking and cordless power are not graduation requirements.
