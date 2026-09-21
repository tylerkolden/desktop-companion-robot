# Reproduce book and CAD artifacts

Firmware uses `robot.ps1`; these optional authoring tasks are separate. All project code and inputs are in this repository. Install Python 3 and OpenSCAD only if modifying these artifacts; reading the EPUB and printing existing STLs do not require them.

## Python environment in the repository

Run from the repository root, using your installed Python 3 executable (`python` or Windows `py -3`):

```powershell
python -m venv .venv
.\.venv\Scripts\python.exe -m pip install -r requirements-book.txt
.\.venv\Scripts\python.exe scripts/validate_meshes.py
.\.venv\Scripts\python.exe docs/book/build_book.py
```

The book builder uses Windows Arial fonts from `C:/Windows/Fonts`; that operating-system dependency is explicit. It writes the EPUB, HTML, diagrams, and validation report alongside the tracked manuscript. Review generated changes before committing. The original book is an edition snapshot; current setup and lessons take precedence where workflows have evolved.

## CAD

Use OpenSCAD 2021.01 and the repository-root commands in [cad/README.md](../cad/README.md). No author-specific installation path is required; if OpenSCAD is not on PATH, invoke its installed executable by full path. Bambu Studio slicing still requires checking the actual A1 nozzle, filament, orientation, and scale. Generated printer instructions are not supplied as a universal file.

## Validation scope

Run `./scripts/check.ps1` after setup to parse scripts, check upload/serial argument guards without opening a port, and compile all five sketches. `-SkipBuild` runs only syntax/guard checks. Run `python scripts/check_docs.py` to check local Markdown destinations (not web links or anchors).

Firmware build assertions verify selected control logic. Lesson compilation checks syntax/library compatibility. Mesh checks inspect digital surfaces. Book checks inspect EPUB packaging and links. None proves physical operation. Record tool versions and physical outcomes separately in [the test log](TEST-LOG.md).
