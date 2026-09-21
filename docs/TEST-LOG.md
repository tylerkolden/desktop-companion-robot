# Test log

No physical hardware or print tests have been completed. The following checks were performed on the development computer.

## 2026-09-13 — initial bench implementation

- Firmware: `firmware/Companion/Companion.ino`, bench prototype 0.1.0.
- Arduino CLI 1.5.1, ESP32 core 3.3.0, exact libraries in [firmware README](../firmware/README.md).
- Target: `esp32:esp32:XIAO_ESP32C3:CDCOnBoot=default` (USB CDC enabled).
- Result: final build passed with all compile-time logic assertions; 357,140 bytes flash (27%), 13,144 bytes global RAM (4%). No upload performed.
- A second incremental build verified the final OLED transfer-rate and frame-interval changes. This is not an independent clean-machine reproduction.
- Windows long toolchain paths initially caused a missing `bits/c++config.h` error. A short drive mapping and matching Arduino configuration resolved it; keep installation paths short.
- OpenSCAD 2021.01 exported both fit gauges. Render previews were inspected.
- `scripts/validate_meshes.py`: both meshes passed closed-edge, consistent-winding, connected-shell, positive-volume and expected-bounds checks.
- Servo gauge: 1,452 triangles; 94 x 28 x 2.4mm; volume 4,002.4mm3.
- OLED gauge: 236 triangles; 32 x 32.3 x 2.4mm; volume 455.4mm3.
- Not tested: electrical wiring, USB upload, module detection, servo current/travel, tap sensitivity, stop latency, physical prints, fit or runtime.

## 2026-09-14 — Windows terminal setup correction

The owner encountered `arduino-cli` not found because the CLI used for the initial build was outside PATH. Added ignored local toolchain settings, automatic temporary short-path mapping, and a clearer missing-CLI error. Running `./scripts/build.ps1 -Install` from the real checkout succeeded using the existing pinned dependencies: 357,140 bytes flash and 13,144 bytes global RAM. No device was uploaded or powered by this check.

## 2026-09-20 — repository-local training setup

- Ran `robot.ps1 setup` with an initially empty `.tools/` directory. Downloaded CLI 1.5.1, checked its pinned SHA-256, and installed ESP32 3.3.0 plus all six pinned libraries. No prior author tool installation was used by the new workflow.
- Parsed scripts and tested required-port and disconnected-servo-power guards for upload/status/monitor without opening any serial port. These checks passed in the current PowerShell and Windows PowerShell 5.1.
- Verified that a competing command was rejected while a build held the checkout lock.
- Local Markdown destination checks and both existing STL mesh checks passed. These do not validate external web links, anchors, prints, or electrical hardware.
- All five sketches compiled successfully for XIAO ESP32C3 with USB CDC enabled. Flash / global RAM bytes: Hello 283,516 / 11,944; Scanner 309,786 / 12,648; Face 324,220 / 12,784; Motion 319,912 / 12,768; Companion 357,116 / 13,144. Companion compile-time assertions passed as part of its build.
- `doctor` reported the pinned CLI, core, and six library versions. `ports` listed COM1 and a generic ESP32-family USB device at COM5. This identifies a candidate connection, not a verified XIAO model or installed program; unplug/replug confirmation remains necessary. No port was opened.
- No upload, serial session, powered robot test, or physical print was performed. The user reports that the ESP32 may already be programmed; that remains unconfirmed.

## Physical session template

Copy this template for each session:

```text
Date / tester:
Firmware commit / sketch:
Board package / library versions:
CAD revision / filament / slicer profile:
Hardware variants:
Supply measured without load / with motion:
Meter or instrument and limitations:
I2C addresses found:
Servo channel / center / inversion / travel limits:
Test performed and duration:
Expected result:
Observed result:
Pass / fail / not tested:
Photos or recordings:
Next change:
```
