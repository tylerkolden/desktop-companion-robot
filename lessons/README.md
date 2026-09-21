# Learn robotics one experiment at a time

Wiring reference: [full pinout atlas and verification limits](../docs/wiring/README.md). Use the stage matching your lesson; do not connect every pictured module at once.

Use the [setup guide](../docs/SETUP.md) first. Run commands from the repository root in PowerShell, or ask Codex to run them. Each standalone sketch is in `lessons/sketches/<Name>/<Name>.ino`, so Arduino IDE users can open it directly too.

Read [command meanings](../docs/COMMANDS.md) before uploading. Every upload replaces the previous program. `COM7` below is a placeholder for your identified XIAO. Before each session: have an adult present and identify the hardware listed in that lesson. Close the monitor and unplug USB before changing connections. These lessons start with a bare XIAO, not a previously assembled robot.

## Two places to type: computer or robot?

The same terminal window has two modes. **Check which mode you are in before typing.**

| What you see | Who receives your typing? | What to type |
|---|---|---|
| `PS C:\...>` | Your computer (PowerShell) | Commands beginning with `.\robot.ps1` |
| `Connecting to COM... Press CTRL-C to exit.` followed by robot messages | Your robot (serial monitor) | For Companion: `help`, `scan`, `status`, and the lesson's commands |

**Enter sends a command. Ctrl+C closes the monitor and returns to PowerShell.** Click inside the terminal before typing. You might not see your typing echoed, and sensor messages may appear while you type. Finish the command and press Enter once. Do not type the backticks, the word `text`, or the `PS ...>` prompt shown in examples.

Uploading puts the program on the board; **it does not open the monitor**. Run the lesson's separate monitor command afterward. Wait for `Connecting to...`, then send the robot commands one at a time. A monitor command such as `scan` is different from the PowerShell command `.\robot.ps1 status ...`, which opens the port for a brief report and closes it again.

Hello, Scanner, Face and Motion are standalone teaching programs: watch their output as their lessons describe. The interactive commands below belong to **Companion**, introduced in 5A; they are not available in those earlier sketches.

**If something seems wrong:**

- `scan is not recognized` means you typed at PowerShell. Open the monitor first.
- No response? Press Enter. In Companion, type `help` and Enter to redisplay the menu.
- Port access denied? Close other serial monitors; only one program can use the port.
- Need to upload again? Press Ctrl+C first and follow that lesson's power preparation.


## How to follow a lesson

Start with the stated hardware, predict the result, perform one experiment, and record what you observed. Each transition tells you what to keep, remove or add. Book chapter numbers are reading references, not lesson numbers.

Close the serial monitor before each upload. An upload replaces the previous program; it does not add features to it. The command option `-ServoPowerDisconnected` is a required tool safety acknowledgement. For the USB-only experiments, confirm that USB is the only power connection before using it.

## 0. Identify before overwriting

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![USB wiring: labelled endpoints and separate power paths](../docs/wiring/images/usb.png)

The pinout for this stage is USB-C socket only: **all XIAO header pins are unused**.

| From / pin | To / pin | Meaning |
|---|---|---|
| Computer USB port | XIAO USB-C socket | Data cable; all headers empty |

<!-- wiring:end -->

**Question:** is this board already running Companion?

**Predict:** if it is, help/status will produce recognizable text. An unknown or silent program is inconclusive.

Run `./robot.ps1 ports` before and after connecting the bare XIAO by data cable. Read [the existing-program check](../docs/COMMANDS.md#check-an-existing-program). Use the actual new port:

```powershell
.\robot.ps1 status -Port COM7 -ServoPowerDisconnected
```

**Pass:** identify a specific port and describe the response, without assuming a board name proves its firmware. **Record:** port and text output. **If stuck:** check data cable and competing monitors. Do not upload just to answer a port question.

## 1. Hello: a program lives on a board

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![USB wiring: labelled endpoints and separate power paths](../docs/wiring/images/usb.png)

The pinout for this stage is USB-C socket only: **all XIAO header pins are unused**.

| From / pin | To / pin | Meaning |
|---|---|---|
| Computer USB port | XIAO USB-C socket | Data cable; all headers empty |

<!-- wiring:end -->

**Parts:** XIAO and USB data cable only. **Book:** chapters 4 and 12.

**Concept:** source code is the recipe; compilation translates it; uploading stores the translated program; the serial console lets it talk back.

**Predict:** what will the following sketch say, and how often? Open [Hello.ino](sketches/Hello/Hello.ino) and identify the message and delay.

```powershell
.\robot.ps1 build -Sketch Hello
.\robot.ps1 upload -Sketch Hello -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

**Expected:** `Hello from Andrew! This is the Hello lesson.` repeats about once a second. Ctrl+C closes the monitor. A successful compile alone is not the pass condition; the actual message must arrive from the board.

**Verification record:** capture the actual greeting, count approximately ten message intervals, and record the timing. After a USB reconnect, confirm the same program starts again.

**Change one thing:** predict the effect of changing `1000` to `2000`; close monitor, edit, upload, observe, and explain. Then restore the original or commit your intentional change. **If stuck:** verify selected port, USB CDC board setting, and cable. **Record:** exact message, observed interval, and sketch revision.

## 2. Scanner: devices have addresses

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![OLED wiring: labelled endpoints and separate power paths](../docs/wiring/images/oled.png)

Functional pinout: VCC, GND, SDA, SCL. The supplied display photo has **GND / VCC / SCL / SDA** across its top header when viewed from the front. Our illustration groups wires by function, not that physical order.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | AITRIP SSD1306 OLED VCC | 3.3V logic |
| XIAO GND | AITRIP SSD1306 OLED GND | Shared ground |
| XIAO D4 / GPIO6 | AITRIP SSD1306 OLED SDA | I2C data |
| XIAO D5 / GPIO7 | AITRIP SSD1306 OLED SCL | I2C clock |

<!-- wiring:end -->

**Parts:** XIAO + OLED only. **Book:** chapters 9–10.

**Adult wiring check with USB off:** XIAO 3V3 → module VCC; GND → GND; D4/GPIO6 → SDA; D5/GPIO7 → SCL. Inspect labels, not the order in a picture. Use [the wiring specification](../docs/WIRING.md).

**Coming from lesson 1:** close the monitor, unplug USB, and add only the OLED using this lesson's diagram and pin table. Check the four wires, reconnect USB, then upload Scanner.

**Predict:** the OLED normally acknowledges at 0x3C or 0x3D. These are addresses, not pin numbers. Save the motion sensor for lesson 4.

```powershell
.\robot.ps1 upload -Sketch Scanner -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

**Expected:** scan text repeats about every three seconds, including actual responding addresses. **Pass:** identify the address of one connected module. **Experiment:** unplug USB, remove that module, reconnect and observe the difference. Never remove jumpers while powered. After that experiment, unplug USB again, restore the OLED using the diagram, and reconnect USB; leave the OLED installed for lesson 3. **If stuck:** inspect crossed SDA/SCL, missing ground, and breadboard contact groups with power off. **Record:** module, observed address, and what a scan cannot prove (for example, whether the screen can actually draw an image).

## 3. Face: pixels make expressions

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![OLED wiring: labelled endpoints and separate power paths](../docs/wiring/images/oled.png)

Functional pinout: VCC, GND, SDA, SCL. The supplied display photo has **GND / VCC / SCL / SDA** across its top header when viewed from the front. Our illustration groups wires by function, not that physical order.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | AITRIP SSD1306 OLED VCC | 3.3V logic |
| XIAO GND | AITRIP SSD1306 OLED GND | Shared ground |
| XIAO D4 / GPIO6 | AITRIP SSD1306 OLED SDA | I2C data |
| XIAO D5 / GPIO7 | AITRIP SSD1306 OLED SCL | I2C clock |

<!-- wiring:end -->

**Parts:** XIAO + OLED, same checked wiring. **Book:** chapters 5 and 13.

**Coming from lesson 2:** keep the OLED wiring as shown above. No new hardware is needed. Close the monitor before uploading Face. Scanner is replaced by Face; the screen now draws eyes instead of merely answering an address scan.

**Concept:** coordinates locate pixels, drawing changes a memory image, and sending that image changes the screen. A timer alternates expressions without a long delay in the main loop.

**Predict:** inspect [Face.ino](sketches/Face/Face.ino). What changes when the height is 4 instead of 24?

```powershell
.\robot.ps1 upload -Sketch Face -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

**Verification record:** record the scanned address and observe at least ten awake/sleepy changes. After changing one eye coordinate, confirm the expected direction; restore the original and confirm both expressions again.

**Expected:** eyes alternate roughly every second; serial text says sleepy/awake. **Pass:** observe both distinct expressions. **Challenge:** change one eye's x coordinate slightly; predict the direction before uploading. Keep rectangles within the 128×64 display. **If stuck:** return to Scanner with the same OLED wiring; address detection and drawing are separate checks. Close the monitor and upload Face again afterwards to restore the eyes. **Record:** code change and visual result, not just compile success.

## 4. Motion: gravity is a clue

<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![IMU wiring: labelled endpoints and separate power paths](../docs/wiring/images/imu.png)

Functional pinout: VCC, GND, SCL, SDA, XDA, XCL, AD0, INT. Leave XDA/XCL/INT open; verify AD0 default through scanning. Exact delivered header order is unconfirmed. Check regulator/pull-ups at 3.3V; do not substitute 5V to fix detection.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | GY-521 / MPU6050 VCC | 3.3V logic |
| XIAO GND | GY-521 / MPU6050 GND | Shared ground |
| XIAO D4 / GPIO6 | GY-521 / MPU6050 SDA | I2C data |
| XIAO D5 / GPIO7 | GY-521 / MPU6050 SCL | I2C clock |

<!-- wiring:end -->

**Parts:** XIAO + GY-521 / MPU6050 motion sensor only, with adult-checked 3.3V wiring. **Book:** chapter 6.

**Coming from lesson 3 — change the experiment:**

1. Close the serial monitor and unplug the XIAO USB cable.
2. Remove all four OLED connections and set the display aside. Keep the XIAO and reuse the jumpers if convenient.
3. Introduce the GY-521 sensor now. Follow this lesson's IMU diagram and pin table: identify each printed label instead of copying the OLED's header order.
4. Have an adult check the four connections, then reconnect USB. Recheck the detected port if it changed.
5. Upload Motion and open the monitor using the commands below. This replaces Face. **Expect acceleration numbers in the monitor. The OLED is set aside for this experiment.**

The GY-521 is a **sensor**: it measures motion rather than creating it. You gently tilt the board; the program reports the resulting acceleration readings.

**Concept:** accelerometer readings have axes and units. A stationary supported sensor can report gravity; it need not read zero. The gyroscope measures rotation rate, but this lesson prints only acceleration.

**Predict:** with sensor +Z pointing up, Z should be around +9.8m/s². No claim of exact calibration is implied.

```powershell
.\robot.ps1 upload -Sketch Motion -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

**Verification record:** hold the sensor still with +Z up, then turn it over so +Z points down. Expect Z to change from roughly +9.8 to -9.8 m/s². Try +X and +Y up separately; the corresponding axis should become roughly +9.8 while the others are near zero. Record several readings in each position, including variation. These are qualitative orientation checks, not precision calibration.

**Expected:** three labelled acceleration values every half-second. Slowly rotate the secured board without stressing wires. **Pass:** describe a repeatable change in axes when orientation changes. **Challenge:** predict another orientation, then test. Do not shake, drop, or tug the assembly. **If stuck:** leave the sensor wiring unchanged and use Scanner to look for 0x68 or 0x69; check physical axis markings. Close the monitor and upload Motion again afterwards, or you will still see scan results instead of acceleration. **Record:** orientation, readings, units, and noise/variation.

## 5. Verify individually, then combine

The OLED has passed its address and drawing tests. The GY-521 has passed its acceleration test. Next, verify the new driver by itself. Verify its communication in 5A and its output with one unloaded motor in 5B. Only then combine the three tested modules in 5C. Keep your recorded addresses and observations nearby.

## 5A. Test the driver by itself

<!-- wiring:start -->
**Starting state:** lesson 4 left XIAO + GY-521 running Motion. **New part:** the PCA9685 driver generates timed control signals. First test its logic communication alone, using the same four XIAO connections you already learned.

### Wiring visuals and pinouts

**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![PCA wiring: labelled endpoints and separate power paths](../docs/wiring/images/pca.png)

The green two-screw block is a separate power input: one terminal is + / V+, the other is - / GND. It stays empty during this communication-only test; the driver electronics receive power through VCC. Follow the board markings, not a guessed left/right position. Functional pinout: VCC = logic 3.3V; V+ = separate output-power input; GND = common ground; SDA/SCL = I2C; OE = active-low output enable. Exact clone header order is unconfirmed. Verify OE pull-down; ground OE only if floating. Use only VCC, GND, SDA and SCL for this communication test. Leave V+ and the numbered output connectors empty.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | Teyleten PCA9685 VCC | 3.3V logic |
| XIAO GND | Teyleten PCA9685 GND | Shared ground |
| XIAO D4 / GPIO6 | Teyleten PCA9685 SDA | I2C data |
| XIAO D5 / GPIO7 | Teyleten PCA9685 SCL | I2C clock |

<!-- wiring:end -->

1. Close the monitor and unplug USB. Remove the GY-521 connections and set it beside the previously tested OLED.
2. Connect only the PCA9685 logic pins to the XIAO, using the diagram and table. Leave its V+ terminal and numbered output connectors empty. Reconnect USB and confirm the port.
3. Upload Scanner using the commands below. Expect the driver's default address `0x40`; `0x70` may also respond as its all-call address. Two responding addresses do not necessarily mean two boards.

```powershell
.\robot.ps1 upload -Sketch Scanner -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

4. Record the actual addresses, then close the monitor. With the wiring unchanged, upload Companion and open its console:

```powershell
.\robot.ps1 upload -Sketch Companion -Port COM7 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected
```

5. Send `help`, `scan` and `status` using the monitor instructions below.

### Where to type the board commands

After `Connecting to COM5. Press CTRL-C to exit.` (your port number may differ), the terminal is the **serial monitor**. Click in that same terminal, type one command, and press **Enter** to send it to the XIAO:

```text
help
scan
status
```

Send each line separately and read its response. Do not include backticks or type these at the `PS ...>` PowerShell prompt. Your typing may or may not be echoed. Companion waits for Enter before acting. Unlike Scanner, it does not repeat scans automatically.

You may miss the beginning of the startup message while the monitor connects. Type `help` to print the command list again; no upload is needed. Press **Ctrl+C** when finished to return to PowerShell. Close the monitor before another upload or another program opens the same port.

**Expected for this driver-only checkpoint:**

| Response | What it tells you |
|---|---|
| `scan` lists `0x40` | The PCA9685 acknowledges communication. `0x70` may also appear as its all-call address. |
| `[OK] Motor driver 0x40` | Companion initialized the driver; observe its output in the next test. |
| `[X] OLED screen` and `[X] Motion sensor` | Expected: those modules are set aside. |
| `Timed test: off` and `Calibration channel: none` | No timed test or calibration channel is active. |

For technical numbers, type `details` and press Enter. The four `low / center / high` rows describe calibration settings; they are not detection results or proof of movement. `storage=1` means settings storage opened successfully. `upright=0 sample=0` is expected without the sensor.

**Pass:** the driver responds on its own and Companion recognizes it. This verifies communication and initialization, not its output signals under load. **If stuck:** resolve the failure here before combining modules. **End state:** XIAO + PCA logic, running Companion; OLED and GY-521 set aside.

## 5B. Introduce one servo and its power supply

<!-- wiring:start -->
**Starting state:** only the XIAO and independently tested PCA9685 logic are connected, with Companion running. The tested OLED and GY-521 are set aside. **New parts:** one SG90 servo motor, a regulated 5V wall power supply, its low-voltage barrel-to-screw-terminal connector, prepared power wires, and the multimeter. The servo turns its shaft in response to the driver signal. The separate supply provides motor power; USB continues powering the XIAO. Identify these parts first. The wall supply has not been connected yet.

### Wiring visuals and pinouts

**Before assembling these connections: close the monitor and unplug the XIAO USB cable. Keep the newly introduced 5V supply unplugged from the wall until the voltage-check step. On a repeat session, unplug it before changing wires.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![SERVO wiring: labelled endpoints and separate power paths](../docs/wiring/images/servo.png)

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

![METER wiring: labelled endpoints and separate power paths](../docs/wiring/images/meter.png)

Probe-jack pinout is functional, not the physical socket position of every meter variant. Select DC voltage before probing. Verify barrel size and polarity against the actual adapter; do not assume center-positive from a generic picture.

| From / pin | To / pin | Meaning |
|---|---|---|
| Black probe / COM jack | Adapter negative output | Voltage check only |
| Red probe / V-ohm jack | Adapter positive output | DC volts; never current mode |

<!-- wiring:end -->

**Start with the [guided motor lab](../docs/MOTOR-LAB.md): type `motor`, read the preparation steps, then type `go` only after the adult wiring/power check. The guide identifies channel 0 and explains what to observe.**

**Why a separate supply?** A motor needs more current than the controller's logic connections are intended to deliver. The driver uses VCC for logic and V+ for motor power. Trace both paths in the diagrams before connecting anything.

Follow [One servo, no leg attached](../docs/FIRST-POWER-ON.md#one-servo-no-leg-attached) in order. It contains the voltage check, connection sequence and first commands. Use only one unloaded motor on channel 0. Companion verified in checkpoint 5A remains installed; do not upload another sketch for this step.

**Timed movement test:** follow the supply-check and power-on sequence in the linked guide, then type `motor` in the already-open monitor. Follow its instructions and type `go` when prepared. The firmware requests the saved center for 500ms, then center +10 microseconds for 500ms, then requests pulse-off. Only channel 0 is used. With untouched defaults, the requests are 1500us and 1510us. This is a one-second test made of repeating 50Hz control pulses, not a one-second HIGH pulse.

Watch the unloaded shaft; do not hold or obstruct it. Starting from an unknown shaft position can cause a larger initial move to center. The +10us movement may be very small or hidden by the servo's deadband. If you cannot see it, record **inconclusive**, not passed. Do not increase travel automatically: inspect power and orientation, then use the adult-supervised calibration procedure to investigate. Do not attach a leg for visibility.

After automatic completion, enter `status`: expect `Timed test: off` and `Calibration channel: none`. These report software state, not measured shaft position. For a separate repeat, enter `test 0` then `stop` before it completes and verify the test does not resume. A PCA9685 can retain pulses if communication or the controller fails; unplug the 5V supply to remove motor power. The one-second duration is a software deadline checked by the loop, not a hardware cutoff.

**Predict:** a small change in the command should produce a small shaft movement. **Pass:** controlled unloaded movement, correct connector orientation, known supply polarity, and an explained difference between stopping control pulses and removing electrical power. **If stuck:** unplug the now-introduced 5V supply and inspect the circuit with the adult. **Record:** channel, command, observed movement and measured supply voltage. End by unplugging the 5V supply, closing the monitor and unplugging USB.

If the motor has not visibly moved and the output number is uncertain, use the [all-channel finder](../docs/MOTOR-LAB.md#find-an-unknown-channel-without-moving-the-plug): `hunt` explains the test; `hunt start` checks all 16 outputs in sequence with one unloaded motor. Record the result before proceeding.

## 5C. Combine the three verified modules

**Mission: make the parts work as a team!** You already tested each part on its own. Now keep the driver connected, add the screen, test, then add the motion sensor and test again. No new program upload is needed: keep Companion installed.

### Get ready: park the motor

At the end of 5B you had a XIAO, driver and one motor. Unplug the 5V wall supply, press **Ctrl+C** to close the monitor, and unplug USB. With both supplies off, remove the motor's three-wire plug and set the motor aside. Keep the four XIAO-to-driver logic connections. If you fitted an **OE-to-GND** jumper in 5B, keep it. The wall supply stays unplugged throughout 5C; its low-voltage leads may remain attached to the green block.

### Four meeting places for wires

The boards share four connections. Think of them as four separate meeting places on the breadboard:

- **3V3 → VCC:** power for each board's electronics.
- **GND → GND:** their shared electrical reference.
- **D4 → SDA:** the wire that carries messages.
- **D5 → SCL:** the timing wire that keeps those messages in step.

The last two together are **I2C** (say “eye-squared-see”). Each board has its own address, so the XIAO can choose who receives a message.

**How do several wires fit on one XIAO pin?** Run one wire from that pin to a connected breadboard group. The matching wire from each module joins another hole in that same group. Use a *different, electrically separate group* for each of the four connections. An adult should confirm the breadboard's connected holes with power off; do not assume a whole printed row or a split power rail is connected. Never squeeze several plugs onto one pin.

If your driver currently connects directly to the XIAO, move those four connections onto the four groups, one at a time, following the table below; leave the screen disconnected for this small change. Reconnect USB, open the monitor, and check `scan` still finds `0x40`. Then close the monitor and unplug USB again. That gives you a checked starting point before adding anything.

**Read the pictures:** follow one colored line at a time. A dot means wires join. A crossing without a dot does not. Match the board's printed labels, not the position of a pin in the picture. These are functional illustrations, not exact breadboard-hole layouts or Tinkercad simulations.

## 5C.1 Add the screen

<!-- wiring:start -->
### Wiring visuals and pinouts

**5C setup:** keep the 5V wall supply unplugged and the motor plug removed. Close the monitor and unplug USB before each wiring change. Keep the tested PCA logic connections and any OE-to-GND jumper from 5B. These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![TEAM-OLED wiring: labelled endpoints and separate power paths](../docs/wiring/images/team-oled.png)

Models: Seeed XIAO ESP32C3, Teyleten PCA9685, AITRIP SSD1306 OLED. Read the printed pin labels on each board. Each colored line means one connected breadboard group, not a physical row or rail. The picture rearranges pins to make the four connections easy to follow; it is not a top-view placement guide. Colors are suggestions. Use the XIAO D4 pin for SDA and D5 for SCL.

| From / pin | To / pin | Meaning |
|---|---|---|
| 3V3 | PCA + OLED VCC | Power for the electronics |
| GND | PCA + OLED GND | Shared ground |
| D4 / GPIO6 | PCA + OLED SDA | Messages |
| D5 / GPIO7 | PCA + OLED SCL | Timing |

<!-- wiring:end -->

**Predict:** we should find two devices now: the driver and the screen.

With USB unplugged, keep the driver connected. Add the OLED's four wires using the diagram and table above. Connect one wire, trace it with your finger, then move to the next. The OLED's **VCC joins the 3V3 group**, not the 5V motor supply. Leave the motion sensor on the table for now.

Reconnect USB. At the PowerShell prompt, run this command (replace COM5 if your board uses another port):

```powershell
.\robot.ps1 monitor -Port COM5 -ServoPowerDisconnected
```

After **Connecting to COM5**, you are inside the monitor. Type each line below and press **Enter** after each one. These are messages to the robot, not PowerShell commands:

```text
scan
status
face happy
face awake
```

Pause between the face commands so you can see each expression.

| Look for | What it means |
|---|---|
| `0x40` and `0x3C` (or `0x3D`) in the scan | Driver and screen both answer |
| `[OK]` for driver and OLED | Companion initialized both |
| `[X]` for motion sensor | Correct for now: it is still on the table |
| The face changes on the screen | We checked real behavior, not just an address |

The driver may also answer at `0x70`; that is not another board. **Checkpoint:** record the two addresses and the expressions you saw. Continue only when both parts pass.

## 5C.2 Add the motion sensor

<!-- wiring:start -->
### Wiring visuals and pinouts

**5C setup:** keep the 5V wall supply unplugged and the motor plug removed. Close the monitor and unplug USB before each wiring change. Keep the tested PCA logic connections and any OE-to-GND jumper from 5B. These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![TEAM-ALL wiring: labelled endpoints and separate power paths](../docs/wiring/images/team-all.png)

Models: Seeed XIAO ESP32C3, Teyleten PCA9685, AITRIP SSD1306 OLED, and GY-521 / MPU6050. Read the printed pin labels on each board. Each colored line means one connected breadboard group, not a physical row or rail. The picture rearranges pins to make the four connections easy to follow; it is not a top-view placement guide. Colors are suggestions. Use the XIAO D4 pin for SDA and D5 for SCL.

| From / pin | To / pin | Meaning |
|---|---|---|
| 3V3 | PCA + OLED + GY-521 VCC | Power for the electronics |
| GND | PCA + OLED + GY-521 GND | Shared ground |
| D4 / GPIO6 | PCA + OLED + GY-521 SDA | Messages |
| D5 / GPIO7 | PCA + OLED + GY-521 SCL | Timing |

<!-- wiring:end -->

**Predict:** the two working parts should keep working, and one new address should appear.

Press **Ctrl+C**, then unplug USB. Keep the driver and screen wires exactly as they are. Add the GY-521's **VCC, GND, SDA and SCL** to the same four groups shown above. Keep its other pins as they were in lesson 4; do not add wires to XDA, XCL or INT.

Reconnect USB and run the same PowerShell monitor command from 5C.1. In the monitor, type `scan`, then `status`, pressing Enter after each. Try `face happy` and `face awake` again. Gently tilt the sensor by its edges and watch the repeating `accel m/s2:` line: its three numbers should change as you tilt it. Set it down and let the numbers settle. Those numbers describe acceleration along three directions; gravity is part of what the sensor measures.

| Team member | Expected address | Real-world check |
|---|---|---|
| PCA9685 driver | `0x40` | Still recognized; movement was tested in 5B |
| OLED screen | `0x3C` or `0x3D` | Changes expression on command |
| GY-521 motion sensor | `0x68` or `0x69` | Acceleration readings change when tilted |

**Mission complete:** all three show `[OK]`, the screen changes, and sensor readings respond to tilt. Write down the addresses and what you observed. We are checking the electronics together; the motor stays set aside and the 5V wall supply stays unplugged.

**If a teammate disappears:** close the monitor and unplug USB. Remove only the module you just added, then reconnect USB and repeat the last passing test. Inspect the new module's four connections with power off before trying again. This is why we added one part at a time: we know where to investigate!

**Explain it to Andrew:** “Why do three boards share SDA and SCL without all having the same address?” Let him trace each of the four groups and explain its job.

## 5D. Andrew's tiny talent show

<!-- wiring:start -->
**5D setup:** use the familiar SG90 and 5V supply from 5B. The screen and sensor remain connected as shown in 5C; this diagram highlights the motor connection.

### Wiring visuals and pinouts

**Keep the three-module logic wiring from 5C. Unplug USB and the 5V wall supply before restoring the single channel-0 servo connection shown below. Open the monitor with motor power off.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![SERVO wiring: labelled endpoints and separate power paths](../docs/wiring/images/servo.png)

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

<!-- wiring:end -->

**Mission:** make a face and a motor tell the same story! Finish 5C first. This is a scripted personality performance, not real feelings or a walking robot.

**Starting state:** the three logic modules passed together; motor power is unplugged and the single tested SG90 is set aside. Keep the combined logic wiring from 5C. Use the [complete servo diagram and pin table from 5B](#5b-introduce-one-servo-and-its-power-supply) to restore the one unloaded servo on channel 0 with both supplies unplugged. Keep any OE-to-GND jumper. No horn, leg or mechanism is attached. All other motor channels stay empty. Have an adult check the connection and keep the power plug accessible.

Upload the updated Companion while motor power remains unplugged, then open the monitor. Opening the monitor can reset the controller: do this before powering the motor. Use your identified port in place of COM5:

```powershell
.\robot.ps1 upload -Sketch Companion -Port COM5 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM5 -ServoPowerDisconnected
```

### Your turn: talk to the robot

Wait until you see `Connecting to COM5. Press CTRL-C to exit.` The PowerShell prompt is now gone. **Stay in this same terminal.** Type these commands separately, pressing Enter and reading the reply after each:

```text
scan
status
show
```

Expect the three device addresses, three `[OK]` readiness results, and `ANDREW'S TINY TALENT SHOW`. Nothing should move yet. All three boards must be ready. `show` prints the guide and requests pulses off; it does not move the motor. Only use this performance after physically checking channel 0's center +/-50us range with the supervised calibration procedure in the motor lab. The owner's earlier 1450–1550us test showed movement, but other builders must check their own servo. Existing narrower limits reduce travel automatically; this show never widens limits.

After the adult's 5B power check, reconnect the 5V supply without changing wiring. Keep the monitor open. In that same terminal, type this command and press Enter once:

```text
show start
```

Expect `CURTAIN UP!`, followed by four numbered scene announcements. Watch both the face and the shaft:

| Time | Story | Motor request |
|---|---|---|
| 0–3 seconds | Hello, Andrew! Open eyes | One smooth sway |
| 3–6 seconds | What's over there? Curious angled eyes | One smooth sway |
| 6–9 seconds | Happy dance! Smiling eyes | Three quicker wiggles |
| 9–12 seconds | Time for a nap. Sleepy eyes | Return to center and hold |
| Finish | Curtain down | All pulses requested off |

Only channel 0 moves. Travel is at most +/-50us from its configured center, within existing limits. This is more visible than the old +10us test, not a full-range sweep or continuous rotation. Initial centering may move farther from an unknown starting position.

Type `stop` and Enter to cancel at any point. Sensor/driver communication failures stop the show; it also checks whether the screen still answers. These are software checks, not a hardware emergency stop: unplug the motor supply if the motor binds, buzzes continuously or behaves unexpectedly. Sensor tilt does not steer or cancel this unloaded bench show. Never use it with an assembled mechanism.

### Check the finish

Wait for `CURTAIN DOWN`. In the still-open monitor, send each line separately:

```text
status
diagnose
```

**Verify:** observe the four expressions, synchronized shaft movement, and automatic finish. Expect `Personality show: off`, `Calibration channel: none`, and `full-off=1` for every channel in the diagnostic report. These reports verify software/register state, not physical position. Unplug motor power before restarting the monitor or changing connections. For a prepared repeat, type `show`, then `show start` again.

**Done for today?** Unplug the 5V motor supply, press Ctrl+C to close the monitor, then unplug USB. Ctrl+C closes the computer connection; it is not the robot's `stop` command and does not remove motor power.

**Why did the eyes close?** After 30 seconds without a qualifying interaction, Companion shows sleepy eyes. The motion sensor still runs. `face awake` wakes the face. `SERVO RESTING` describes motor control, not sensor health.

**Teach it back:** the controller follows a timed sequence called a state machine. One scene selects both an expression and a motion pattern. Ask Andrew which scene looked happiest and why. Record what actually moved, not just what the program printed.

## 6. Measure, print, revise

**Prerequisites:** actual parts available, printer supervision. **Book:** chapter 16. Use [measurement sheet](../docs/MEASUREMENTS.md) and [fit-gauge instructions](../cad/README.md).

**Predict:** choose which gauge clearance should fit. Print at 100% scale with the actual nozzle/material profile, compare without force, and record the result. **Pass:** measured dimensions and observed fit. A manifold STL check is not a physical pass. Full enclosure and leg design remain future work; do not invent final fastener sizes.

## 7. Improve one thing and explain it

**Book:** chapters 17–20. Start from a working baseline. Create one issue-sized change: eye expression, helpful serial message, or clarified instruction. Write a pass condition. Change one thing, test, and record a limitation. Use [the contribution guide](../CONTRIBUTING.md).

**Graduation evidence:** explain how a sensor reading becomes a face response; identify the separate servo power path; show a useful failed experiment and what it taught you. Walking and cordless power are not graduation requirements.
