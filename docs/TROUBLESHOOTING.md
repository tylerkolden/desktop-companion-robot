# Troubleshooting by evidence

Wiring reference: [full pinout atlas and verification limits](wiring/README.md). Use the stage matching your lesson; do not connect every pictured module at once.
<!-- wiring:start -->
### Wiring visuals and pinouts

**Before changing wires: unplug USB and the servo adapter. Battery stays disconnected.** These custom Tinkercad-style illustrations are not a Tinkercad project or simulation. Board shapes are illustrative; terminal positions are functional, not verified physical placement. Read actual board labels.

![USB wiring: labelled endpoints and separate power paths](wiring/images/usb.png)

The pinout for this stage is USB-C socket only: **all XIAO header pins are unused**. No external modules or battery.

| From / pin | To / pin | Meaning |
|---|---|---|
| Computer USB port | XIAO USB-C socket | Data cable; all headers empty |

![OLED wiring: labelled endpoints and separate power paths](wiring/images/oled.png)

Functional pinout: VCC, GND, SDA, SCL. The supplied display photo has **GND / VCC / SCL / SDA** across its top header when viewed from the front. Our illustration groups wires by function, not that physical order.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | AITRIP SSD1306 OLED VCC | 3.3V logic |
| XIAO GND | AITRIP SSD1306 OLED GND | Shared ground |
| XIAO D4 / GPIO6 | AITRIP SSD1306 OLED SDA | I2C data |
| XIAO D5 / GPIO7 | AITRIP SSD1306 OLED SCL | I2C clock |

![IMU wiring: labelled endpoints and separate power paths](wiring/images/imu.png)

Functional pinout: VCC, GND, SCL, SDA, XDA, XCL, AD0, INT. Leave XDA/XCL/INT open; verify AD0 default through scanning. Exact delivered header order is unconfirmed. Check regulator/pull-ups at 3.3V; do not substitute 5V to fix detection.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | GY-521 / MPU6050 VCC | 3.3V logic |
| XIAO GND | GY-521 / MPU6050 GND | Shared ground |
| XIAO D4 / GPIO6 | GY-521 / MPU6050 SDA | I2C data |
| XIAO D5 / GPIO7 | GY-521 / MPU6050 SCL | I2C clock |

![PCA wiring: labelled endpoints and separate power paths](wiring/images/pca.png)

Functional pinout: VCC = logic 3.3V; V+ = separate servo power; GND = common ground; SDA/SCL = I2C; OE = active-low output enable. Exact clone header order is unconfirmed. Verify OE pull-down; ground OE only if floating. V+ and servo plugs remain disconnected in this logic-only stage.

| From / pin | To / pin | Meaning |
|---|---|---|
| XIAO 3V3 | Teyleten PCA9685 VCC | 3.3V logic |
| XIAO GND | Teyleten PCA9685 GND | Shared ground |
| XIAO D4 / GPIO6 | Teyleten PCA9685 SDA | I2C data |
| XIAO D5 / GPIO7 | Teyleten PCA9685 SCL | I2C clock |

![SERVO wiring: labelled endpoints and separate power paths](wiring/images/servo.png)

Functional servo pinout: ground, power, signal. Read the driver row labels before inserting the plug. **Also complete the PCA logic diagram** (3V3/VCC, GND, SDA, SCL). Start with only channel 0; later 0/1/2/3 map to front-left/front-right/rear-left/rear-right. USB and adapter grounds meet at PCA GND; adapter positive never meets XIAO 5V or 3V3. Grounded OE is not a reset interlock: unplug servo power for uploads, resets and horn fitting.

| From / pin | To / pin | Meaning |
|---|---|---|
| Adapter positive | PCA V+ screw terminal | Regulated 5V; direct power wire |
| Adapter negative | PCA GND screw terminal | Direct power wire |
| XIAO GND | PCA GND | Common reference |
| PCA channel 0 GND | SG90 ground | Usually brown/black; verify |
| PCA channel 0 V+ | SG90 power | Usually red; verify |
| PCA channel 0 PWM | SG90 signal | Usually orange/yellow; verify |

<!-- wiring:end -->

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
