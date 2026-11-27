# Changelog

## Checkpoint 6 (in progress): dog body and electronics deck — 2026-11-27

- Added Dog body V1 (four servos, deck posts, MPU6050 floor mount) and top deck V1 (mini breadboard + XIAO, PCA9685, OLED face slot). Board sizes from owner photos; not yet printed.
- Added dog stand/sit/bow pose timing to LessonLogic.h (host-compiled static checks only; the Dog sketch is not written yet).

## Checkpoint 5: one working leg — 2026-11-27

- Leg V1 and single-leg test stand (OpenSCAD source, STLs, previews, mesh bounds checks).
- `Leg` sketch: nine fixed measuring poses between 1000 and 2000us with 4-second holds and HOLD markers; same test/run/stop safety flow as Servo. Compile-time assertions keep poses inside 1000–2000us.
- Checkpoint 5 learner guide: horn fit, centering with the Leg program, assembly, angle measurement.
- Leg V2 after V1 failed the owner's fit test; V3 adds a disc-and-ramp recess (spotted by Andrew) but printed too tight; V4 adds clearance; V5 tried kit M2 screws; V6 (current) uses only the servo-bag screws.
- Not physically tested. The Leg sketch's pose logic was host-compiled; the full ESP32 build was not run in this change.

## Training workflow — 2026-09-20

- Repository-local Windows x64 setup with verified Arduino CLI download, pinned board/libraries, and no dependency on the author's tool directories or PATH.
- One `robot.ps1` entry point for setup, inventory, compilation, port discovery, explicit upload, bounded status capture, and interactive monitoring.
- Standalone Hello, Scanner, Face, and Motion sketches; staged learner experiments before Companion integration.
- Beginner setup, command explanations, troubleshooting, experiment template, Codex handoff, contribution guide, and book/CAD reproduction instructions.
- Computer-only script guard checks and local documentation-link checker. Hardware and physical print validation remain pending.

## 0.1.0 — bench prototype (2026-09-13)

- Original XIAO ESP32C3 firmware for PCA9685, SSD1306 and MPU6050.
- Serial hardware diagnostics and bounded command parser.
- Single-channel servo calibration, stored limits, manual arming and timeouts.
- Animated eyes, double-tap face wake, idle sleep and orientation-gated small gesture.
- Build-time regression assertions for control logic.
- Two original editable fit gauges, STL exports and mesh validation script.
- First-power-on instructions and exact dependency versions.

Not a V1 robot release. No hardware or physical print validation; no walking gait, full enclosure, Wi-Fi interface or cordless subsystem.
