// Acceleration includes gravity. Observe X/Y/Z in m/s2.
// Read lessons/plans/ before wiring. Commands: help, status, scan, read, start, test, stop.
// Hardware selection happens at compile time; no Companion firmware is involved.
#define LESSON_KIND 1
#define LESSON_NAME "Accelerometer"
#define LESSON_IMU 1
#define LESSON_OLED 0
#define LESSON_SERVO 0
#include <RobotLesson.h>

void setup() { lesson::begin(); }
void loop() { lesson::tick(); }
