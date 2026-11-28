// One SG90 with a printed leg on the test stand, channel 0. test prepares; run moves; stop cancels.
// Fixed measuring poses 1000-2000us with 4-second holds. Read docs/BUILD-CHECKPOINT-05.md first.
// Commands: help, status, scan, read, start, test, run, stop, outputs.
#define LESSON_KIND 7
#define LESSON_NAME "Leg"
#define LESSON_IMU 0
#define LESSON_OLED 0
#define LESSON_SERVO 1
#include <RobotLesson.h>

void setup() { lesson::begin(); }
void loop() { lesson::tick(); }
