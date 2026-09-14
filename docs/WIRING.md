# V1 wiring specification

Status: proposed connection plan, not yet physically validated. Read board labels; do not infer pin order from wire colors or the orientation of an online photograph.

## Power architecture

1. USB powers the XIAO. The XIAO 3V3 output supplies only low-current logic and sensors.
2. The separate 5V/4A adapter feeds PCA9685 **V+** and **GND** through the screw terminal.
3. The PCA9685 logic **VCC** gets 3.3V from the XIAO.
4. XIAO and PCA9685 share ground. Keep the high-current servo path directly between the wall adapter, terminal block and servo connectors.
5. Leave the XIAO **5V pin unconnected** in this two-supply design. Do not join the wall adapter's +5V to USB power.

## Logic connections

| From XIAO | Connect to |
|---|---|
| 3V3 | OLED VCC, GY-521 VCC, PCA9685 VCC |
| GND | OLED GND, GY-521 GND, PCA9685 GND |
| D4 / GPIO6 | OLED SDA, GY-521 SDA, PCA9685 SDA |
| D5 / GPIO7 | OLED SCL, GY-521 SCL, PCA9685 SCL |

Use `Wire.begin(6, 7)` in ESP32 Arduino firmware. Sensor/control transactions start at 100kHz. The OLED library temporarily uses 400kHz for framebuffer transfers and restores 100kHz afterwards; keep jumper runs short. Use mini breadboards as junctions for these four low-current nets, checking continuity of each connected group before applying power. Pre-soldered XIAO header placement may need adjacent breadboards to leave accessible holes; never force pins or assume opposite sides of a board are connected.

Use 3.3V module power so I2C pull-ups do not pull the bus to 5V. The GY-521 clone's onboard regulator and pull-ups must be verified during sensor bring-up; if it fails at this supply, investigate that module rather than applying 5V to the shared bus.

Leave MPU6050 auxiliary XDA/XCL and INT unconnected for polling. AD0 normally defaults low; verify by scanning. PCA9685 OE normally has a pull-down on this board style; verify outputs are enabled. If OE is floating, connect it to GND. Its V+ control-header pin is not a logic supply.

The included firmware uses this wiring without an OE interlock. A grounded OE allows retained pulses while the controller resets. Disconnect servo power before uploads or resets; see [First power-on](FIRST-POWER-ON.md). Adding a fail-disabled OE circuit is future work, not a property of the current wiring.

## Servo connections

| PCA9685 channel | Planned location |
|---|---|
| 0 | Front left |
| 1 | Front right |
| 2 | Rear left |
| 3 | Rear right |

Brown/black normally means GND, red means V+, orange/yellow means signal. Match actual markings. Start at 50Hz with a nominal 1500 microsecond neutral pulse on one unloaded servo. Do not use an automatic 0-180 degree sweep: the mechanical range is not yet known. Calibrate each horn direction, center offset and safe travel independently.

## Expected I2C addresses

| Device | Expected address |
|---|---|
| OLED | Usually 0x3C; sometimes 0x3D |
| MPU6050 | Usually 0x68; 0x69 with AD0 high |
| PCA9685 | Usually 0x40; 0x70 may also acknowledge as the all-call address |

Addresses are expectations, not test results. Connect one module at a time to identify any mismatch.

## Meter setup for the first power check

Black lead in COM; red lead in the voltage/resistance socket; select DC volts (20V range on a manual-ranging meter). Measure the low-voltage adapter output and then terminal polarity before connecting electronics. The target is approximately 5V; investigate any materially incorrect reading. Never put a meter in current mode directly across the supply: that shorts it. Continuity checks require power disconnected. No mains-side probing is needed.

## References

- [Seeed XIAO ESP32C3 pin map](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [PCA9685 breakout power and signal roles](https://learn.adafruit.com/16-channel-pwm-servo-driver/pinouts): reference design; verify the Teyleten clone's actual labels and implementation.
