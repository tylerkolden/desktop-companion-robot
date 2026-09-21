# Your first powered session

Wiring reference: [full pinout atlas and verification limits](wiring/README.md). Use the stage matching your lesson; do not connect every pictured module at once.

This is the integration guide for lesson 5, after the standalone Hello, Scanner, Face and Motion lessons. It is not the starting point for lesson 1. The firmware is an early bench prototype, not a tested robot release. Use the [wiring table](WIRING.md), with servo power unplugged during wiring and uploads. No battery is used.

## Upload

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: unplug USB and the servo adapter. Battery stays disconnected.** These custom Tinkercad-style illustrations are not a Tinkercad project or simulation. Board shapes are illustrative; terminal positions are functional, not verified physical placement. Read actual board labels.

![USB wiring: labelled endpoints and separate power paths](wiring/images/usb.png)

The pinout for this stage is USB-C socket only: **all XIAO header pins are unused**. No external modules or battery.

| From / pin | To / pin | Meaning |
|---|---|---|
| Computer USB port | XIAO USB-C socket | Data cable; all headers empty |

<!-- wiring:end -->

**Entry checkpoint:** close the monitor, unplug USB, and remove the lesson-4 GY-521 connections. Start with the bare XIAO shown above. Set the tested sensor and OLED aside for the next checkpoint; servo power and battery stay disconnected.

For the repository terminal workflow, complete [setup](SETUP.md), identify the port with `./robot.ps1 ports`, then run `./robot.ps1 upload -Sketch Companion -Port COM7 -ServoPowerDisconnected` using your actual port. Open the console with `./robot.ps1 monitor -Port COM7 -ServoPowerDisconnected` while servo power is still off. The switch records your physical check; it cannot measure power. The following IDE steps are an optional alternative.

1. Install Arduino IDE, ESP32 board package **3.3.0**, and the exact libraries listed in `firmware/README.md`.
2. Open `firmware/Companion/Companion.ino`. Select **XIAO_ESP32C3** and the connected port. Set **USB CDC On Boot: Enabled** where that option appears.
3. Leave the servo supply unplugged. Upload over the USB data cable.
4. Open Serial Monitor at **115200**, with **Newline** line endings. Enter `help`, then `status` and `scan`.
5. Power off USB before adding each module. Reconnect and reboot after wiring changes. Confirm addresses and status before attaching servos.

Command output is plain text; no application or Wi-Fi credentials are required. At this bare-board checkpoint, missing OLED/IMU/PCA status and no I2C addresses are expected. After the modules pass the next checkpoint, the screen blinks and looks around, goes to sleep after 30 seconds without interaction, and responds to a double tap with a happy face. Taps never arm motion. Tap sensitivity and orientation thresholds still require real-hardware tuning.

## Face and motion-sensor checks

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: unplug USB and the servo adapter. Battery stays disconnected.** These custom Tinkercad-style illustrations are not a Tinkercad project or simulation. Board shapes are illustrative; terminal positions are functional, not verified physical placement. Read actual board labels.

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

Functional pinout: VCC = logic 3.3V; V+ = separate servo power; GND = common ground; SDA/SCL = I2C; OE = active-low output enable. Exact clone header order is unconfirmed. Verify OE pull-down; ground OE only if floating. V+ and servo plugs remain disconnected in this logic-only stage.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | Teyleten PCA9685 VCC | 3.3V logic |
| XIAO GND | Teyleten PCA9685 GND | Shared ground |
| XIAO D4 / GPIO6 | Teyleten PCA9685 SDA | I2C data |
| XIAO D5 / GPIO7 | Teyleten PCA9685 SCL | I2C clock |

<!-- wiring:end -->

**5A: rebuild the logic bus, one device at a time.** The three diagrams above are successive additions here, unlike the separate one-module lessons. Share the like-labelled 3V3, GND, SDA and SCL nets using checked low-current breadboard junctions.

1. Close the monitor and unplug USB. Add the OLED using its diagram/table, reconnect USB, reopen the monitor, and confirm `scan` finds its recorded address. Try a face command.
2. Close the monitor and unplug USB again. Keep the OLED and add the previously tested GY-521 using its diagram/table. Reconnect USB and reopen the monitor. Confirm both addresses and sensor readings.
3. Repeat the power-off procedure to add PCA logic using its diagram/table. No servos or servo supply yet. Reconnect USB, reopen the monitor, and confirm `scan` and `status` recognize all three modules (PCA all-call 0x70 may also appear). Stop here if any previously working device disappears.

Companion stays installed throughout these additions; do not re-upload Face or Motion. Each USB reconnection restarts Companion so it can initialize the newly added device.

Try `face happy`, `face sleep`, and `face awake`. Check the acceleration messages; with the MPU's +Z axis up, Z should be near +9.8 m/s2 and `upright=1`. If the mounted sensor faces another direction, fix the mount or deliberately update the orientation transform before using demo mode. The present gesture mode stops when Z falls below its upright threshold; this is not fall prevention or a desk-edge detector.

## One servo, no leg attached

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: unplug USB and the servo adapter. Battery stays disconnected.** These custom Tinkercad-style illustrations are not a Tinkercad project or simulation. Board shapes are illustrative; terminal positions are functional, not verified physical placement. Read actual board labels.

![SERVO wiring: labelled endpoints and separate power paths](wiring/images/servo.png)

Functional servo pinout: ground, power, signal. Read the driver row labels before inserting the plug. **Also complete the PCA logic diagram** (3V3/VCC, GND, SDA, SCL). Start with only channel 0; later 0/1/2/3 map to front-left/front-right/rear-left/rear-right. USB and adapter grounds meet at PCA GND; adapter positive never meets XIAO 5V or 3V3. Grounded OE is not a reset interlock: unplug servo power for uploads, resets and horn fitting.

| From / pin | To / pin | Meaning |
|---|---|---|
| Adapter positive | PCA V+ screw terminal | Regulated 5V; direct power wire |
| Adapter negative | PCA GND screw terminal | Direct power wire |
| XIAO GND | PCA GND | Common reference |
| PCA channel 0 GND | SG90 ground | Usually brown/black; verify |
| PCA channel 0 V+ | SG90 power | Usually red; verify |
| PCA channel 0 PWM | SG90 signal | Usually orange/yellow; verify |

![METER wiring: labelled endpoints and separate power paths](wiring/images/meter.png)

Probe-jack pinout is functional, not the physical socket position of every meter variant. Select DC voltage before probing. Verify barrel size and polarity against the actual adapter; do not assume center-positive from a generic picture.

| From / pin | To / pin | Meaning |
|---|---|---|
| Black probe / COM jack | Adapter negative output | Voltage check only |
| Red probe / V-ohm jack | Adapter positive output | DC volts; never current mode |

<!-- wiring:end -->

1. Disconnect both supplies. Attach one unloaded servo to channel 0.
2. Start controller USB with servo power still off. `status` should report PCA available and no active channel.
3. Verify the wall supply is approximately +5V at the servo terminal, then connect servo power.
4. Enter `cal 0`. This sends a nominal 1500us neutral pulse to channel 0 only. Other channels are off.
5. Enter `jog 0 10` or `jog 0 -10` for small adjustments. Default limits are 1450-1550us, intentionally narrow. A 15-second gap without a calibration command disables pulses.
6. Enter `stop` to disable pulses. Unplug servo power before installing a horn. Pulse-off does not disconnect electrical power or guarantee the leg stays upright.

To change limits while stopped, use `limits 0 1400 1600`. The firmware rejects bounds outside 1300-1700us. Never widen limits without physically inspecting the available travel. `center 0` sets the current active pulse as center, provided it is strictly inside the limits. `invert 0` reverses future gesture direction while stopped. Any changed calibration needs saving again.

Only after physically checking the servo and mounted geometry, enter `save 0`. This is your explicit confirmation; firmware cannot detect binding. Repeat for channels 1, 2 and 3. `status` lists saved flags; saved settings survive reboot, but arming never does.

## Short gesture demo

With all four calibrations saved, the IMU connected and +Z up, enter `demo`. It performs a 10-second small synchronized gesture, at most +/-25us around each center and always within saved limits. It then disables pulses. This is not a walking gait. Support the chassis so removing pulses cannot make it fall. Enter `stop` at any time or unplug servo power.

## Limits of the current stop mechanism

The program disables PCA channels during initialization, on a calibration timeout, and on detected I2C failures. **It is not a hardware emergency stop.** A PCA9685 can retain PWM while the ESP32 reboots, freezes, loses USB power, or cannot communicate over I2C. Always disconnect servo power before uploading, resetting or repairing wiring. If software cannot talk to the PCA, it cannot guarantee that pulse-off commands reached it. A future OE interlock with a pull-up is an optional hardware improvement, not currently wired or claimed.

## Report back

Copy the output from `status` and `scan`, plus a photo of the first printed gauges with your parts resting in them. Record whether the controller has a stable USB port, the OLED works, the IMU reads correctly and one servo moves without buzzing. These observations unlock the final enclosure and larger gestures.
