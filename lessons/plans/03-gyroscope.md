# 03. Turn detective

**Program:** `Gyroscope` · **Time:** 20–30 minutes · **Partners:** learner operates and explains; adult checks connections and power. Swap the observer/operator roles after the first experiment.

## 1. Mission and success

Distinguish rotation rate (degrees per second) from tilt angle. Success requires the observations below, not just compilation or `[OK]`.

## 2. Starting setup

Keep the XIAO + GY-521 wiring from lesson 2. No wiring change. Close the monitor before uploading the Gyroscope sketch.

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

Will a tilted but motionless board keep reporting a large rotation rate? Write one prediction before pressing Enter. Discuss what would count as evidence against it.

## 5. Upload from PowerShell

Close any monitor with Ctrl+C. USB powers this lesson. If returning from an assembled robot, disconnect motor power first. From the repository root, run ports and identify the XIAO. COM5 is an example, not automatic selection. Replace it in both commands if different.

```powershell
.\robot.ps1 ports
.\robot.ps1 upload -Sketch Gyroscope -Port COM5 -ServoPowerDisconnected
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

Type `read` while still. Type `test`, rotate gently one way, pause, rotate the opposite way, then pause again. Repeat with another axis. Use `start` and `stop` if you need more observation time.

## 8. Evidence and exit ticket

Rotation produces a rate change; opposite rotation changes its sign; holding still returns near the baseline, even while tilted. Record baseline, direction and approximate peak for two directions.

Write: **prediction → command → actual observation → explanation → next question**. Repeat the observation once. Ask your teammate to explain the result in their own words; if either person is unsure, record NOT SURE and repeat or troubleshoot instead of declaring a pass.

## 9. Troubleshoot one change at a time

The program converts library radians/second to degrees/second. It does not estimate angle. Small stationary bias is expected; avoid pulling wires or spinning repeatedly.

## 10. Finish and hand off

Keep the same GY-521 wiring for its third measuring function: temperature.

[Course map](../README.md) · [Previous lesson](02-accelerometer.md) · [Next lesson](04-temperature.md) · [Source code](../sketches/Gyroscope/Gyroscope.ino)
