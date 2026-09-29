# Teach your robot how far to move

Do this with an adult. We will choose one end position, then the other. **You do not need to type any numbers.**

Use the [Servo lesson](../lessons/plans/06-servo.md) to connect the parts and open the monitor. Its pictures and pin table show the wiring. Keep the motor power off when opening the monitor. Use one bare motor on connector 0, without a leg attached.

## 1. Start the helper

Inside the monitor, type this and press Enter:

```text
calibrate
```

Read the steps on the screen. Ask the adult to check the motor and turn on its power. Keep your hands clear. Then type:

```text
run
```

The motor goes to the middle. Its first move might be big.

## 2. Choose the first end

The screen says **SIDE 1 OF 2**.

- Type `more`. Press Enter. Watch one small move.
- Wait for the next message before typing again.
- Type `more` again if you want to try a little farther.
- Type `back` to come one small step toward the middle.
- When the position works smoothly, type `keep`.

**A small range is OK. Do not try to force the motor to turn farther.** If it sticks or keeps buzzing, type `stop`. Ask the adult to unplug motor power.

The robot will not let you choose the middle as an end. If it says to try another small step, use `more` and watch again.

## 3. Choose the other end

After `keep`, the motor gently returns to the middle. Wait.

The screen says **SIDE 2 OF 2**. Use the same three words:

- `more`: one small step outward on this side.
- `back`: one small step toward the middle.
- `keep`: choose this end position.

After the second `keep`, the screen says **SETUP COMPLETE!** The motor signals turn off. Ask the adult to unplug motor power. Leave USB connected.

## 4. Try the movement

Type:

```text
test
```

Follow the steps on the screen. Ask the adult to turn on motor power when instructed. Then type:

```text
run
```

Watch the motor move between the two positions you chose. It returns to the middle and stops after one minute. Type `stop` to stop sooner. Ask the adult to unplug motor power when it finishes.

Tell your teammate: **Did it move smoothly both ways?** Say YES, NO, or NOT SURE. The robot cannot see whether its motor moved.

## If you get stuck

- Type `help`. The robot repeats your current step.
- If one minute runs out during setup, do not rush. Ask the adult to unplug motor power. Type `calibrate` and follow the screen again. A chosen first end is remembered while USB stays connected; each new session begins at the middle.
- If you mistype a word, the robot shows the words to use.
- Type `stop` whenever you want to stop. It stops sending movement signals; the adult still needs to unplug motor power.

## Notes for the adult

The helper hides scrolling readings during setup so prompts stay visible. Sampling still occurs every 100ms. `numbers` shows the data; `quiet` returns to the simple prompts. Normal one-minute test runs still show data every 100ms.

Setup uses 25us requests, ramped in 5us increments. `more` changes direction automatically for side 2. `back` cannot cross the middle. Both sides require observed positions away from center before `keep` accepts them. Software limits are not mechanical limit detection. Stop before binding and leave margin; this does not certify 180 degrees or suitability for a mounted leg.

On completion, copy the printed `range` line into your notebook. After uploading Integration, you may re-enter that exact line for this same unloaded servo and unchanged setup. Rebooting loses the stored positions. Use `range reset` before changing the servo or mechanism, then run setup again. Starting `calibrate` after completed setup begins a fresh pair of positions.

[Servo calibration background](https://learn.adafruit.com/16-channel-pwm-servo-driver/using-the-adafruit-library).
