# Project working instructions

- Preserve requirements: solder-free assembly, Amazon-only purchases, XIAO ESP32C3, corded V1 before cordless V2.
- Read README.md and docs/PLAN.md before changing implementation.
- Do not describe proposed hardware, generated meshes, or compiled code as physically tested.
- Keep servo V+ separate from 3.3V logic. Do not silently change the two-supply V1 architecture.
- Verify exact product variants, installed headers, connector polarity and included accessories.
- Keep calibration per leg; do not add uncontrolled startup movement or full-range sweeps.
- Record physical tests and exact firmware dependencies. Update documentation when design decisions change.
- Do not commit Wi-Fi credentials, tokens, local account details, or upstream assets with unverified reuse terms.
- This is a first-time learner training repository. Read docs/START-HERE.md and lessons/README.md. Explain one checkpoint at a time: prediction, action, expected result, and evidence.
- Use robot.ps1 and toolchain.psd1 for all firmware operations. Never depend on build.local.psd1, the original author's external toolchain, or remembered absolute installation paths.
- Downloads belong in ignored .tools/ and generated output in build/. Document prerequisites and reproduction commands for any added artifact.
- Never guess a COM port or overwrite a potentially programmed board just to identify it. Opening serial can reset it. Obtain physical confirmation of disconnected servo power before serial/upload actions; compile and discovery do not need it.
- Introductory sketches do not initialize PCA outputs. Follow each lesson's limited wiring; restore Companion explicitly for integration.
