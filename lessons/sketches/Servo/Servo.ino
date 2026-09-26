// One unloaded channel-0 SG90. test prepares; run moves; stop cancels.
// Read lessons/plans/ before wiring. Commands: help, status, scan, read, start, test, stop.
// Hardware selection happens at compile time; no Companion firmware is involved.
#define LESSON_KIND 5
#define LESSON_NAME "Servo"
#define LESSON_IMU 0
#define LESSON_OLED 0
#define LESSON_SERVO 1
#include <RobotLesson.h>

void setup() { lesson::begin(); }
void loop() { lesson::tick(); }
