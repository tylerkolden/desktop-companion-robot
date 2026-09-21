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

## 2026-09-20 — user-reported Hello upload attempt

- User's Hello compilation passed: 283,516 bytes flash and 11,944 bytes global RAM.
- Earlier COM5 status probe was denied access. A subsequent upload opened COM5 and identified ESP32-C3 revision v0.4 with native USB-Serial/JTAG, then started its stub flasher.
- Transfer failed with `Packet content transfer stopped`; output includes a requested switch to 921600 baud. No successful flash verification or Hello greeting was shown. Cause remains unknown; no device identifiers beyond model/revision are recorded here.
- Added selectable upload baud, default 115200, and documented cable/monitor checks plus manual BOOT recovery. This change has not yet been demonstrated to resolve the physical transfer failure. No firmware source changed.

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
## 2026-09-20 — wiring documentation revision

Added six reproducible functional wiring illustrations, component pinouts and endpoint tables across nine guides and eleven book chapters. Updated the self-contained EPUB and browser book. Local Markdown destinations and EPUB XML, image references and reading-order checks passed. Visually inspected the diagrams for readability. Physical pin positions on clone modules remain unverified; no hardware was connected, uploaded or tested for this documentation revision. These are custom illustrations, not actual Tinkercad simulations.

## 2026-09-20 — lesson transition review

Checked Face and Motion sketch behavior against the teaching sequence. Made lessons 2–3 OLED-only, explicitly replaced OLED with GY-521 in lesson 4, and introduced the first servo only after lesson 5 logic checks. Aligned book workshop sessions and integration entry/exit states. Rebuilt wiring documentation and EPUB; local-link and book structural checks passed. Documentation-only change: no upload or physical hardware test performed.

## 2026-09-20 — teaching-state review

Removed unintroduced hardware from early-lesson prose and generated warnings. Split lesson 5 into a logic communication checkpoint and a motor/power checkpoint with an explicit parts introduction. Clarified first supply measurement versus subsequent disconnection. Checked lessons 0–4 for premature hardware references, regenerated diagrams and book, and checked local links and EPUB structure. No hardware test or firmware change.

## 2026-09-20 — individual device validation and timed output test

Added explicit `test N` command: 500ms center, 500ms center+10us, then pulse-off request; stopped-state and existing travel limits required. Stop/scan cancel, no boot activation, no saved calibration changes. Companion compiled for the pinned XIAO target (358062 bytes flash, 13152 bytes globals), including phase-boundary, overdue and rollover assertions. Documentation now tests ESP32 persistence/timing, OLED repeated rendering, sensor orientation, driver communication and observed unloaded output before combined validation. Documentation links and EPUB structure checked. No upload or physical movement test performed; cooperative timer and I2C stop limitations remain.

## 2026-09-20 — owner-reported lesson 5A result

Owner supplied serial output after a successful Companion upload: scan returned 0x40; status reported PCA=1, OLED=0, MPU=0, storage=1, timed_test=0 and cal_channel=-1. This supports driver communication and initialization at the driver-only checkpoint, not motor output or movement. Added explicit instructions distinguishing monitor input from PowerShell and explaining missed startup text. Documentation/book checks passed; no new upload or physical action performed by the assistant.

## 2026-09-20 — beginner device reports

Added named device/address reports with ASCII [OK]/[X] markers, fresh scan replies, readiness explanations and details command retaining numeric diagnostics. Documentation links and EPUB structure passed. Compilation attempted twice but blocked by the existing robot command session lock; no process interrupted, no port opened, no upload performed. Firmware compilation and physical output verification remain pending.


## 2026-09-20 — guided motor monitor and channel identification

Added motor preparation guide and one-shot go command for channel 0, progress labels and observation prompt. Opening the guide requests stop; go requires preparation and consumes it. Updated channel identification using printed 0 and three-pin grouping, without claiming verified clone orientation. Companion compiled successfully: 362592 bytes flash, 13152 bytes globals, including existing timing assertions. This build also validates the previously pending beginner reports. Script guard checks, local documentation links and EPUB structure passed. No upload, serial connection or physical movement test performed.


## 2026-09-20 — sequential channel finder

Added hunt preparation and hunt start diagnostic: 16 outputs sequentially, center/offset/off pause, one active output, checked all-off writes and manual stop. Companion compiled: 364964 bytes flash, 13168 bytes globals. Existing compile-time assertions passed; new sequence has not been exercised on hardware. Documentation checks passed. No upload or movement performed by assistant.

## 2026-09-20 — no motion on channel search

Owner reported no visible motion on any channel. Root cause remains unconfirmed. Reviewed installed Adafruit library: setPWM success convention, 50Hz prescale setup and pulse conversion match firmware use. Added read-only diagnose command for MODE1/MODE2/PRESCALE and all channel registers, with explicit physical measurement limitations. Build passed: 366936 bytes flash, 13168 bytes globals. Script guards and documentation links passed. No upload or physical test performed by assistant.


## 2026-09-20 — combined bench check and personality-show implementation
Owner confirmed screen expression changes. Live serial showed OLED 0x3C, MPU 0x68 and PCA 0x40 together, with changing acceleration readings during handling. Earlier owner confirmed channel-0 movement in the 1450–1550us test; cause of the earlier no-motion result remains unproven. Added a 12-second, channel-0-only personality show with four face/motion scenes, capped travel, explicit preparation/start, cancellation and automatic pulse-off. New sequence is not yet physically tested. Display wording now distinguishes servo inactivity from sensor operation.

## 2026-09-20 — personality show observed on hardware
Companion revision 1501ffd uploaded successfully to the XIAO ESP32C3 on COM5. After the owner confirmed motor power connected, the assistant sent show start through the existing monitor. Serial reported all four scenes and automatic completion. Owner then confirmed: 'it worked!' in response to the question about coordinated eye changes and servo sway/wiggles. Post-run status reported show off, no active calibration channel, and all three modules initialized. Driver readback showed full-off=1 on all 16 channels. This supports one successful coordinated bench performance and output-register shutdown; it does not establish measured servo angles, repeatability, emergency-stop behavior or loaded mechanism performance. Motor-supply disconnection was requested afterward but has not been confirmed.
