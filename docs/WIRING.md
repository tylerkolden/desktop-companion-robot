# V1 wiring specification

Wiring reference: [full pinout atlas and verification limits](wiring/README.md). Use the stage matching your lesson; do not connect every pictured module at once.

Status: proposed connection plan, not yet physically validated. Read board labels; do not infer pin order from wire colors or the orientation of an online photograph.

## Power architecture

<!-- wiring:start -->
**Parts for this power checkpoint:** a regulated 5V wall supply, its low-voltage connector and prepared wires, and a multimeter. For the motor connection, also identify the SG90 servo and PCA9685 driver. The supply powers the motor through the driver; the meter checks its output before connection. Identify these parts before following the diagram.

### Wiring visuals and pinouts

**Before assembling these connections: close the monitor and unplug the XIAO USB cable. Keep the newly introduced 5V supply unplugged from the wall until the voltage-check step. On a repeat session, unplug it before changing wires.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![SERVO wiring: labelled endpoints and separate power paths](wiring/images/servo.png)

**Identify the green two-screw block on the PCA9685:** its + / V+ terminal accepts the regulated 5V supply positive wire; its - / GND terminal accepts the negative wire. They are not both V+. This block is not VCC, and it is different from the separate barrel-to-terminal connector on the supply cable. Confirm the actual board markings with power unplugged; the illustration does not establish left/right orientation. Functional servo pinout: ground, power, signal. Read the driver row labels before inserting the plug. **Keep all four XIAO logic wires connected**, as shown in this complete diagram: 3V3/VCC, GND/GND, D4/SDA and D5/SCL. The shared-ground wire is one of these four wires, not an extra fifth wire. Use only channel 0 for this experiment. USB and adapter grounds meet at PCA GND; adapter positive never meets XIAO 5V or 3V3. Grounded OE is not a reset interlock: unplug servo power for uploads, resets and horn fitting.

| From / pin | To / pin | Meaning |
|---|---|---|
| Adapter positive | PCA green block: + / V+ | Regulated 5V; direct power wire |
| Adapter negative | PCA green block: - / GND | Direct power wire |
| XIAO 3V3 | PCA VCC | Logic power, separate from V+ |
| XIAO GND | PCA GND | Common reference |
| XIAO D4 / GPIO6 | PCA SDA | I2C data |
| XIAO D5 / GPIO7 | PCA SCL | I2C clock |
| PCA channel 0 GND | SG90 ground | Usually brown/black; verify |
| PCA channel 0 V+ | SG90 power | Usually red; verify |
| PCA channel 0 PWM | SG90 signal | Usually orange/yellow; verify |

<!-- wiring:end -->

1. USB powers the XIAO. The XIAO 3V3 output supplies only low-current logic and sensors.
2. The separate 5V/4A adapter feeds PCA9685 **V+** and **GND** through the screw terminal.
3. The PCA9685 logic **VCC** gets 3.3V from the XIAO.
4. XIAO and PCA9685 share ground. Keep the high-current servo path directly between the wall adapter, terminal block and servo connectors.
5. Leave the XIAO **5V pin unconnected** in this two-supply design. Do not join the wall adapter's +5V to USB power.

## Logic connections

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![OLED wiring: labelled endpoints and separate power paths](wiring/images/oled.png)

Functional pinout: VCC, GND, SDA, SCL. The supplied display photo has **GND / VCC / SCL / SDA** across its top header when viewed from the front. Our illustration groups wires by function, not that physical order.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | AITRIP SSD1306 OLED VCC | 3.3V logic |
| XIAO GND | AITRIP SSD1306 OLED GND | Shared ground |
| XIAO D4 / GPIO6 | AITRIP SSD1306 OLED SDA | I2C data |
| XIAO D5 / GPIO7 | AITRIP SSD1306 OLED SCL | I2C clock |

![IMU wiring: labelled endpoints and separate power paths](wiring/images/imu.png)

Functional pinout: VCC, GND, SCL, SDA, XDA, XCL, AD0, INT. Leave XDA/XCL/INT open; verify AD0 default through scanning. Exact delivered header order is unconfirmed. Check regulator/pull-ups at 3.3V; do not substitute 5V to fix detection.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | GY-521 / MPU6050 VCC | 3.3V logic |
| XIAO GND | GY-521 / MPU6050 GND | Shared ground |
| XIAO D4 / GPIO6 | GY-521 / MPU6050 SDA | I2C data |
| XIAO D5 / GPIO7 | GY-521 / MPU6050 SCL | I2C clock |

![PCA wiring: labelled endpoints and separate power paths](wiring/images/pca.png)

The green two-screw block is a separate power input: one terminal is + / V+, the other is - / GND. It stays empty during this communication-only test; the driver electronics receive power through VCC. Follow the board markings, not a guessed left/right position. Functional pinout: VCC = logic 3.3V; V+ = separate output-power input; GND = common ground; SDA/SCL = I2C; OE = active-low output enable. Exact clone header order is unconfirmed. Verify OE pull-down; ground OE only if floating. Use only VCC, GND, SDA and SCL for this communication test. Leave V+ and the numbered output connectors empty.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | Teyleten PCA9685 VCC | 3.3V logic |
| XIAO GND | Teyleten PCA9685 GND | Shared ground |
| XIAO D4 / GPIO6 | Teyleten PCA9685 SDA | I2C data |
| XIAO D5 / GPIO7 | Teyleten PCA9685 SCL | I2C clock |

<!-- wiring:end -->

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

<!-- wiring:start -->
**Parts for this power checkpoint:** a regulated 5V wall supply, its low-voltage connector and prepared wires, and a multimeter. For the motor connection, also identify the SG90 servo and PCA9685 driver. The supply powers the motor through the driver; the meter checks its output before connection. Identify these parts before following the diagram.

### Wiring visuals and pinouts

**Before assembling these connections: close the monitor and unplug the XIAO USB cable. Keep the newly introduced 5V supply unplugged from the wall until the voltage-check step. On a repeat session, unplug it before changing wires.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![METER wiring: labelled endpoints and separate power paths](wiring/images/meter.png)

Probe-jack pinout is functional, not the physical socket position of every meter variant. Select DC voltage before probing. Verify barrel size and polarity against the actual adapter; do not assume center-positive from a generic picture.

| From / pin | To / pin | Meaning |
|---|---|---|
| Black probe / COM jack | Adapter negative output | Voltage check only |
| Red probe / V-ohm jack | Adapter positive output | DC volts; never current mode |

<!-- wiring:end -->

Black lead in COM; red lead in the voltage/resistance socket; select DC volts (20V range on a manual-ranging meter). Measure the low-voltage adapter output and then terminal polarity before connecting electronics. The target is approximately 5V; investigate any materially incorrect reading. Never put a meter in current mode directly across the supply: that shorts it. Continuity checks require power disconnected. No mains-side probing is needed.

## References

- [Seeed XIAO ESP32C3 pin map](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [PCA9685 breakout power and signal roles](https://learn.adafruit.com/16-channel-pwm-servo-driver/pinouts): reference design; verify the Teyleten clone's actual labels and implementation.
