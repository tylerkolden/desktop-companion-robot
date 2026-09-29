# Teach the robot its servo's usable travel

**Mission:** find comfortable endpoints on one unloaded channel-0 servo, then sweep through that whole checked range. A position command does not measure the shaft. This is not a promise of 180 degrees and does not mean pushing against a mechanical stop.

Use the [Servo lesson's starting setup, complete wiring diagram, pin table and power check](../lessons/plans/06-servo.md). Keep the shaft clear, with no leg or mechanism attached. Open the monitor with the motor supply disconnected, as that lesson explains. This page changes commands, not wiring. Servo and Integration both support them; use Servo for the first calibration.

## Predict

Which way will the shaft move when its pulse width decreases? Write a prediction. The pulse is a position request in microseconds; it is not a number of degrees. Increasing and decreasing pulse width move in opposite directions, whose appearance depends on how you view the servo.

## Prepare without movement

In the monitor, type each line separately and press Enter:

```text
range reset
calibrate
```

`range reset` clears old endpoints. Use it whenever you change servo or mechanism. `calibrate` shows the guide and starts no movement. After the lesson's adult power check, connect motor power and type `run` within 60 seconds. It requests 1500us center; this initial movement can be substantial from an unknown starting position.

## Find comfortable limits, one step at a time

1. Type `lower`. It requests 25us less, ramped in small increments. Wait until it finishes and watch the actual shaft before asking for another step. Do not paste a batch of commands.
2. Repeat only while motion is free and useful. Do not intentionally seek a stall. If it binds, stops responding to position changes or buzzes continuously, type `stop` and unplug motor power; do not continue pushing. Choose a comfortably smaller range when restarting.
3. At a low position you have observed working comfortably, type `marklow`. This records your observation, not an automatic measurement. Low must be at or below 1450us.
4. Type `center` and wait for the gentle return to 1500us. Then use `higher`, one step at a time, to find a comfortable high position. Type `markhigh` when checked. High must be at or above 1550us.
5. Type `finish`. It turns pulses off and, if both marks are present, accepts the range. Unplug motor power. Write down the **exact `range` command printed by the board** for this servo.

Each calibration session ends automatically after **60 seconds**, even if you keep typing. `stop` also ends it. Endpoint marks survive within this powered session, so you can use `calibrate`, perform the power check, then `run` again to continue; each session starts at center. `status` shows whether each endpoint has been marked. Only `finish` accepts both marks as a checked range. Restarting the board loses all marks and limits.

The software refuses requests outside 500–2500us. Those numbers are **software ceilings, not recommended endpoints**. Your particular servo may need a much narrower range. Leave margin for load and variation; a mounted robot leg needs its own mechanical calibration.

## Sweep the complete checked range

With motor power disconnected and the monitor already open:

```text
test
```

Read the printed low/high values. After the adult power check, connect motor power and type:

```text
run
```

The one-minute run repeats this eight-second path seven times:

| Time within each cycle | Requested motion |
|---|---|
| 0–2 seconds | Center to checked low endpoint |
| 2–2.5 seconds | Brief low-end hold |
| 2.5–5.5 seconds | Across the full checked range to high |
| 5.5–6 seconds | Brief high-end hold |
| 6–8 seconds | Return to center |

It then rests at center for four seconds and requests all pulses off. `stop` cancels at any time; it does not disconnect electrical power. Remove motor power afterward. The monitor reports requested pulse widths every 100ms, not measured angles. The first centering request is not a ramp from a known physical position.

## Reuse and verify

After uploading Integration, re-enter the exact `range LOW HIGH` command the board printed for this **same servo and unchanged unloaded setup**. Replace LOW and HIGH with your recorded numbers; do not copy someone else's endpoints. The command only sets limits and stops existing activity. Follow with `test`, the power check and `run`. If no notebook record exists, calibrate again. No endpoints are loaded from Companion or retained through a reboot.

Record whether the shaft reaches both checked endpoints smoothly, returns to center, responds to `stop`, and does not restart. Record YES / NO / NOT SURE. An `[OK]` driver reply cannot answer these questions for you.

[Adafruit's servo calibration guidance](https://learn.adafruit.com/16-channel-pwm-servo-driver/using-the-adafruit-library) explains why endpoint pulse widths vary and why travel should be approached gradually.
