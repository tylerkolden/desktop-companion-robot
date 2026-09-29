# Workshop command reference

Wiring reference: [full pinout atlas and verification limits](wiring/README.md). Use the stage matching your lesson; do not connect every pictured module at once.

Follow [one lesson at a time](../lessons/README.md). Commands below are for the new standalone course, not the archived Companion application.

## PowerShell: talk to the computer

At the `PS ...>` prompt, from the repository root:

```powershell
.\robot.ps1 setup
.\robot.ps1 ports
.\robot.ps1 build -Sketch Accelerometer
.\robot.ps1 upload -Sketch Accelerometer -Port COM5 -ServoPowerDisconnected
.\robot.ps1 monitor -Port COM5 -ServoPowerDisconnected
```

Setup is needed once (and after intentional dependency changes); build does not open a port. Upload also builds, replaces the board's program and requires motor power physically disconnected. Replace COM5 with the detected XIAO port. Run each line separately. Close any monitor with Ctrl+C before upload or another port-opening command. The default build sketch is Basics.

| Sketch | Standalone purpose |
|---|---|
| Basics | USB and controller |
| Accelerometer | Acceleration, m/s² |
| Gyroscope | Rotation rate, deg/s |
| Temperature | MPU6050 chip temperature, °C |
| Display | OLED expressions |
| Servo | Driver + one unloaded channel-0 motor |
| Integration | All verified functions together |

## Serial monitor: talk to the robot

After `Connecting to COM...`, stay in the same terminal and type one command followed by Enter. No `PS` prefix, no backticks. Startup messages can be missed: type help. Input is case-sensitive lowercase. No program starts a motor automatically.

| Command | All new lesson sketches |
|---|---|
| `help` | Redisplay this lesson's menu |
| `status` | Only this lesson's required devices and activity state |
| `scan` | Stop current activity, then list fresh I2C replies |
| `read` | One measurement/report; no motor start |
| `start` | Repeat measurements/reports every 100ms for 60 seconds; no motor start |
| `stop` | End streaming/test; motor sketches request outputs off |
| `test` | 60-second observation in lessons 1–5; preparation only in lessons 6–7 |
| `run` | Lessons 6–7 only: after test and adult preparation, one 60-second motor sequence |
| `outputs` | Lessons 6–7 only: stopped driver output-register check |
| `awake`, `happy`, `sleep`, `curious`, `surprised`, `cool`, `cheeky`, `clear` | Display and Integration only; [face gallery](faces/README.md) |

Motor preparation expires after 60 seconds; repeat test to prepare again. A run consumes preparation. stop/scan cancel it. Long/invalid commands never arm motion. The servo course uses fixed 1450–1550µs limits and does not load Companion calibration. Read the exact lesson before powering any motor.

**What closes what?** stop is sent to the board. Ctrl+C closes the computer monitor and does not remove electrical power. Unplug motor power before closing/reopening a motor session, resets or wiring changes.

## Check an existing program

Do not upload merely to identify a board. With motor power disconnected and the port identified, `.\robot.ps1 status -Port COM5 -ServoPowerDisconnected` opens a short connection, sends help/status and saves a log in build/logs. Opening serial may reset the board. Silence does not prove it is blank. Do not run this while another monitor owns the port.

## Legacy commands

Hello, Scanner, Face, Motion and Companion remain available for historical work. Their interfaces differ. Commands such as motor/go, wiggle/wiggle start, show/show start, cal and demo belong to Companion; they are not used in the current course. See [archived lessons](../lessons/legacy.md) only if intentionally running that firmware.

## Data tables and timing

Current course sketches use a 60-second automatic deadline and 100ms scheduled rows for start/test/run as applicable. read is one snapshot. TIME_MS is board uptime; actual spacing can vary with I2C/serial work. There is no burst of catch-up rows. Headers print at the beginning of a run; units stay above the data instead of being repeated in every row.

Accelerometer and Gyroscope show TIME_MS, X, Y, Z in matching fixed-width columns. Temperature shows TEMP_C. Display shows the requested expression. Servo shows CHANNEL and PULSE_US; 0 indicates requested pulse-off. Integration puts AX/AY/AZ, GX/GY/GZ, TEMP_C and PULSE_US in one row. Requested motor values are not shaft-position measurements. The one-minute motor test repeats seven bounded eight-second cycles and rests at center for the final four seconds; its travel envelope is unchanged.

The archived legacy sketches and Companion retain their historical timing. Use the named current lesson sketches for these one-minute tests.

Face commands animate eyes and mouth in four-second loops for up to 60 seconds; `stop` freezes the current frame and `clear` blanks the screen. Start readings with `start`, then select a face to animate alongside the 100ms sensor rows. Display `test` shows each routine for four seconds during its minute-long parade. Integration `test` still prepares the motor; only the separate `run` starts it. `cool` replaces the old `love` command.
