# Reproduce book and CAD artifacts

Firmware uses `robot.ps1`; these optional authoring tasks are separate. All project code and inputs are in this repository. Install Python 3 and OpenSCAD only if modifying these artifacts; reading the EPUB and printing existing STLs do not require them.

## Python environment in the repository

Run from the repository root, using your installed Python 3 executable (`python` or Windows `py -3`):

```powershell
python -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r requirements-book.txt
.\.venv\Scripts\python.exe scripts/validate_meshes.py
.\.venv\Scripts\python.exe scripts/build_wiring.py
.\.venv\Scripts\python.exe docs/book/build_book.py
```

The book builder uses Windows Arial fonts from `C:/Windows/Fonts`; that operating-system dependency is explicit. It writes the EPUB, HTML, diagrams, and validation report alongside the tracked manuscript. Review generated changes before committing. The original book is an edition snapshot; current setup and lessons take precedence where workflows have evolved.

## CAD

Use OpenSCAD 2021.01 and the repository-root commands in [cad/README.md](../cad/README.md). No author-specific installation path is required; if OpenSCAD is not on PATH, invoke its installed executable by full path. Bambu Studio slicing still requires checking the actual A1 nozzle, filament, orientation, and scale. Generated printer instructions are not supplied as a universal file.

## Validation scope

Run `./scripts/check.ps1` after setup to parse scripts, check upload/serial argument guards without opening a port, and compile the seven current course sketches plus five legacy sketches. `-SkipBuild` runs only syntax/guard checks. Run `python scripts/check_docs.py` to check local Markdown destinations (not web links or anchors), and `python scripts/check_lessons.py` to check course profiles, source entry points and lesson structure.

For routine lesson edits, run `python scripts/build_lessons.py`; this generates the seven lesson pages without changing the book. Use `python scripts/build_lessons.py --book` only during the requested final book pass. `scripts/build_wiring.py` also modifies book chapters and images, so defer that generator while book changes are on hold. Edit course data in build_lessons.py rather than generated pages. The local RobotLessons Arduino library is supplied by robot.ps1's compile command; no global installation is required for that workflow.

For OLED artwork, run `python scripts/build_faces.py` with the same Python dependencies. It generates the firmware bitmap header and [face gallery](faces/README.md) from the same 128×64 pixels, without modifying the book. Rebuild Display and Integration after changing faces.

Firmware build assertions verify selected control logic. Lesson compilation checks syntax/library compatibility. Mesh checks inspect digital surfaces. Book checks inspect EPUB packaging and links. None proves physical operation. Record tool versions and physical outcomes separately in [the test log](TEST-LOG.md).
