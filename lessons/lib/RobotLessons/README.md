# Shared lesson code

Each sketch under `lessons/sketches/` selects its hardware before including RobotLesson.h. These are independently compiled programs, not modes of Companion. Uploading one replaces the previous program.

Read the `.ino` first: its flags tell you which devices belong in the experiment. Then follow these functions in `src/RobotLesson.h`:

| Function | Teaching purpose |
|---|---|
| `begin` | Initialize only the selected devices; no automatic movement |
| `command` | Turn a complete typed command into an action |
| `sample` / `read` | Get a measurement and print the selected function with units |
| `draw` | Turn rectangle coordinates into expressions |
| `tick` | Keep checking input, deadlines, readings and device health |
| `stop` / `outputsOff` | Cancel software activity and request pulse-off |

`src/LessonLogic.h` contains the pure timing, unit conversion and motor trajectory functions. Its compile-time assertions cover the motor envelope, ramps, final center and timer rollover. No lesson uses saved Companion calibration. Servo/Integration use a fixed 500–2500us channel-0 practice sweep and two-step test/run commands, without calibration; they are not suitable for a mounted robot mechanism without a separate mechanical calibration design.

The command parser limits line length and work per loop; oversized input stops activity. Motor starts require a successful initialization and an unexpired preparation, consumed when run begins. Stop/scan cancel preparation. Device health checks run during active tasks; failures request pulse-off, which is not a hardware power disconnect.

Build with robot.ps1 to supply this custom library without a global install. `scripts/check.ps1` compiles current and legacy sketches. `scripts/check_lessons.py` checks course/sketch consistency. These computer checks do not prove physical response, actual PWM frequency or real-time guarantees. Use each lesson's observation and shutdown checks on hardware.
