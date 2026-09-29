# 04. The chip thermometer

**Program:** `Temperature` · **Time:** 20–30 minutes · **Partners:** learner operates and explains; adult checks connections and power. Swap the observer/operator roles after the first experiment.

## 1. Mission and success

Read Celsius and distinguish chip temperature from room or body temperature. Success requires the observations below, not just compilation or `[OK]`.

## 2. Starting setup

Keep the same XIAO + GY-521 circuit. Close the monitor and upload Temperature. No separate temperature module is needed: this sensor is inside the MPU6050.

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

Could a powered chip be warmer than the room around it? Write one prediction before pressing Enter. Discuss what would count as evidence against it.

## 5. Upload from PowerShell

Close any monitor with Ctrl+C. USB powers this lesson. If returning from an assembled robot, disconnect motor power first. From the repository root, run ports and identify the XIAO. COM5 is an example, not automatic selection. Replace it in both commands if different.

```powershell
.\robot.ps1 ports
.\robot.ps1 upload -Sketch Temperature -Port COM5 -ServoPowerDisconnected
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

`start` repeats readings every 100 milliseconds for one minute, then ends automatically; `stop` ends them early. `scan` also stops tests. `test` starts a 60-second observation window and ends automatically. Do not paste preparation and movement commands together. Unknown commands print help guidance. Ctrl+C closes the monitor; it does not send `stop`.

### Read the columns

All timed runs last **60 seconds** unless stopped or faulted. Rows are scheduled every **100ms (10 per second)**. `read` remains one snapshot. `TIME_MS` is board uptime, so compare consecutive timestamps to see the actual spacing; I2C and serial work can delay a row, and missed rows are not replayed in a burst. `help` repeats the command menu; begin another reading run to print a fresh data header.

TEMP_C is one chip-temperature measurement in Celsius. It has no X, Y or Z axes. It may barely change during a minute.

Example rows to explain the layout (not recorded test evidence):

```text
   TIME_MS    TEMP_C
      1200     25.12
      1300     25.12
```

## 7. Investigate

Type `read` and record the value. Type `test` for a 60-second sample. Leave the board in the same place for a minute and repeat; compare the readings without touching contacts, applying heat, liquids or cooling sprays.

## 8. Evidence and exit ticket

Obtain finite Celsius readings in two observation windows and explain why an unchanged value is a valid result. A difference over time may be small; do not require an artificial temperature increase to pass.

Write: **prediction → command → actual observation → explanation → next question**. Repeat the observation once. Ask your teammate to explain the result in their own words; if either person is unsure, record NOT SURE and repeat or troubleshoot instead of declaring a pass.

## 9. Troubleshoot one change at a time

This measures die temperature, not calibrated ambient temperature. It is not a medical thermometer. An I2C response alone does not validate accuracy.

## 10. Finish and hand off

Close the monitor and unplug USB. The next lesson swaps the GY-521 for the OLED; leave the sensor wiring until that setup step.

[Course map](../README.md) · [Previous lesson](03-gyroscope.md) · [Next lesson](05-oled.md) · [Source code](../sketches/Temperature/Temperature.ino)
