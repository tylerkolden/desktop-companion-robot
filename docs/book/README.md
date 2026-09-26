# Andrew and the Tiny Robot with Big Ideas

A personalized, approximately 10,600-word robotics adventure for Andrew, age eleven, and his adult building partner. Twenty chapters explain the actual desktop companion project, with five diagrams, a cover, missions, a glossary, eight suggested family sessions, and Kindle delivery instructions.

## Read it

**Current learning path:** the book is the September 15 first-edition snapshot. For today's repository-local terminal setup and standalone lesson sketches, start at [the beginner guide](../START-HERE.md) and [lessons](../../lessons/README.md). These supersede the edition's older workflow descriptions; you do not need to rebuild the book to use the project.

- **Kindle:** upload [the EPUB](Andrew-and-the-Tiny-Robot-with-Big-Ideas.epub) at [Amazon Send to Kindle](https://www.amazon.com/sendtokindle), using the Amazon account registered to your Kindle. Enable Add to your library if offered, send, then connect the Kindle to Wi-Fi and sync. Look under All or Documents if a Books filter hides it. Full instructions, an email alternative, and a child-profile caveat are in Appendix D.
- **Computer:** open [the browser edition](Andrew-and-the-Tiny-Robot-with-Big-Ideas.html). Keep the `images` folder beside it.
- **Edit:** change [manuscript.md](manuscript.md), then regenerate the reading files.

The EPUB contains its own images; upload that single file. Do not upload this entire folder. Delivery through Amazon and rendering on a physical Kindle have not been tested. The book is a personal document, not a Kindle Store publication.

## Contents

Robot systems; electricity; circuit-board components; XIAO ESP32C3; OLED pixels; MPU6050 motion sensing; SG90 servos; PCA9685 pulse timing; I2C/USB/UART/SPI/Wi-Fi; wiring and breadboards; multimeter use; Arduino IDE and a Hello sketch; scheduling and tap detection; staged bring-up; calibration; CAD and printing; debugging; project management and Git; battery planning; iterative improvement.

This edition distinguishes checked code from untested hardware, and describes full-body CAD, walking, Wi-Fi control, and cordless power as future work. It does not introduce a replacement wiring design. Check the latest repository instructions before physical work.

## Rebuild

Requires Python 3 with `Pillow` and `Markdown`. Run `python build_book.py` from this folder. The illustration builder currently uses Arial fonts from `C:/Windows/Fonts`; adjust `FONT` and filenames for a different operating system. It regenerates images, EPUB, HTML, and `VALIDATION.txt`. No network requests occur during the build.

Validation includes XML parsing, EPUB packaging, navigation/local resource targets, image alternative text, and manifest/spine consistency. All six original images were visually inspected. A 600-pixel browser reading preview was inspected and showed no horizontal overflow. These checks are not a substitute for Amazon conversion or physical Kindle testing.

Original educational text and diagrams created for the family with OpenAI Codex assistance. ANDI dialogue is fictional. Technical sources are linked inside the book. First edition: September 15, 2026.

The current Appendix A is the seven-lesson standalone course: Basics, Accelerometer, Gyroscope, Temperature, Display, Servo and Integration. Regenerate with `python scripts/build_wiring.py` then `python docs/book/build_book.py`. The lesson plans and code replace the old stacked Companion exercises.
