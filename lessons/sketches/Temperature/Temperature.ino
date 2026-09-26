// The MPU6050 measures its own chip temperature in Celsius.
// Read lessons/plans/ before wiring. Commands: help, status, scan, read, start, test, stop.
// Hardware selection happens at compile time; no Companion firmware is involved.
#define LESSON_KIND 3
#define LESSON_NAME "Temperature"
#define LESSON_IMU 1
#define LESSON_OLED 0
#define LESSON_SERVO 0
#include <RobotLesson.h>

void setup() { lesson::begin(); }
void loop() { lesson::tick(); }
