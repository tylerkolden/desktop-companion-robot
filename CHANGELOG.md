# Changelog

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
