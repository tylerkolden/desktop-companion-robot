# Commands and what they prove

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

## Upload and monitor

Uploading replaces the current program. Choose the lesson deliberately. It does not guarantee erasure of previously stored calibration. Close monitor with Ctrl+C before another upload or status capture. In monitor, type a command and press Enter; Companion requires a newline. The automatic status probe always sends LF explicitly.

For first tests use only `help`, `status`, `scan`, and face commands. Servo calibration follows the adult-supervised [first-power-on guide](FIRST-POWER-ON.md), not a generic sweep example. The provided console workflow deliberately requires servo power disconnected; use it for diagnostics and lessons. For a powered servo session, follow that guide with an already-open console, verified wiring, and the adult controlling when power is connected.

Do not reconnect/reopen a console or upload during a powered servo session. Disconnect servo power before stopping/restarting tools. The PCA can retain pulses through a controller reset.
