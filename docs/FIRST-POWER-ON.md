# Your first powered session

This is the next build step. The firmware is an early bench prototype, not a tested robot release. Use the [wiring table](WIRING.md), with servo power unplugged during wiring and uploads. No battery is used.

## Upload

1. Install Arduino IDE, ESP32 board package **3.3.0**, and the exact libraries listed in `firmware/README.md`.
2. Open `firmware/Companion/Companion.ino`. Select **XIAO_ESP32C3** and the connected port. Set **USB CDC On Boot: Enabled** where that option appears.
3. Leave the servo supply unplugged. Upload over the USB data cable.
4. Open Serial Monitor at **115200**, with **Newline** line endings. Enter `help`, then `status` and `scan`.
5. Power off USB before adding each module. Reconnect and reboot after wiring changes. Confirm addresses and status before attaching servos.

Command output is plain text; no application or Wi-Fi credentials are required. The screen blinks and looks around, goes to sleep after 30 seconds without interaction, and responds to a double tap with a happy face. Taps never arm motion. Tap sensitivity and orientation thresholds still require real-hardware tuning.

## Face and motion-sensor checks

Try `face happy`, `face sleep`, and `face awake`. Check the acceleration messages; with the MPU's +Z axis up, Z should be near +9.8 m/s2 and `upright=1`. If the mounted sensor faces another direction, fix the mount or deliberately update the orientation transform before using demo mode. The present gesture mode stops when Z falls below its upright threshold; this is not fall prevention or a desk-edge detector.

## One servo, no leg attached

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
