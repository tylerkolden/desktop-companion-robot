# 01. Meet the controller

**Program:** `Basics` · **Time:** 20–30 minutes · **Partners:** learner operates and explains; adult checks connections and power. Swap the observer/operator roles after the first experiment.

## 1. Mission and success

Explain the difference between building, uploading and running a program. Success requires the observations below, not just compilation or `[OK]`.

## 2. Starting setup

Start with a bare XIAO ESP32C3 and USB data cable. All header pins are unused. If returning with an assembled robot, unplug its motor supply and USB before disconnecting external modules.

## 3. Wiring and pinout

These custom functional illustrations identify the purchased hardware. They are not exact hole-placement diagrams or Tinkercad simulations. Read printed labels; colors are suggestions. Change wiring only with power unplugged.

![usb: functional connections; follow actual board labels](../../docs/wiring/images/usb.png)

The pinout for this stage is USB-C socket only: **all XIAO header pins are unused**.

| From / pin | To / pin | Meaning |
|---|---|---|
| Computer USB port | XIAO USB-C socket | Data cable; all headers empty |

## 4. Predict before you run

Will the board remember its program after USB is unplugged? Write one prediction before pressing Enter. Discuss what would count as evidence against it.

## 5. Upload from PowerShell

Close any monitor with Ctrl+C. USB powers this lesson. If returning from an assembled robot, disconnect motor power first. From the repository root, run ports and identify the XIAO. COM5 is an example, not automatic selection. Replace it in both commands if different.

```powershell
.\robot.ps1 ports
.\robot.ps1 upload -Sketch Basics -Port COM5 -ServoPowerDisconnected
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

Read TIME_MS and MESSAGE: the changing timestamp proves the program keeps running.

Example rows to explain the layout (not recorded test evidence):

```text
   TIME_MS  MESSAGE
      1200  Hello, teammate!
      1300  Hello, teammate!
```

## 7. Investigate

Type `read` to receive one greeting. Type `test` and watch the timestamps advance during the 60-second test. After it ends, close the monitor, unplug/reconnect USB, reopen the monitor and type `read` without uploading again.

## 8. Evidence and exit ticket

The greeting returns after reconnecting. During test, messages arrive about ten times a second. Explain that flash stores the program while power lets it run.

Write: **prediction → command → actual observation → explanation → next question**. Repeat the observation once. Ask your teammate to explain the result in their own words; if either person is unsure, record NOT SURE and repeat or troubleshoot instead of declaring a pass.

## 9. Troubleshoot one change at a time

A successful build is not proof the board ran. Check the current port and USB data cable if no replies arrive.

## 10. Finish and hand off

Keep the bare XIAO. Next, add the GY-521 for acceleration.

[Course map](../README.md) · [Next lesson](02-accelerometer.md) · [Source code](../sketches/Basics/Basics.ino)
