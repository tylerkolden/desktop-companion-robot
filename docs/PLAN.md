# Development plan

## Milestone 0: documented starting point

- [x] Study upstream code and missing model situation.
- [x] Record owned hardware and Amazon-only selections.
- [x] Choose solder-free assembly, corded V1 and cordless V2.
- [x] Draft wiring, measurement, build and validation instructions.
- [ ] Confirm additional purchases and receipt checks.

## Milestone 1: bench diagnostics

- [ ] Add I2C scanner and serial diagnostics; compile for XIAO ESP32C3.
- [ ] Pin exact board/library versions and document a repeatable build.
- [ ] Validate OLED and IMU separately.
- [ ] Add PCA9685 one-servo calibration with outputs inactive until commanded.
- [ ] Measure supply behavior and calibrate four unloaded servos.

Exit: repeatable upload, detected modules, stable power, recorded calibration.

## Milestone 2: editable mechanical design

- [ ] Measure actual boards, servo horns and connector envelopes.
- [ ] Model servo cradle and display mount fit coupons.
- [ ] Print, measure and revise coupons.
- [ ] Design lightweight chassis, removable shell, four legs and cord strain relief.
- [ ] Use captive nuts or accessible screws; avoid heat-set inserts and permanent glue for service parts.
- [ ] Specify final Amazon fasteners and count; retain original servo horn screws.
- [ ] Export versioned CAD, STL and printer settings; verify meshes and physical fit.

Exit: assembled shell with accessible plugs, no binding, and documented fasteners.

## Milestone 3: V1 behavior

- [ ] Separate display, sensing, motion, behavior and configuration modules.
- [ ] Use a non-blocking scheduler; continue sensing during animations.
- [ ] Implement real double-tap timing/debounce, orientation hysteresis and recovery.
- [ ] Separate last-user-interaction time from animation time so idle sleep works.
- [ ] Clamp motion to per-servo limits; handle missing sensors conservatively.
- [ ] Add stand/sit, blink/sleep, small gestures and predictable startup.
- [ ] Optional after baseline: local Wi-Fi controls with motion stop and credentials outside git.

## V1 release acceptance

- [ ] Reproduce firmware build from a clean checkout.
- [ ] Boot with servos inactive until initialization succeeds.
- [ ] Demonstrate display, motion sensing and four calibrated legs.
- [ ] Demonstrate tap timeout, orientation response and idle sleep.
- [ ] Run a 30-minute supervised gesture/idle cycle without observed resets, binding or loose wires.
- [ ] Check power during startup and simultaneous motion; document measurement limitations.
- [ ] Confirm cable strain relief, component retention and access to unplug power.
- [ ] Build without soldering or undocumented modifications.
- [ ] Record tested hardware revisions, CAD revision, settings, fasteners and limitations.

Walking and desk-edge avoidance are not release promises. No unattended movement near an edge.

## V2: cordless

Begin only after V1 current, mass and motion requirements are measured. The existing JLJLUP pack is 3000mAh, 65 x 36 x 10mm and 49g. Its listing says both 1C (implying 3A) and an explicit maximum of about 1.5A. Use the more restrictive limit; the pack is not selected for four-servo power. At nominal voltage, 1.5A gives 5.55W input, or roughly 0.94-1.0A at 5V for 85-90% conversion efficiency, before controller consumption.

- [ ] Capture typical and peak current with suitable measurement equipment.
- [ ] Set runtime, mass, regulator and connector requirements from measurements.
- [ ] Select a compatible preassembled battery, charging and regulation solution on Amazon.
- [ ] Verify connector family and polarity; a generic JST1.25 description is insufficient.
- [ ] Define power switching, low-voltage cutoff and charge/use behavior.
- [ ] Revise removable power compartment and center of mass.
- [ ] Test runtime, temperature, peak-load stability and charging independently.

Do not improvise series/parallel combinations of the two existing batteries. No V2 battery or charger is approved in this plan yet.
