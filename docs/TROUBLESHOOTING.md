# Troubleshooting by evidence

## Upload connects, then reports Packet content transfer stopped

This means compilation and initial bootloader communication may have succeeded while the later transfer failed. It does not confirm that the requested program was installed, or prove a defective board. Our observed case identified ESP32-C3 on COM5, started the stub flasher, and failed around the switch to 921600 baud. Output from different streams can appear out of order, so that sequence alone does not establish the cause.

Uploads now request 115200 baud by default. This is a diagnostic fallback, not a confirmed cure; native USB-Serial/JTAG does not behave exactly like a physical UART link. Keep other monitors closed, servo supply and battery disconnected, and use the bare XIAO over a reliable USB data cable directly to the computer. Retry once:

```powershell
.\robot.ps1 upload -Sketch Hello -Port COM5 -UploadBaud 115200 -ServoPowerDisconnected
```

Use COM5 only if it is still the identified board. If that fails, unplug USB, hold the XIAO's BOOT button, reconnect USB while holding it, then release BOOT. Run `./robot.ps1 ports` again because the port can change. Retry with that port. Change cable/USB port as a separate test if needed. Do not erase the whole flash as the first troubleshooting step.

After successful upload, open the monitor on the current port and expect Hello's repeating greeting. If it remains in download mode, close the monitor, tap RESET once with servo power still off, then reopen on the detected port. Record actual results rather than marking an upload attempt as passed.

Sources: [Espressif upload troubleshooting](https://docs.espressif.com/projects/esptool/en/latest/esp32c3/troubleshooting.html), [Seeed XIAO recovery instructions](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/#troubleshooting).

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
