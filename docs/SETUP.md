# Set up the repository

## 1. Get your own copy

This repository is currently private. New learners need authorized GitHub access; it is not a public download for everyone yet. GitHub account and access management are adult tasks.

If Git is installed, open PowerShell in a folder where you keep projects and run:

```powershell
git clone https://github.com/tylerkolden/desktop-companion-robot.git
cd desktop-companion-robot
```

Alternatively, sign into GitHub, use **Code → Download ZIP**, extract it, and open PowerShell in the extracted folder. ZIP users can build but do not have Git history or `git pull`. Do not run scripts from inside the ZIP viewer.

Existing owners should use their current clone rather than create another copy. All commands below are run from the folder containing `robot.ps1`.

To update an existing Git clone later, run `git status` first and preserve any lesson edits. Then use `git pull --ff-only` on the intended branch. If Git reports conflicting/local changes, resolve them deliberately rather than discarding them. Rerun setup when the pinned toolchain changes. A ZIP copy needs a fresh authorized download; keep learner notes and edits separately before replacing it.

## 2. Install the project tools

```powershell
.\robot.ps1 setup
.\robot.ps1 doctor
.\robot.ps1 build -Sketch Hello
```

Setup downloads Arduino CLI 1.5.1 from its official GitHub release, verifies its SHA-256 against the checked-in release checksum, and installs the ESP32 core and libraries pinned in `toolchain.psd1`. Arduino performs its package checks. Initial setup can take several minutes and downloads large compiler packages. Let it finish; rerunning after a connection failure reuses cached downloads where possible.

Everything this script installs goes under `.tools/` in the checkout: CLI, board packages, libraries, downloads, and compile cache. Build output goes under `build/`. Both are ignored by Git. Source, lessons, version specifications, and setup code are tracked; multi-gigabyte tool binaries are downloaded reproducibly rather than committed. The initial setup needs internet. Installed builds normally work offline.

No global PATH edits, administrator installation, Arduino IDE setup, old `build.local.psd1`, or files from the author's computer are required. A temporary free drive letter gives Windows compilers a short path and is released afterward. The script prevents simultaneous commands in one checkout. It clears inherited `ARDUINO_*` overrides for the run and restores them afterward.

If PowerShell blocks a downloaded script, inspect it first. On a trusted clone/ZIP you reviewed, remove the download mark with `Unblock-File .\robot.ps1`. If local execution policy still blocks it, use a session-only policy permitted by your computer's rules: `Set-ExecutionPolicy -Scope Process RemoteSigned`. Do not change organization-managed policy or turn off system-wide protections.

## 3. Understand the result

The compile report gives flash and RAM use. Success means the program was built for the selected board. It has not been uploaded. Continue to [lessons](../lessons/README.md) or [inspect an existing program](COMMANDS.md#check-an-existing-program).

## Other tools and operating systems

- **Arduino IDE:** optional. Open any sketch in its own folder and install the versions in `toolchain.psd1` through its managers. Select XIAO_ESP32C3 and USB CDC On Boot enabled. IDE installs are separate from `.tools/`.
- **Codex/VS Code:** optional. They can edit these files and run the same commands. See [Codex learning prompt](CODEX-WORKFLOW.md).
- **3D printing:** pre-exported fit STLs are included. Bambu Studio is still needed for the documented A1 slicing/print workflow. OpenSCAD 2021.01 is needed only to regenerate or change CAD. [CAD instructions](../cad/README.md) include commands from the repository root.
- **Book editing / mesh checks:** Python 3 is optional; [reproduction instructions](REPRODUCE.md) describe repository-local commands and a virtual environment.
- **macOS/Linux/Windows ARM:** the automatic bootstrap has not been implemented or tested for these platforms. Arduino IDE with the pinned dependencies is the manual alternative. Do not run the Windows x64 installer on another platform.

References: [Arduino CLI configuration](https://arduino.github.io/arduino-cli/1.5/configuration/), [pinned CLI release](https://github.com/arduino/arduino-cli/releases/tag/v1.5.1), [release checksums](https://github.com/arduino/arduino-cli/releases/download/v1.5.1/1.5.1-checksums.txt).
