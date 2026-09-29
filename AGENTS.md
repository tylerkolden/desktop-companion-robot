# Project working instructions

- Defer book manuscript, HTML, EPUB and external book-copy changes until the user requests the final book pass. Update repository lesson plans independently with scripts/build_lessons.py (without --book). Do not run book-generating scripts during routine code updates; build_wiring.py also modifies book content.

- Every wiring instruction must include a stage-specific visual diagram and pinout/connection table. Maintain `scripts/build_wiring.py` and the wiring atlas; regenerate affected guides and the book. Identify the exact model, viewing orientation and verification limits. Never invent physical pin positions, breadboard hole coordinates, or an actual Tinkercad simulation. Clearly label functional diagrams when physical layout is unverified. Preserve the learner's solder-free, adult-supervised staged sequence.

- Preserve requirements: solder-free assembly, Amazon-only purchases, XIAO ESP32C3, corded V1 before cordless V2.
- Read README.md and docs/PLAN.md before changing implementation.
- Do not describe proposed hardware, generated meshes, or compiled code as physically tested.
- Keep servo V+ separate from 3.3V logic. Do not silently change the two-supply V1 architecture.
- Verify exact product variants, installed headers, connector polarity and included accessories.
- Keep calibration per leg; do not add uncontrolled startup movement. The beginner lesson now uses a fixed wider practice sweep (1000–2000us), with test/run and no calibration prerequisite. Full mechanical travel is outside this beginner test and must not be claimed. Preserve stop/deadline controls. Never infer mechanical limits from an SG90 label.
- Record physical tests and exact firmware dependencies. Update documentation when design decisions change.
- Do not commit Wi-Fi credentials, tokens, local account details, or upstream assets with unverified reuse terms.
- This is a first-time learner training repository. Read docs/START-HERE.md and lessons/README.md. Explain one checkpoint at a time: prediction, action, expected result, and evidence.
- Write learner-facing instructions for a 10-year-old: short direct actions, exact words to type, and the next step after each command. Do not reintroduce the rejected more/back/keep calibration workflow. Keep the motor lesson to test, follow power instructions, run; retain 100ms test data and clear normal completion versus fault messages.
- Use robot.ps1 and toolchain.psd1 for all firmware operations. Never depend on build.local.psd1, the original author's external toolchain, or remembered absolute installation paths.
- Downloads belong in ignored .tools/ and generated output in build/. Document prerequisites and reproduction commands for any added artifact.
- Never guess a COM port or overwrite a potentially programmed board just to identify it. Opening serial can reset it. Obtain physical confirmation of disconnected servo power before serial/upload actions; compile and discovery do not need it.
- Basics, Accelerometer, Gyroscope, Temperature and Display never initialize PCA outputs. Servo and Integration initialize outputs off and require test then run for motion. Open/upload these with motor power off. Use Integration for the final course lesson; Companion is an optional legacy application.

- Teaching flow must track the learner's actual starting state. Introduce a part by name and purpose before any instruction involving it. State what to keep, remove or add and the expected ending state. Do not mention absent future hardware in warnings (including “battery stays disconnected” or “no servo yet”). Distinguish first assembly from repeat sessions; never tell a first-time learner to unplug a supply they have not introduced or connected.
