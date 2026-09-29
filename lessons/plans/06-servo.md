# 06. Signals become movement

**Program:** `Servo` · **Time:** 20–30 minutes · **Partners:** learner operates and explains; adult checks connections and power. Swap the observer/operator roles after the first experiment.

## 1. Mission and success

Separate driver communication, motor power and observed shaft movement. Success requires the observations below, not just compilation or `[OK]`.

## 2. Starting setup

With USB off, set the OLED aside. Introduce the PCA9685 driver, one unloaded SG90, and the previously specified regulated 5V supply and connector. Keep the 5V supply unplugged. Start with the four XIAO-to-driver logic wires; check scan/status before adding motor power. Use the complete circuit below to connect channel 0 with both supplies unplugged. No horn, leg or mechanism; all other servo channels empty. Keep the verified OE-to-GND jumper if fitted. On first assembly an adult must verify supply voltage/polarity using the linked first-power check before attaching it.

## 3. Wiring and pinout

These custom functional illustrations identify the purchased hardware. They are not exact hole-placement diagrams or Tinkercad simulations. Read printed labels; colors are suggestions. Change wiring only with power unplugged.

![servo: functional connections; follow actual board labels](../../docs/wiring/images/servo.png)

**Identify the green two-screw block on the PCA9685:** its + / V+ terminal accepts the regulated 5V supply positive wire; its - / GND terminal accepts the negative wire. They are not both V+. This block is not VCC, and it is different from the separate barrel-to-terminal connector on the supply cable. Confirm the actual board markings with power unplugged; the illustration does not establish left/right orientation. Functional servo pinout: ground, power, signal. Read the driver row labels before inserting the plug. **Keep all four XIAO logic wires connected**, as shown in this complete diagram: 3V3/VCC, GND/GND, D4/SDA and D5/SCL. The shared-ground wire is one of these four wires, not an extra fifth wire. Channel 0 is the three-pin output group beside the printed 0 among groups 0 through 15. One plug spans GND, V+ and signal within that group, not three neighboring channel numbers. The green block is not channel 0. Verify actual markings; left/right orientation is not established by this diagram. Use only channel 0 for this experiment. USB and adapter grounds meet at PCA GND; adapter positive never meets XIAO 5V or 3V3. Grounded OE is not a reset interlock: unplug servo power for uploads, resets and horn fitting.

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

## 4. Predict before you run

Does finding 0x40 prove that the servo has power or that its shaft moves? Write one prediction before pressing Enter. Discuss what would count as evidence against it.

## 5. Upload from PowerShell

Close any monitor with Ctrl+C. Keep the 5V motor supply unplugged throughout upload and monitor opening. From the repository root, run ports and identify the XIAO. COM5 is an example, not automatic selection. Replace it in both commands if different.

```powershell
.\robot.ps1 ports
.\robot.ps1 upload -Sketch Servo -Port COM5 -ServoPowerDisconnected
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

`start` repeats readings every 100 milliseconds for one minute, then ends automatically; `stop` ends them early. `scan` also stops tests. `test` prints motor preparation without moving; `run` is the separate physical start after the adult check. Do not paste preparation and movement commands together. Unknown commands print help guidance. Ctrl+C closes the monitor; it does not send `stop`.

### Read the columns

All timed runs last **60 seconds** unless stopped or faulted. Rows are scheduled every **100ms (10 per second)**. `read` remains one snapshot. `TIME_MS` is board uptime, so compare consecutive timestamps to see the actual spacing; I2C and serial work can delay a row, and missed rows are not replayed in a burst. `help` repeats the command menu; begin another reading run to print a fresh data header.

CHANNEL stays at 0. PULSE_US is the requested pulse width, not a measured shaft position. Zero means pulse-off requested. Watch the shaft as the numbers change.

Example rows to explain the layout (not recorded test evidence):

```text
   TIME_MS   CHANNEL  PULSE_US
      1200         0      1500
      1300         0      1490
```

## 7. Investigate

With motor power unplugged, open the monitor. Type `scan` and `status`. Type `test`. Follow its three steps: keep the bare motor clear, ask the adult to turn on the checked supply, then type `run`. Watch it turn slowly back and forth for one minute. No calibration or repeated typing is needed. Type `stop` to end early. When DONE appears, unplug motor power. Type `status` or repeat `test` when ready. See the [quick motor test](../../docs/SERVO-RANGE.md) for the short version.

## 8. Evidence and exit ticket

Observe actual shaft movement and automatic completion, then a separate prepared run cancelled by stop that does not resume. All 16 output full-off flags should read 1 afterward. Record YES/NO/NOT SURE; software cannot see the shaft.

Write: **prediction → command → actual observation → explanation → next question**. Repeat the observation once. Ask your teammate to explain the result in their own words; if either person is unsure, record NOT SURE and repeat or troubleshoot instead of declaring a pass.

## 9. Troubleshoot one change at a time

This lesson uses a fixed wider practice range (500–2500µs, centered at 1500µs), not the full mechanical range or an assumed angle. It is for one unloaded servo. No numbers need to be entered or saved. The first centering move can be larger. If it sticks or keeps buzzing, type stop and unplug motor power. DONE means the one-minute timer finished normally; FAULT means a device error. A new BOOT message means the board restarted: save that output for troubleshooting.

## 10. Finish and hand off

Leave motor power unplugged. Close the monitor and unplug USB before building the team circuit. Keep the tested driver logic; the servo will be set aside during the first integration checks.

[Course map](../README.md) · [Previous lesson](05-oled.md) · [Next lesson](07-integration.md) · [Source code](../sketches/Servo/Servo.ino)

[Supply voltage/polarity measurement](../../docs/wiring/README.md) · [Motor power background](../../docs/WIRING.md)
