# 02. Gravity detective

**Program:** `Accelerometer` · **Time:** 20–30 minutes · **Partners:** learner operates and explains; adult checks connections and power. Swap the observer/operator roles after the first experiment.

## 1. Mission and success

Identify acceleration along X, Y and Z, including gravity, in m/s². Success requires the observations below, not just compilation or `[OK]`.

## 2. Starting setup

From lesson 1, close the monitor and unplug USB. Add only the GY-521 / MPU6050 using the pin table. Reconnect USB after checking labels. Leave XDA, XCL, INT and AD0 unconnected on the checked module; scan verifies its default address.

## 3. Wiring and pinout

These custom functional illustrations identify the purchased hardware. They are not exact hole-placement diagrams or Tinkercad simulations. Read printed labels; colors are suggestions. Change wiring only with power unplugged.

![imu: functional connections; follow actual board labels](../../docs/wiring/images/imu.png)

Functional pinout: VCC, GND, SCL, SDA, XDA, XCL, AD0, INT. Leave XDA/XCL/INT open; verify AD0 default through scanning. Exact delivered header order is unconfirmed. Check regulator/pull-ups at 3.3V; do not substitute 5V to fix detection.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | GY-521 / MPU6050 VCC | 3.3V logic |
| XIAO GND | GY-521 / MPU6050 GND | Shared ground |
| XIAO D4 / GPIO6 | GY-521 / MPU6050 SDA | I2C data |
| XIAO D5 / GPIO7 | GY-521 / MPU6050 SCL | I2C clock |

## 4. Predict before you run

With +Z pointing upward, which reading will be near +9.8? What happens upside down? Write one prediction before pressing Enter. Discuss what would count as evidence against it.

## 5. Upload from PowerShell

Close any monitor with Ctrl+C. USB powers this lesson. If returning from an assembled robot, disconnect motor power first. From the repository root, run ports and identify the XIAO. COM5 is an example, not automatic selection. Replace it in both commands if different.

```powershell
.\robot.ps1 ports
.\robot.ps1 upload -Sketch Accelerometer -Port COM5 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM5 -ServoPowerDisconnected
```

Run each line separately. Upload replaces the previous program. This lesson does not require Companion. The shared repository library is included automatically by robot.ps1.

## 6. Commands inside the monitor

After `Connecting to COM...`, stay in that terminal. You are now talking to the board, not PowerShell. Type **one command and press Enter** each time. Nothing streams or moves automatically at startup. If you missed the greeting, `help` repeats it.

```text
help
status
scan
read
```

`start` repeats readings every half-second; `stop` ends them. `scan` also stops tests. `test` starts a ten-second observation window and ends automatically. Do not paste preparation and movement commands together. Unknown commands print help guidance. Ctrl+C closes the monitor; it does not send `stop`.

## 7. Investigate

Type `read` while still. Type `test`, slowly tilt the board by its edges, and return it to the original position. For a longer experiment use `start`, then `stop`. Repeat with +Z up and down; try +X and +Y up.

## 8. Evidence and exit ticket

Record three readings per orientation. The upward axis is roughly +9.8 m/s² and reverses sign when turned over; small offsets/noise are normal. Do not call this precision calibration.

Write: **prediction → command → actual observation → explanation → next question**. Repeat the observation once. Ask your teammate to explain the result in their own words; if either person is unsure, record NOT SURE and repeat or troubleshoot instead of declaring a pass.

## 9. Troubleshoot one change at a time

A stationary supported sensor need not read zero. If absent, check 0x68/0x69 with scan and inspect the four connections with USB off.

## 10. Finish and hand off

Keep the exact same GY-521 wiring for the gyroscope lesson; change only the program.

[Course map](../README.md) · [Previous lesson](01-controller.md) · [Next lesson](03-gyroscope.md) · [Source code](../sketches/Accelerometer/Accelerometer.ino)
