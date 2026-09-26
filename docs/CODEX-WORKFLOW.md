# Use Codex as a teaching partner

For every wiring step, include its [stage diagram and pinout table](wiring/README.md), identify the actual hardware, and explain any unverified physical layout. Follow the repository's wiring documentation rule in AGENTS.md. Never treat a functional illustration as a simulated or physically tested circuit.

Codex is optional. It can read and edit this repository and invoke its scripts, but cannot physically verify wiring or replace a slicer or compiler. No paid AI service is required to use the lessons manually. Use a parent-managed account consistent with its terms.

Start Codex in this repository from PowerShell (`codex` if installed), then paste:

```text
Read AGENTS.md, docs/START-HERE.md, docs/COMMANDS.md, lessons/README.md,
and docs/TEST-LOG.md. Teach me and Andrew one checkpoint at a time.
Use robot.ps1 and toolchain.psd1; do not use outside tool installations,
build.local.psd1, guessed COM ports, or remembered machine-specific paths.
Explain the prediction, command, expected result, and what it proves.
Start by asking which hardware is connected and identifying the board.
Do not overwrite existing firmware or command motion until we reach that
step. Record observed results, not assumptions. Distinguish compile checks
from physical tests. Tell us what to do by hand and wait for the result.
```

Useful follow-ups:

- “Run `./robot.ps1 setup`, then compile Basics. Explain both steps.”
- “Run `./robot.ps1 ports`. Help me identify the XIAO by unplug/replug.”
- “Servo power is physically disconnected. Probe help/status on the port we identified; do not upload.”
- “Guide the standalone Gyroscope lesson 3. Ask me to predict the effect before testing. Use its sketch and commands, not Companion.”
- “Review my experiment record and add only confirmed results to the test log.”

PowerShell commands go in the terminal; natural-language requests go in Codex's conversation. A fresh Codex session should read repository instructions instead of relying on chat history. Background monitors must be closed before another serial command. Do not give an agent an unrestricted mandate to move the robot unattended.

Reference: [official Codex CLI documentation](https://developers.openai.com/codex/cli/).
