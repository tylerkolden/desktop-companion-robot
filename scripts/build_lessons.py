"""Generate the consistent, one-page-per-lesson curriculum and book appendix.
Run build_wiring.py first. This generator reads its reviewed images/tables.
"""
from pathlib import Path
import re
ROOT=Path(__file__).resolve().parents[1]
LESSONS=ROOT/'lessons'
COURSE=[
 ('01-controller','Basics','Meet the controller','usb',
  'Explain the difference between building, uploading and running a program.',
  'Start with a bare XIAO ESP32C3 and USB data cable. All header pins are unused. If returning with an assembled robot, unplug its motor supply and USB before disconnecting external modules.',
  'Will the board remember its program after USB is unplugged?',
  'Type `read` to receive one greeting. Type `test` and watch the timestamps advance during the 60-second test. After it ends, close the monitor, unplug/reconnect USB, reopen the monitor and type `read` without uploading again.',
  'The greeting returns after reconnecting. During test, messages arrive about ten times a second. Explain that flash stores the program while power lets it run.',
  'A successful build is not proof the board ran. Check the current port and USB data cable if no replies arrive.',
  'Keep the bare XIAO. Next, add the GY-521 for acceleration.'),
 ('02-accelerometer','Accelerometer','Gravity detective','imu',
  'Identify acceleration along X, Y and Z, including gravity, in m/s².',
  'From lesson 1, close the monitor and unplug USB. Add only the GY-521 / MPU6050 using the pin table. Reconnect USB after checking labels. Leave XDA, XCL, INT and AD0 unconnected on the checked module; scan verifies its default address.',
  'With +Z pointing upward, which reading will be near +9.8? What happens upside down?',
  'Type `read` while still. Type `test`, slowly tilt the board by its edges, and return it to the original position. For another one-minute experiment use `start`; `stop` ends it early. Repeat with +Z up and down; try +X and +Y up.',
  'Record three readings per orientation. The upward axis is roughly +9.8 m/s² and reverses sign when turned over; small offsets/noise are normal. Do not call this precision calibration.',
  'A stationary supported sensor need not read zero. If absent, check 0x68/0x69 with scan and inspect the four connections with USB off.',
  'Keep the exact same GY-521 wiring for the gyroscope lesson; change only the program.'),
 ('03-gyroscope','Gyroscope','Turn detective','imu',
  'Distinguish rotation rate (degrees per second) from tilt angle.',
  'Keep the XIAO + GY-521 wiring from lesson 2. No wiring change. Close the monitor before uploading the Gyroscope sketch.',
  'Will a tilted but motionless board keep reporting a large rotation rate?',
  'Type `read` while still. Type `test`, rotate gently one way, pause, rotate the opposite way, then pause again. Repeat with another axis. Use `start` and `stop` if you need more observation time.',
  'Rotation produces a rate change; opposite rotation changes its sign; holding still returns near the baseline, even while tilted. Record baseline, direction and approximate peak for two directions.',
  'The program converts library radians/second to degrees/second. It does not estimate angle. Small stationary bias is expected; avoid pulling wires or spinning repeatedly.',
  'Keep the same GY-521 wiring for its third measuring function: temperature.'),
 ('04-temperature','Temperature','The chip thermometer','imu',
  'Read Celsius and distinguish chip temperature from room or body temperature.',
  'Keep the same XIAO + GY-521 circuit. Close the monitor and upload Temperature. No separate temperature module is needed: this sensor is inside the MPU6050.',
  'Could a powered chip be warmer than the room around it?',
  'Type `read` and record the value. Type `test` for a 60-second sample. Leave the board in the same place for a minute and repeat; compare the readings without touching contacts, applying heat, liquids or cooling sprays.',
  'Obtain finite Celsius readings in two observation windows and explain why an unchanged value is a valid result. A difference over time may be small; do not require an artificial temperature increase to pass.',
  'This measures die temperature, not calibrated ambient temperature. It is not a medical thermometer. An I2C response alone does not validate accuracy.',
  'Close the monitor and unplug USB. The next lesson swaps the GY-521 for the OLED; leave the sensor wiring until that setup step.'),
 ('05-oled','Display','Pixels tell a story','oled',
  'Connect a successful I2C exchange to a visible change in screen pixels.',
  'With USB unplugged, remove the GY-521 connections and set it aside. Add only the AITRIP SSD1306 OLED using the table. Reconnect USB after an adult checks the labels.',
  'Can a screen answer a scan but still fail to draw the picture we expect?',
  'Type `awake`, `happy`, `sleep`, `curious`, `surprised`, `love`, and `cheeky`, pressing Enter and pausing after each. Look for eyes, a tiny nose and a different mouth. Curious raises an eyebrow; Surprised gasps; Love has heart eyes; Cheeky winks and sticks out its tongue. Type `clear`, then `awake` again. Type `test` for a 60-second face parade: each face stays for two seconds, then the seven-face sequence repeats. `stop` freezes the last image; it does not erase it.',
  'Observe seven distinct full faces and a cleared screen. Record the actual address (0x3C or 0x3D) and what each command displayed. Have a teammate guess the emotion from the eyes and mouth before reading its name.',
  'Initialization does not prove pixels drew correctly. Match VCC/GND/SDA/SCL labels, not pin position. If missing, inspect only with USB off.',
  'Close the monitor and unplug USB. The next lesson swaps the OLED for the servo system; leave the OLED wiring until that setup step.'),
 ('06-servo','Servo','Signals become movement','servo',
  'Separate driver communication, motor power and observed shaft movement.',
  'With USB off, set the OLED aside. Introduce the PCA9685 driver, one unloaded SG90, and the previously specified regulated 5V supply and connector. Keep the 5V supply unplugged. Start with the four XIAO-to-driver logic wires; check scan/status before adding motor power. Use the complete circuit below to connect channel 0 with both supplies unplugged. No horn, leg or mechanism; all other servo channels empty. Keep the verified OE-to-GND jumper if fitted. On first assembly an adult must verify supply voltage/polarity using the linked first-power check before attaching it.',
  'Does finding 0x40 prove that the servo has power or that its shaft moves?',
  'With the motor supply still unplugged, open the monitor, type `scan`, `status`, `read` and `outputs`. Type `test` to read the preparation guide: it starts no movement. After the adult wiring/power check, plug the supply into the wall and type `run` within 60 seconds. Watch seven repeating eight-second wiggle cycles (three low/high pairs per cycle), followed by four seconds resting at center and automatic pulse-off at 60 seconds. Type `stop` to cancel early. Unplug motor power afterward; type `status` and `outputs`. For a prepared repeat, type `test` again before `run`.',
  'Observe actual shaft movement and automatic completion, then a separate prepared run cancelled by stop that does not resume. All 16 output full-off flags should read 1 afterward. Record YES/NO/NOT SURE; software cannot see the shaft.',
  'Fixed bench range is 1450–1550µs with center 1500µs, ramping for 0.5s and holding for 0.5s at each endpoint. It does NOT use saved Companion calibration. Only use an unloaded servo checked for this range; initial centering may move farther from an unknown start. A supported shaft can be harmed by full sweeps: no automatic limit widening. If binding, continuous buzzing or communications fail, unplug motor power. Software stop is not a hardware power cutoff.',
  'Leave motor power unplugged. Close the monitor and unplug USB before building the team circuit. Keep the tested driver logic; the servo will be set aside during the first integration checks.'),
 ('07-integration','Integration','The team test','team',
  'Combine individually tested functions while identifying which observation proves each one works.',
  'All five standalone function checks should have an observation record. With USB and motor supply unplugged, remove the motor plug and keep the driver logic. Add the OLED using the first diagram; power USB and scan to verify 0x40 plus 0x3C/0x3D. Close the monitor and unplug USB, then add the GY-521 using the second diagram. Reconnect USB. Share four separate breadboard groups, never join different groups. No motor power during these additions.',
  'Will all three addresses remain when the last board joins the circuit?',
  'Type `scan`, `status`, `read`, then `start`. Observe acceleration, rotation rate and chip Celsius together. Tilt and rotate gently, then set the sensor down. Type `stop`. Use `awake`, `happy`, `sleep`, `curious`, `surprised`, `love`, `cheeky` to recheck the OLED. Only after these pass, close the monitor and unplug USB; restore the single unloaded channel-0 servo using the complete diagram, keeping the 5V supply unplugged. Reconnect USB and reopen the monitor before motor power. Type `test`, complete the adult check, connect motor power, then type `run`. Observe all seven full faces cycling every two seconds with the same 60-second servo trajectory; the final four seconds show Sleep. After completion unplug motor power, then `status` and `outputs`.',
  'Record all three addresses, each sensor measurement with units, visible expressions, actual servo motion and final output-off flags. Missing devices must prevent run; do not mark an address reply as a full pass. The integrated motion is scripted, not autonomous walking or sensor-controlled movement.',
  'If an addition fails, unplug USB and motor power before removing only the newest module, then restore the last passing setup. Integration requires all three boards; the individual sketches require only their own hardware.',
  'Unplug motor power, close the monitor, then unplug USB. Keep your five independent results plus the team result. Next use the measurement and fit-gauge guides; no enclosure is assumed complete.')]

def wiring(key, book=False):
    keys=['team-oled','team-all','servo'] if key=='team' else [key]
    atlas=(ROOT/'docs/wiring/README.md').read_text(encoding='utf-8')
    pieces=[]
    for k in keys:
        # Reuse canonical tables from atlas, preserving model-specific notes.
        match=re.search(r'!\['+k.upper()+r' wiring:[^\n]+\n(.*?)(?=\n!\[|\n<!-- wiring:end -->)',atlas,re.S)
        assert match,k
        body=match[1].strip()
        image='images/wiring-'+k+'.png' if book else '../../docs/wiring/images/'+k+'.png'
        pieces.append(f'![{k}: functional connections; follow actual board labels]({image})\n\n'+body)
    return '\n\n'.join(pieces)

def lesson_text(item, book=False):
    slug,sketch,title,wire,objective,state,predict,experiment,evidence,trouble,end=item
    motor=sketch in ['Servo','Integration']
    tables={
      'Basics': ('Read TIME_MS and MESSAGE: the changing timestamp proves the program keeps running.', '   TIME_MS  MESSAGE\n      1200  Hello, teammate!\n      1300  Hello, teammate!'),
      'Accelerometer': ('Watch the **X, Y, Z** columns. All three use m/s², include gravity, and show two decimal places.', '   TIME_MS         X         Y         Z\n      1200      0.12     -0.08      9.79\n      1300      1.45     -0.10      9.65\n      1400      3.10     -0.12      9.28'),
      'Gyroscope': ('Watch the same **X, Y, Z** column order as the acceleration lesson. Here the units are degrees per second: rotation rate, not angle.', '   TIME_MS         X         Y         Z\n      1200      0.12     -0.08      0.19\n      1300     15.45     -0.10      0.65\n      1400     32.10     -0.12      0.28'),
      'Temperature': ('TEMP_C is one chip-temperature measurement in Celsius. It has no X, Y or Z axes. It may barely change during a minute.', '   TIME_MS    TEMP_C\n      1200     25.12\n      1300     25.12'),
      'Display': ('DISPLAY names the expression requested on the OLED. Compare the word with the actual screen; the table cannot prove pixels lit.', '   TIME_MS   DISPLAY\n      1200     AWAKE\n      1300     AWAKE'),
      'Servo': ('CHANNEL stays at 0. PULSE_US is the requested pulse width, not a measured shaft position. Zero means pulse-off requested. Watch the shaft as the numbers change.', '   TIME_MS   CHANNEL  PULSE_US\n      1200         0      1500\n      1300         0      1490'),
      'Integration': ('AX/AY/AZ are acceleration in m/s²; GX/GY/GZ are rotation rate in degrees per second. TEMP_C is chip temperature. PULSE_US is requested pulse width, not measured shaft position; zero means pulse-off requested. Widen the terminal to at least 80 characters.', '   TIME_MS      AX      AY      AZ      GX      GY      GZ  TEMP_C PULSE_US\n      1200    0.12   -0.08    9.79    0.10    0.20   -0.10   25.12     1500')
    }
    column_help, example_rows=tables[sketch]
    text=f'''# {slug[:2]}. {title}

**Program:** `{sketch}` · **Time:** 20–30 minutes · **Partners:** learner operates and explains; adult checks connections and power. Swap the observer/operator roles after the first experiment.

## 1. Mission and success

{objective} Success requires the observations below, not just compilation or `[OK]`.

## 2. Starting setup

{state}

## 3. Wiring and pinout

These custom functional illustrations identify the purchased hardware. They are not exact hole-placement diagrams or Tinkercad simulations. Read printed labels; colors are suggestions. Change wiring only with power unplugged.

{wiring(wire,book)}

## 4. Predict before you run

{predict} Write one prediction before pressing Enter. Discuss what would count as evidence against it.

## 5. Upload from PowerShell

Close any monitor with Ctrl+C. {'Keep the 5V motor supply unplugged throughout upload and monitor opening.' if motor else 'USB powers this lesson. If returning from an assembled robot, disconnect motor power first.'} From the repository root, run ports and identify the XIAO. COM5 is an example, not automatic selection. Replace it in both commands if different.

```powershell
.\\robot.ps1 ports
.\\robot.ps1 upload -Sketch {sketch} -Port COM5 -ServoPowerDisconnected
.\\robot.ps1 monitor -Port COM5 -ServoPowerDisconnected
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

`start` repeats readings every 100 milliseconds for one minute, then ends automatically; `stop` ends them early. `scan` also stops tests. {'`test` prints motor preparation without moving; `run` is the separate physical start after the adult check.' if motor else '`test` starts a 60-second observation window and ends automatically.'} Do not paste preparation and movement commands together. Unknown commands print help guidance. Ctrl+C closes the monitor; it does not send `stop`.

### Read the columns

All timed runs last **60 seconds** unless stopped or faulted. Rows are scheduled every **100ms (10 per second)**. `read` remains one snapshot. `TIME_MS` is board uptime, so compare consecutive timestamps to see the actual spacing; I2C and serial work can delay a row, and missed rows are not replayed in a burst. `help` repeats the command menu; begin another reading run to print a fresh data header.

{column_help}

Example rows to explain the layout (not recorded test evidence):

```text
{example_rows}
```

## 7. Investigate

{experiment}

## 8. Evidence and exit ticket

{evidence}

Write: **prediction → command → actual observation → explanation → next question**. Repeat the observation once. Ask your teammate to explain the result in their own words; if either person is unsure, record NOT SURE and repeat or troubleshoot instead of declaring a pass.

## 9. Troubleshoot one change at a time

{trouble}

## 10. Finish and hand off

{end}
'''
    if not book:
        i=COURSE.index(item)
        if sketch in ['Display','Integration']:
            text+='\n[Preview all seven faces and learn to edit their pixels](../../docs/faces/README.md).\n'
        text+='\n[Course map](../README.md)'
        if i: text+=f' · [Previous lesson]({COURSE[i-1][0]}.md)'
        if i+1<len(COURSE): text+=f' · [Next lesson]({COURSE[i+1][0]}.md)'
        text+=f' · [Source code](../sketches/{sketch}/{sketch}.ino)\n'
        if motor: text+='\n[Supply voltage/polarity measurement](../../docs/wiring/README.md) · [Motor power background](../../docs/WIRING.md)\n'
    return text

def main(update_book=False):
    (LESSONS/'plans').mkdir(exist_ok=True)
    for item in COURSE:
        (LESSONS/'plans'/(item[0]+'.md')).write_text(lesson_text(item),encoding='utf-8')
    index='''# Robot workshop: one lesson, one program

Start here. Each lesson has its own page and standalone sketch. Test one function, record evidence, then build on it. **Companion is not part of this course.** It remains an optional experimental application; the old lessons are archived for reference.

| Lesson | Program to upload | Hardware | What you prove |
|---|---|---|---|
'''
    hardware=['XIAO only','XIAO + GY-521','Same GY-521 wiring','Same GY-521 wiring','XIAO + OLED only','XIAO + PCA9685 + one SG90','All tested modules + one SG90']
    for item,hw in zip(COURSE,hardware):
        index+=f'| [{item[0][:2]}. {item[2]}](plans/{item[0]}.md) | `{item[1]}` | {hw} | {item[4]} |\n'
    index+='''
The MPU6050 contains three measuring functions: acceleration, rotation rate and chip temperature. Lessons 2–4 use the same physical board but different programs; do not buy three sensors.

Every page follows **mission → starting setup → diagram/pinout → prediction → upload → monitor commands → investigation → evidence → troubleshooting → handoff**. This uses explicit modeling, guided practice, retrieval questions and a check for understanding before the next lesson. Allow 20–30 minutes and repeat any uncertain observation.

## Two places to type

At `PS ...>` run `.\\robot.ps1 ...` commands. After `Connecting to COM...`, type the short robot commands and press Enter. `help` redisplays instructions. `read` gives one result, `start` streams, `stop` ends activity. `test` runs a timed non-motor observation or prints motor preparation; the motor lessons require a separate `run`. **No automatic motion on boot.** Ctrl+C closes the monitor; it does not stop a motor by itself.

## Starting from an assembled robot

Unplug the motor supply and USB before changing wires. Follow lesson 1's bare-board setup, then each page's keep/remove/add instructions. Do not unplug modules while powered. A returning learner may select a standalone function directly using its stated setup. Keep motor power off before any upload, reset or monitor opening.

## Teacher and maintainer notes

- [Setup](../docs/SETUP.md), [command reference](../docs/COMMANDS.md), [experiment record](../templates/EXPERIMENT.md).
- The canonical page source is `scripts/build_lessons.py`; regenerate after `scripts/build_wiring.py`, leave the book unchanged until the final book pass (`--book`).
- Each sketch chooses devices at compile time. Shared command handling and bounded motor timing live in `lessons/lib/RobotLessons/src/`; a sensor sketch neither includes the PCA driver nor writes motor outputs.
- For Arduino IDE, install the repository's `lessons/lib/RobotLessons` as a custom library and the pinned dependencies from `toolchain.psd1`. The repository PowerShell workflow supplies the library automatically.
- [Legacy lessons](legacy.md) preserve historical experiments, not the recommended sequence. Hello/Scanner/Face/Motion/Companion remain buildable for compatibility.
- After lesson 7: [measure parts](../docs/MEASUREMENTS.md), [print fit gauges](../cad/README.md), then [plan improvements](../docs/PLAN.md).

## Sensor references

- [MPU6050 product specification](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet.pdf): on-chip temperature measures the die.
- [Adafruit Arduino example](https://learn.adafruit.com/mpu6050-6-dof-accelerometer-and-gyro/arduino): acceleration m/s², gyro rad/s, temperature Celsius. Our gyro lesson converts rad/s to deg/s.
'''
    (LESSONS/'README.md').write_text(index,encoding='utf-8')
    if not update_book:
        print('Generated seven lesson pages and course index. Book unchanged; use --book only for the final book pass.')
        return
    p=ROOT/'docs/book/manuscript.md'; text=p.read_text(encoding='utf-8-sig')
    start=text.index('## Appendix A.'); end=text.index('## Appendix B.',start)
    appendix='## Appendix A. The seven-lesson workshop\n\nUse one program per lesson. The repository lesson index has one page per experiment. PowerShell commands assume the repository root; COM5 is an example.\n\n'
    for item in COURSE:
        page=lesson_text(item,True)
        page=re.sub(r'^## ', '#### ',page,flags=re.M)
        page=re.sub(r'^# ', '### ',page,flags=re.M)
        appendix+=page+'\n'
    text=text[:start]+appendix+text[end:]
    bonus=text.find('## Bonus lab:')
    if bonus!=-1: text=text[:bonus]
    (ROOT/'docs/book/manuscript.md').write_text(text.rstrip()+'\n',encoding='utf-8')
    print('Generated seven standalone lesson pages, course index and book workshop.')

if __name__=='__main__':
    import sys
    main(update_book='--book' in sys.argv)
