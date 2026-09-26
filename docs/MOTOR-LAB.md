# Motor lab: one small movement

**Legacy Companion reference.** New learners should use [the current standalone course](../lessons/README.md). Commands in this document describe the older application, not the new Servo/Integration sketches.

## If the tiny movement is hard to see

Follow [lesson 5B.1: Big wiggle](../lessons/legacy.md#5b1-big-wiggle-make-movement-easier-to-see). The updated Companion adds `wiggle` (guide, no movement) and `wiggle start` (one unloaded servo on channel 0, three repeated low/high pairs, automatic stop). It uses at most center +/-50us inside existing limits. Only the driver is required. Follow the lesson's upload, power preparation and observation steps before starting.

## If every channel stays still

An all-channel search with no visible movement does not identify the cause. The 10us offset can be too small to observe. Do not widen limits just to force a visible result.

The `diagnose` command reads PCA9685 MODE1, MODE2, prescaler and all 16 output register sets. It does not start movement or rewrite configuration. It rejects a running timed test, hunt or gesture. Readback distinguishes configuration faults from an I2C address acknowledgement, but cannot prove a physical waveform or motor voltage.

With motor supply unplugged, upload the diagnostic firmware and open the monitor. Send `stop`, then `diagnose`, pressing Enter after each. All output full-off flags should be 1 after stop. Copy the complete report. A subsequent `cal 0` followed promptly by `diagnose`, still with motor supply unplugged, checks that CH00's programmed pulse changes (about 307 ticks for default 1500us); finish with `stop`. This does not test shaft movement.

**OE is a separate physical gate:** LOW enables outputs; HIGH disables them even if I2C communication succeeds. The reference design pulls it low, but the delivered clone must be checked. Do not infer its state from an empty pin or from the board's power LED. The firmware cannot read OE through I2C. Likewise, 5V at the input block does not establish voltage at the motor connector under load. Diagnose these separately before assigning blame to the software, driver or motor.

Reference: [PCA9685 control-pin roles](https://learn.adafruit.com/16-channel-pwm-servo-driver/pinouts).

Start with lesson 5B's completed wiring and measured power check. Keep the motor unloaded and its shaft clear. Open the serial monitor; type each command there and press Enter, not at the PowerShell prompt.

1. Type `motor`. Companion requests pulse-off and shows preparation instructions. Opening this guide starts no movement.
2. Read the instructions with an adult. Once the lesson's power and wiring checks are complete, type `go`.
3. Watch the shaft. `[1/2]` requests center for half a second; `[2/2]` requests a tiny offset for half a second. `stop` cancels the test.
4. `[DONE]` means the sequence ended and pulse-off was requested. Record **YES / NO / NOT SURE** for observed movement. The board cannot see the shaft.
5. Remove the 5V supply power. If unsure, review the plug, power and common ground; do not increase travel blindly. To repeat after preparation, open `motor` again. A second `go` alone does not restart the test.

The first move to center can be larger than the small offset. A 10us offset may be difficult to see. Pulse-off is not electrical power-off, and the software deadline cannot guarantee a stop after a controller or communication failure.

## Find channel 0

Find the **printed number 0** beside the output headers. The 16 output groups are numbered **0 through 15**. Each channel has three pins; channel 0 is the whole three-pin group marked 0, not one individual pin or the green power block.

```text
Logical grouping only — NOT physical board orientation

channel:       0          1          2       ... 15
signal         o          o          o            o
V+             o          o          o            o
GND            o          o          o            o
               ^
       one plug covers this group
```

| Channel-0 pin | Servo wire function |
|---|---|
| GND | Ground, usually brown/black |
| V+ | Power, usually red |
| PWM / signal | Control, usually orange/yellow |

Match the actual board markings and servo connector before plugging in, with power off. Do not put the plug across three neighboring channel numbers. Do not guess left/right or row orientation from this logical map. If markings are unreadable, use a clear close-up before connecting.

![Complete XIAO, driver and motor circuit](wiring/images/servo.png)

The [PCA9685 reference guide](https://learn.adafruit.com/16-channel-pwm-servo-driver/hooking-it-up) describes the three-pin servo connectors. It is a reference board, not verification of the physical orientation of the delivered Teyleten board. See the [complete pin tables](wiring/README.md).

Updated firmware must be compiled and uploaded before `motor` and `go` are available. `test N` remains the direct advanced command; beginners use the guide above.

## Find an unknown channel without moving the plug

Use this when communication works but the motor has not visibly moved. Keep **one unloaded servo** in its current connector, wired as in the complete diagram above. With both supplies off first, disconnect any other motors. After the existing power check, leave the wiring untouched during the test.

In the monitor, type `hunt` and press Enter. It prints the plan without moving a motor. When prepared, type `hunt start` and press Enter.

The finder checks channels 0 through 15 automatically. For each channel it requests center for 500ms, center +10us for 500ms, then requests all outputs off for a one-second pause. Only one output is driven at a time. The complete run takes about 32 seconds. Type `stop` to cancel; `status` shows the current channel. No restart occurs automatically.

Record the printed channel number if the shaft moves. A response on a different number supports a channel mismatch. No movement remains inconclusive: the offset may be below the servo's visible deadband, and this test does not measure signal voltage, output enable, motor power under load or motor health. Do not increase the range automatically. The first move to center may be larger than the offset. Software timing is not a hardware cutoff.

Channels 0–3 use existing saved centers and require 10us headroom inside their limits; channels 4–15 use 1500us then 1510us. The test changes no saved settings. On a reported write failure, unplug the motor supply; pulse-off cannot be guaranteed over a failed connection. After completion, unplug motor power and report the observed channel or NONE / UNSURE.
