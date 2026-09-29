# Quick motor test

**No calibration. No numbers to enter. No repeated commands.**

Use the [Servo lesson diagram and pin table](../lessons/plans/06-servo.md) for the connections. Use one bare motor on connector 0. Keep motor power off when uploading or opening the monitor.

## Do this

1. In the monitor, type `test`. Press Enter.
2. Ask the adult to check the motor, keep its shaft clear, and turn on its checked 5V supply.
3. Type `run`. Press Enter.
4. Watch the motor move slowly back and forth. You do not need to type anything else.
5. When **DONE** appears after one minute, ask the adult to unplug motor power.

Type `stop` to stop early. If the motor sticks or keeps buzzing, stop and unplug motor power.

Tell your teammate: **Did it move both ways?** Say YES, NO or NOT SURE. To try again, start at step 1.

## What the screen means

| Message | Do this |
|---|---|
| `RUNNING: ... seconds left` | Watch the motor. The test is still running. |
| `DONE: ... finished normally` | The timer finished. Unplug motor power. This is a normal finish. |
| `READY CHECK TIMED OUT` | Nothing moved. Type `test` when you are ready. |
| `FAULT` | Unplug motor power. Keep the message so we can troubleshoot it. |
| `[BOOT]` appears again unexpectedly | The board restarted. Unplug motor power and keep the output. This is different from DONE. |

The board still accepts `status` after a normal finish. Old commands such as `calibrate` and `more` now direct you to `test`; they do not move the servo.

## Adult notes

This replaces the endpoint-finding activity. The preset sends 500–2500 microsecond position requests around 1500us center. It makes a wider sweep, not a full-travel or 180-degree claim. It has not been physically verified on every servo. Use it only with the unloaded bench motor; a mounted leg needs a separate mechanical design and limit check. The first move to center may be larger than later moves.

The one-minute program repeats seven slow eight-second cycles, rests centered for four seconds, then requests all pulses off. It keeps 100ms data rows and periodic countdown messages. `stop` is still available; it does not disconnect power. Nothing starts automatically on boot.

The old calibration had a one-minute cutoff and a software pulse limit. Either could interrupt a long typing session. Without the earlier serial output we cannot determine whether the reported interruption was that cutoff, a reset or another fault. The new messages distinguish these cases; this change does not claim to repair an unobserved electrical fault.
