# Troubleshooting by evidence

| Symptom | First useful check |
|---|---|
| Local tools missing / arduino-cli not recognized | Run `./robot.ps1 setup`; do not type bare arduino-cli or depend on PATH |
| Setup download interrupted | Rerun setup; keep the full first error; check internet and disk space |
| Checksum mismatch | Stop; do not run the archive or bypass verification. Check the official release |
| Script blocked | See setup's reviewed-script/session-policy instructions |
| Another command running | Close monitor or wait for setup/build; do not launch competing commands |
| Missing bits/c++config.h | Use robot.ps1 so tools use a short mapped path; check availability of a free drive letter |
| No new COM port | Check a data-capable cable, USB connection, and port inventory before/after reconnecting |
| Access to port denied | Close other monitors; only one program should own the port |
| Silent status probe | Could be a different sketch, USB CDC disabled, wrong port, or startup timing; silence does not establish a blank board |
| Compile error | Find the first error, check toolchain versions with doctor, and undo the last isolated code change if appropriate |
| OLED/IMU missing | Power off; inspect actual labels, grounds, SDA/SCL, 3.3V wiring, and loose jumpers; use Scanner |
| Reset or buzzing during motion | Disconnect servo power and inspect binding and supply paths with an adult |

Do not change several wires and libraries at once. Log expected result, actual result, exact sketch, commands, and connected modules. Never use a missing I2C response as a reason to apply 5V to the shared logic bus.

If a terminal is forcibly terminated, `finally` cleanup may not run. `subst` lists current mappings. Only remove a stale mapping after confirming it points to this checkout and no build/monitor is using it. Normal script completion cleans up automatically.
