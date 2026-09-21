# Motor lab: one small movement

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
