# Commands and what they prove

Wiring reference: [full pinout atlas and verification limits](wiring/README.md). Use the stage matching your lesson; do not connect every pictured module at once.
<!-- wiring:start -->
**Parts for this power checkpoint:** a regulated 5V wall supply, its low-voltage connector and prepared wires, and a multimeter. For the motor connection, also identify the SG90 servo and PCA9685 driver. The supply powers the motor through the driver; the meter checks its output before connection. Identify these parts before following the diagram.

### Wiring visuals and pinouts

**Before assembling these connections: close the monitor and unplug the XIAO USB cable. Keep the newly introduced 5V supply unplugged from the wall until the voltage-check step. On a repeat session, unplug it before changing wires.** These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.

![USB wiring: labelled endpoints and separate power paths](wiring/images/usb.png)

The pinout for this stage is USB-C socket only: **all XIAO header pins are unused**.

| From / pin | To / pin | Meaning |
|---|---|---|
| Computer USB port | XIAO USB-C socket | Data cable; all headers empty |

![SERVO wiring: labelled endpoints and separate power paths](wiring/images/servo.png)

Functional servo pinout: ground, power, signal. Read the driver row labels before inserting the plug. **Also complete the PCA logic diagram** (3V3/VCC, GND, SDA, SCL). Use only channel 0 for this experiment. USB and adapter grounds meet at PCA GND; adapter positive never meets XIAO 5V or 3V3. Grounded OE is not a reset interlock: unplug servo power for uploads, resets and horn fitting.

| From / pin | To / pin | Meaning |
|---|---|---|
| Adapter positive | PCA V+ screw terminal | Regulated 5V; direct power wire |
| Adapter negative | PCA GND screw terminal | Direct power wire |
| XIAO GND | PCA GND | Common reference |
| PCA channel 0 GND | SG90 ground | Usually brown/black; verify |
| PCA channel 0 V+ | SG90 power | Usually red; verify |
| PCA channel 0 PWM | SG90 signal | Usually orange/yellow; verify |

![PCA wiring: labelled endpoints and separate power paths](wiring/images/pca.png)

Functional pinout: VCC = logic 3.3V; V+ = separate output-power input; GND = common ground; SDA/SCL = I2C; OE = active-low output enable. Exact clone header order is unconfirmed. Verify OE pull-down; ground OE only if floating. Use only VCC, GND, SDA and SCL for this communication test. Leave V+ and the numbered output connectors empty.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | Teyleten PCA9685 VCC | 3.3V logic |
| XIAO GND | Teyleten PCA9685 GND | Shared ground |
| XIAO D4 / GPIO6 | Teyleten PCA9685 SDA | I2C data |
| XIAO D5 / GPIO7 | Teyleten PCA9685 SCL | I2C clock |

<!-- wiring:end -->

These are **PowerShell commands**, executed from the repository root. In a Codex conversation, ask Codex to run the exact command; plain English prompts are not shell syntax. `COM7` below is an example, never a detected port.

| Command | Purpose | Changes board firmware? |
|---|---|---|
| `.\robot.ps1` | Show help | No |
| `.\robot.ps1 setup` | Install pinned tools locally | No |
| `.\robot.ps1 doctor` | List installed versions | No |
| `.\robot.ps1 ports` | Discover serial ports | No |
| `.\robot.ps1 build` | Compile Companion | No |
| `.\robot.ps1 build -Sketch Hello` | Compile a lesson | No |
| `.\robot.ps1 status -Port COM7 -ServoPowerDisconnected` | Send only help/status; read for six seconds after a two-second startup wait | No, but opening a port may reset it |
| `.\robot.ps1 upload -Sketch Hello -Port COM7 -ServoPowerDisconnected` | Compile fresh and upload selected sketch | Yes |
| `.\robot.ps1 monitor -Port COM7 -ServoPowerDisconnected` | Interactive serial console at 115200 | No, but user commands can change behavior |

Other sketch names: `Scanner`, `Face`, `Motion`, `Companion`. Default is Companion. Upload never guesses a port. `-ServoPowerDisconnected` is a human declaration, not an electrical measurement. Use it only after physically removing servo power. Keep the battery disconnected too.

## Check an existing program

1. Unplug servo power. Disconnect USB before changing any other wiring.
2. Run `ports` with the XIAO unplugged, then with it connected by USB data cable. Identify the new port; COM1 alone is not identification. An unknown board name can still be a serial device—confirm by unplug/replug.
3. Run `status` on that identified port. Close Arduino Serial Monitor or other programs using it first.
4. Companion should respond with command help and flags such as `OLED=`, `MPU=`, `PCA=`. Missing modules are expected on a bare board. Hello instead repeats its greeting. No output is inconclusive; consult [troubleshooting](TROUBLESHOOTING.md).

The bounded capture is saved in ignored `build/logs/`. Review useful observations before copying them into the test log. The script does not claim to identify arbitrary unknown firmware or back it up. Sending help/status is designed for this project's sketches.

### Where to type the board commands

After `Connecting to COM5. Press CTRL-C to exit.` (your port number may differ), the terminal is the **serial monitor**. Click in that same terminal, type one command, and press **Enter** to send it to the XIAO:

```text
help
scan
status
```

Send each line separately and read its response. Do not include backticks or type these at the `PS ...>` PowerShell prompt. Your typing may or may not be echoed. Companion waits for Enter before acting. Unlike Scanner, it does not repeat scans automatically.

You may miss the beginning of the startup message while the monitor connects. Type `help` to print the command list again; no upload is needed. Press **Ctrl+C** when finished to return to PowerShell. Close the monitor before another upload or another program opens the same port.


## Upload and monitor

Uploads request **115200 baud** by default. `-UploadBaud` accepts 115200, 230400, 460800, or 921600. This affects the uploader's requested setting, not the firmware's Serial Monitor setting. On native USB, lowering it is only a troubleshooting experiment, not proof of a slower physical USB connection. See [transfer-failure recovery](TROUBLESHOOTING.md#upload-connects-then-reports-packet-content-transfer-stopped).

Uploading replaces the current program. Choose the lesson deliberately. It does not guarantee erasure of previously stored calibration. Close monitor with Ctrl+C before another upload or status capture. In monitor, type a command and press Enter; Companion requires a newline. The automatic status probe always sends LF explicitly.

For first tests use only `help`, `status`, `scan`, and face commands. Servo calibration follows the adult-supervised [first-power-on guide](FIRST-POWER-ON.md), not a generic sweep example. The provided console workflow deliberately requires servo power disconnected; use it for diagnostics and lessons. For a powered servo session, follow that guide with an already-open console, verified wiring, and the adult controlling when power is connected.

Do not reconnect/reopen a console or upload during a powered servo session. Disconnect servo power before stopping/restarting tools. The PCA can retain pulses through a controller reset.

## Reading Companion reports

`status` prints a friendly device report with `[OK]` for initialized devices and `[X]` for devices not ready. `[X]` is expected when a lesson has set that part aside. These flags are not a fresh physical connection test. `scan` checks current I2C address replies and labels the expected project devices; an address cannot prove device identity or full function. `details` preserves the original numeric flags and calibration rows for troubleshooting. All three are typed inside the serial monitor, followed by Enter. Updated wording requires uploading the new Companion firmware.
