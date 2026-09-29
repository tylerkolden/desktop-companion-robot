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

## 2026-09-25 — Big Wiggle lesson added
Owner reported NOT SURE after two 1500/1510us tests. Added lesson 5B.1 and wiggle / wiggle start: channel 0 only, three ramped low/high pairs with holds, center return and eight-second automatic pulse-off. Range capped at center +/-50us and existing calibration limits; no automatic widening. Optional screen/sensor need not be removed. Added compile-time checks for ramp endpoints, bounded trajectory samples, asymmetric limits and amplitude cap. Companion compiled successfully (370740 bytes flash, 13192 bytes globals); script guards, 184 local documentation destinations and EPUB structure passed. New test has not been uploaded or physically observed.

## 2026-09-25 — independent workshop refactor
Replaced the current stacked lesson index with seven separate plans and seven independent sketch entry points: Basics, Accelerometer, Gyroscope, Temperature, Display, Servo, Integration. Shared compile-time-selected library provides consistent commands without Companion or its saved settings. MPU6050 functions have separate measurement outputs and correct units; temperature is explicitly die temperature. Motor programs require test then run, expire preparation after 60 seconds, perform fresh pre-run device checks, limit channel 0 to 1450–1550us, and request all outputs off after eight seconds or stop/fault. No new sketch starts motion at boot.
All seven course sketches compiled for the pinned XIAO ESP32C3 toolchain; Servo and Integration were recompiled after the final pre-run check change. Compile-time assertions cover trajectory bounds, ramps, center return, unit conversion and timer rollover. Script argument guards, course/profile contracts, 237 local Markdown destinations and EPUB structure passed. Updated book and standalone pages generated. No serial port opened, upload performed or physical test attempted in this refactor. Prior Companion observations do not validate the new sketches; follow each plan's physical exit criteria.

## 2026-09-28 — one-minute lesson tests and aligned data tables
All seven current course programs now end timed reading/movement runs after 60 seconds and schedule data rows every 100ms. Accelerometer and Gyroscope share fixed-width X/Y/Z tables; Integration shows both three-axis measurements, chip temperature and requested pulse width. Single read remains a snapshot. Servo motion retains channel 0 and 1450–1550us limits, repeats seven eight-second cycles, rests centered for four seconds, then requests pulse-off. Stop/fault cancellation and explicit preparation remain in place. Archived firmware is unchanged.
All seven compiled through robot.ps1 with the pinned toolchain: Basics 312536, Accelerometer 324108, Gyroscope 324100, Temperature 323952, Display 328756, Servo 319780, Integration 341444 bytes flash. Compile-time assertions check the minute deadline across timer rollover, 100ms boundary, repeated trajectory bounds and final center. Script guards, seven lesson contracts, 238 local Markdown destinations and book structural checks passed. Regenerated the seven plans and HTML/EPUB, including per-lesson example tables; refreshed the external book copy. No upload or serial connection performed. Actual timing, movement and sensor behavior have not been measured on hardware for this revision.

## 2026-09-28 — seven full OLED faces
Redesigned Awake, Happy and Sleep with eyes, nose and mouth; added Curious, Surprised, Love and Cheeky. Display and Integration share generated 128x64 monochrome bitmaps. The enlarged gallery uses the same source pixels and was visually reviewed. Display test cycles seven faces every two seconds for the existing minute; Integration cycles them during its existing bounded motor sequence and ends in Sleep. Named face commands remain non-motion commands and require stop during a motor run. Archived Companion is unchanged.
Display compiled (335176 flash bytes, 12912 globals) and Integration compiled (347808 flash bytes, 13032 globals). Lesson contracts, 243 local Markdown destinations, script guards and whitespace checks passed. No upload, serial connection or hardware observation performed. Updated repository lessons and command guide only; book files and external book copy deliberately unchanged at the owner's request. Lesson generator now leaves the book alone unless --book is explicitly used in the final pass.

## 2026-09-28 — simpler cartoon artwork
Replaced all seven face bitmaps with large cartoon eyes and mouths only. Removed noses, pupils, eyebrows, decorative symbols and screen labels; mouths now use the lower screen area. Command names remain in the monitor. Reviewed the exact-pixel gallery and updated the OLED lesson and face guide. Display (335176 flash bytes) and Integration (347808 flash bytes) compiled; lesson contracts, 243 documentation destinations and whitespace checks passed. No upload or physical screen check performed. Book unchanged.

## 2026-09-28 — animated canine expressions
Added four-second, 40-frame routines for Awake, Happy, Sleep, Curious, Surprised, Cool and Cheeky. Cool sunglasses replaces Love. Current OLED lessons show only cartoon eyes and mouths, with no labels or noses. Frame scheduling uses elapsed time without delay/catch-up loops. stop/fault freezes animation; clear blanks it; startup stays static. Named face commands preserve an existing readings stream but never start motors. Display parade and Integration face changes now allow four seconds per expression; minute deadlines and motor travel remain unchanged.
Generated 81 deduplicated flash bitmaps (82944 bytes), firmware sequences and animated preview. Reviewed representative frames visually; check_faces.py verified all 280 exported frames against source pixels, distinct animation per face and a 4000ms GIF loop. Display compiled (411882 flash bytes, 12928 globals); Integration compiled (424540 flash bytes, 13040 globals). Lesson contracts, 243 documentation destinations and script guards passed. This validates compilation and digital assets, not measured on-device animation timing, stop latency or physical operation. No serial connection or upload performed. Book and external book copy unchanged.

## 2026-09-28 — full checked-range servo sweep
Owner requested full travel in both directions and confirmed the servo is not calibrated. Added calibrate/run preparation, ramped 25us lower/higher steps, center return, observed marklow/markhigh endpoints and finish acceptance. Calibration expires after 60 seconds; stop/fault requests pulse-off. Endpoints remain session-only for channel 0 and must be reset for a changed servo/mechanism; range LOW HIGH imports the owner's checked notebook values without moving. test refuses sweep preparation until endpoints are supplied. test/run now sweeps center/low/high/center seven times over 56 seconds, rests centered four seconds, then requests outputs off. This is the entire checked usable range, not forced mechanical travel or an assumed angle. Initial centering still requests 1500us from an unknown physical position.
Final Servo and Integration builds passed (350026 and 454662 flash bytes respectively). Compile-time assertions check both endpoints, asymmetric ramps, range bounds, center return, calibration increments and timer rollover. Lesson contracts, 246 local documentation destinations, script guards and whitespace checks passed. Added the guided endpoint lesson and updated the current course; book unchanged. No upload, serial connection, calibration or physical movement performed. Hardware endpoint values and behavior remain unverified.
