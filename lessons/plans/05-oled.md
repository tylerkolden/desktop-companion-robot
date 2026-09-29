# 05. Pixels tell a story

**Program:** `Display` · **Time:** 20–30 minutes · **Partners:** learner operates and explains; adult checks connections and power. Swap the observer/operator roles after the first experiment.

## 1. Mission and success

Connect a successful I2C exchange to a visible change in screen pixels. Success requires the observations below, not just compilation or `[OK]`.

## 2. Starting setup

With USB unplugged, remove the GY-521 connections and set it aside. Add only the AITRIP SSD1306 OLED using the table. Reconnect USB after an adult checks the labels.

## 3. Wiring and pinout

These custom functional illustrations identify the purchased hardware. They are not exact hole-placement diagrams or Tinkercad simulations. Read printed labels; colors are suggestions. Change wiring only with power unplugged.

![oled: functional connections; follow actual board labels](../../docs/wiring/images/oled.png)

Functional pinout: VCC, GND, SDA, SCL. The supplied display photo has **GND / VCC / SCL / SDA** across its top header when viewed from the front. Our illustration groups wires by function, not that physical order.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | AITRIP SSD1306 OLED VCC | 3.3V logic |
| XIAO GND | AITRIP SSD1306 OLED GND | Shared ground |
| XIAO D4 / GPIO6 | AITRIP SSD1306 OLED SDA | I2C data |
| XIAO D5 / GPIO7 | AITRIP SSD1306 OLED SCL | I2C clock |

## 4. Predict before you run

Can a screen answer a scan but still fail to draw the picture we expect? Write one prediction before pressing Enter. Discuss what would count as evidence against it.

## 5. Upload from PowerShell

Close any monitor with Ctrl+C. USB powers this lesson. If returning from an assembled robot, disconnect motor power first. From the repository root, run ports and identify the XIAO. COM5 is an example, not automatic selection. Replace it in both commands if different.

```powershell
.\robot.ps1 ports
.\robot.ps1 upload -Sketch Display -Port COM5 -ServoPowerDisconnected
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

DISPLAY names the expression requested on the OLED. Compare the word with the actual screen; the table cannot prove pixels lit.

Example rows to explain the layout (not recorded test evidence):

```text
   TIME_MS   DISPLAY
      1200     AWAKE
      1300     AWAKE
```

## 7. Investigate

Type `awake`, `happy`, `sleep`, `curious`, `surprised`, `love`, and `cheeky`, pressing Enter and pausing after each. Look for eyes, a tiny nose and a different mouth. Curious raises an eyebrow; Surprised gasps; Love has heart eyes; Cheeky winks and sticks out its tongue. Type `clear`, then `awake` again. Type `test` for a 60-second face parade: each face stays for two seconds, then the seven-face sequence repeats. `stop` freezes the last image; it does not erase it.

## 8. Evidence and exit ticket

Observe seven distinct full faces and a cleared screen. Record the actual address (0x3C or 0x3D) and what each command displayed. Have a teammate guess the emotion from the eyes and mouth before reading its name.

Write: **prediction → command → actual observation → explanation → next question**. Repeat the observation once. Ask your teammate to explain the result in their own words; if either person is unsure, record NOT SURE and repeat or troubleshoot instead of declaring a pass.

## 9. Troubleshoot one change at a time

Initialization does not prove pixels drew correctly. Match VCC/GND/SDA/SCL labels, not pin position. If missing, inspect only with USB off.

## 10. Finish and hand off

Close the monitor and unplug USB. The next lesson swaps the OLED for the servo system; leave the OLED wiring until that setup step.

[Preview all seven faces and learn to edit their pixels](../../docs/faces/README.md).

[Course map](../README.md) · [Previous lesson](04-temperature.md) · [Next lesson](06-servo.md) · [Source code](../sketches/Display/Display.ino)
