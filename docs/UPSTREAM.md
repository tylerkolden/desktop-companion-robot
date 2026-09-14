# TN-24 study and provenance

Reviewed upstream commit `9e18fde58cf5a4424d7f16e75b49f69bfb079adf` (January 11, 2025), using a local read-only study clone. Observations reflect the checked version, not an assertion about all future releases.

## What was found

The checked repository has README.md, TN24.ino and expressions.h. Its three-commit history contains no STL, STEP, Fusion or 3MF files. No releases/tags were returned during the study. The main trees of the three listed public forks also contained those same three files.

An open issue requests the 3D models; its one inspected reply repeats the request, with no download. The Electromaker and Hackster articles describe a printed enclosure and browser interface, but the checked firmware has no web server or Wi-Fi implementation. The article's component table says one servo while the text and firmware use four.

## Source-review findings to address

- TAP_WINDOW is defined but not enforced; two events far apart can count as a double tap.
- Blocking delays throughout movement and expression routines postpone sensing.
- Active behavior updates lastActionTime, conflating activity with user interaction and undermining intended idle sleep.
- Several routines command extreme servo positions without per-leg calibration or geometry limits.
- Orientation logic uses a simple Z threshold without hysteresis or a documented mounting transform.
- The code directly drives D0-D3 servos. This project's PCA9685 architecture requires new motion control.

These are static-review findings. The upstream code was not compiled or tested on the owner's hardware.

## Reuse status

The project articles display an Apache-2.0 label, but the checked GitHub tree has no LICENSE file. This repo links to upstream rather than copying its code or assuming blanket permission for unpublished models. Decide and document reuse terms before importing source or publishing derivatives. No legal conclusion about the scope of the article label is made here.

## Sources

- [Original repository](https://github.com/tech-nickk/TN24)
- [Reviewed firmware revision](https://github.com/tech-nickk/TN24/blob/9e18fde58cf5a4424d7f16e75b49f69bfb079adf/TN24.ino)
- [Missing 3D models issue](https://github.com/tech-nickk/TN24/issues/1)
- [Electromaker article](https://www.electromaker.io/project/view/tn-24-v20-cute-desktop-companion-robot)
- [Hackster article](https://www.hackster.io/tech_nickk/tn-24-v2-0-cute-desktop-companion-robot-44a472)
- [Seeed controller documentation](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [PCA9685 power/reference wiring](https://learn.adafruit.com/16-channel-pwm-servo-driver/pinouts)
- [Bambu A1 specifications](https://bambulab.com/en/a1/tech-specs)
- Exact Amazon ASINs and receipt checks are in [SHOPPING-LIST.md](SHOPPING-LIST.md).
