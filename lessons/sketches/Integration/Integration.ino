// Combine the independently verified devices; observe all five functions.
// Read lessons/plans/ before wiring. Commands: help, status, scan, read, start, test, stop.
// Hardware selection happens at compile time; no Companion firmware is involved.
#define LESSON_KIND 6
#define LESSON_NAME "Integration"
#define LESSON_IMU 1
#define LESSON_OLED 1
#define LESSON_SERVO 1
#include <RobotLesson.h>

void setup() { lesson::begin(); }
void loop() { lesson::tick(); }
