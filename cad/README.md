# CAD workspace

Original fit gauges are available now. **These are measurement samples, not the final enclosure or structural servo mounts.** The owner has tested the two-dot opening and reported loose V2 retention. V4 screw retention is not physically tested yet.

| File | Purpose | Overall dimensions |
|---|---|---|
| [servo-fit-gauge.stl](stl/servo-fit-gauge.stl) | Compare three servo-body clearances | 94 x 28 x 2.4mm |
| [oled-fit-gauge.stl](stl/oled-fit-gauge.stl) | Check PCB outline clearance with open header edge | 32 x 32.3 x 2.4mm |
| [fit-tests.scad](fit-tests.scad) | Editable OpenSCAD source for both | Parameters in millimeters |

![Servo clearance gauge](fit-tests-preview.png)

## Print and measure

1. Import STL into Bambu Studio, select your A1 and actual nozzle. Use **100% scale** and verify displayed dimensions.
2. For a standard 0.4mm nozzle and PLA, start with 0.20mm layers and 3 walls. Lay flat as exported; supports should not be needed. These are provisional settings; assess the sliced preview before printing.
3. With power off, pass the servo cable through a notch and gently try the servo body in each opening. Do not force it. The ears stay above the gauge.
4. One dimple marks a 22.6 x 12.5mm aperture, two mark 23.0 x 12.9mm, and three mark 23.4 x 13.3mm. Report which is snug without squeezing. The notches are cable escape gaps, not final cable channels.
5. The OLED gauge's opening is 28.0 x 28.3mm, based on an unmeasured nominal PCB. Hold the module by its PCB and compare the outline. It has no retaining lip: **support the module so it cannot fall through**. Do not press the display glass. A too-small opening is useful measurement feedback, not a reason to force insertion.
6. Record fit, measured board sizes and photos in the measurement sheet. The visible-screen window, screw holes and final mounts are still to be designed.

## Reproduce exports

OpenSCAD 2021.01 was used. From repository root:

```powershell
openscad -o cad/stl/servo-fit-gauge.stl -D 'part="servo"' cad/fit-tests.scad
openscad -o cad/stl/oled-fit-gauge.stl -D 'part="oled"' cad/fit-tests.scad
python scripts/validate_meshes.py
```

The mesh check verifies closed edges, winding, one connected shell, positive volume and expected bounds. It does not verify real-world dimensions, part fit, print strength or printer output. Update expected bounds if deliberately changing the source dimensions.

Next: use [actual measurements](../docs/MEASUREMENTS.md) and gauge results to create the servo cradle, OLED mount, lightweight chassis, legs, removable shell and power-cord strain relief. Final fasteners and STLs await those checks.

Servo openings use your Amazon WWZMDiB SG90 listing, with three clearance choices. See [dimension sources and the seller drawing discrepancy](SERVO-DIMENSIONS.md). Use the regenerated STL; an earlier copy used generic SG90 dimensions.

## First servo cradle

The owner reports the two-dot gauge opening fits best. [Print and check one cradle](../docs/BUILD-CHECKPOINT-02.md), using that opening and adjustable mounting slots. [STL](stl/servo-cradle-v1.stl) / [editable source](servo-cradle-v1.scad). This is an untested fit prototype, not yet a load-bearing robot mount.

## Historical prototype: screw-free V2

Use [checkpoint 3](../docs/BUILD-CHECKPOINT-03.md) and [V2 STL](stl/servo-cradle-v2.stl). It replaces V1 for the next fit test: side-entry cable channel and four releaseable snap tabs. V1 remains as design history; its closed opening obstructed the side cable during installation. V2 retention and durability are unverified.

## Historical revision: V3 retention adjustment

The owner reports loose V2 hooks. Use [V3 STL](stl/servo-cradle-v3.stl) and [updated checkpoint 3](../docs/BUILD-CHECKPOINT-03.md). Each hook reaches 1.0mm farther inward and sits 0.2mm lower; body opening and cable channel are unchanged. This remains an unverified fit prototype. V1/V2 are retained for comparison.

## Current prototype: V4 screw retention

Use [checkpoint 4](../docs/BUILD-CHECKPOINT-04.md) and [V4 STL](stl/servo-cradle-v4.stl). Two M2 x 10mm screws, two M2 nuts and four flat washers retain the servo. The 44 x 30 x 4mm plate preserves the proven opening and moves cable entry to the long side to keep both screw supports intact. V1–V3 are design history; do not print snap-tab versions for the next checkpoint. Physical validation remains pending.

## Current: Leg V1 and single-leg stand

Use [checkpoint 5](../docs/BUILD-CHECKPOINT-05.md). [Leg STL](stl/leg-v1.stl) (14 x 70 x 8mm) has a pocket for the SG90 double-arm horn, sized from the 33mm listing length with 0.6mm clearance, a 5.5mm screwdriver hole over the shaft screw, and 6mm of plastic under the horn for the horn screws. [Stand STL](stl/leg-stand-v1.stl) (80 x 60 x 58mm) holds a V4 mount upright by its continuous long edge so the shaft sits about 65mm above the table. Source: [leg-v1.scad](leg-v1.scad), `part="leg"` or `part="stand"`. Horn hub and tip widths are assumptions, not measurements. No print, fit or motion test yet.

## Leg V2 (replaces Leg V1)

Leg V1 was printed by the owner and did not fit the horn. [Leg V2](stl/leg-v2.stl) (13 x 70 x 5mm, [source](leg-v2.scad)) follows the owner's horn as measured from grid-sheet photos: about 35.7mm long, with a 7.2mm spline boss and a raised 8.7mm disc on the screw side. The horn's flat side goes into the pocket. An M2 x 6 screw through the leg and horn into the shaft clamps it. A [horn gauge](stl/horn-gauge.stl) and [measuring sheet](../docs/measure/README.md) support re-measuring. Not yet printed.

## Leg V3 (replaces Leg V2)

Andrew noticed from close-up photos that the horn's screw side is a straight-sided disc (about 8.0-8.7mm across, about 1.1mm tall) with small ramps where it joins the arm; from the side the ramps make it look sloped. [Leg V3](stl/leg-v3.stl) (13 x 70 x 5.3mm, [source](leg-v3.scad)) has a 9.4mm round recess 1.3mm deep for the disc, plus a sloped clearance (9.4mm at the bottom to 13.4mm at the arm face, trimmed to the arm pocket) for the ramps. V2's recess was only 1.0mm deep. The leg is 0.3mm thicker to keep 2mm of plastic under the hub. Owner's printed gauge: arm 2.0mm, spline boss 7.5mm. Not yet printed.

## Leg V4 (replaces Leg V3)

The owner printed V3: the horn would not go in and the screw holes were too small. [Leg V4](stl/leg-v4.stl) (14 x 70 x 5.8mm, [source](leg-v4.scad)) keeps the V3 layout with more clearance: 0.5mm per side around the horn arms (was 0.2), a 10.0mm x 1.5mm disc recess (was 9.4 x 1.3), a 2.2mm pocket (was 2.0), a 2.9mm shaft-screw hole (was 2.4) and 1.6mm pilot holes (was 1.0). The leg is wider and thicker to keep 2mm walls and floor. Not yet printed.

## Leg V5 (replaces Leg V4)

Owner's screw plan: the servo's own centre screw holds the horn on the shaft, and two M2 x 4 screws go down through the leg into the horn's 4th arm holes (13.4mm from centre), cutting their own thread in the horn plastic. [Leg V5](stl/leg-v5.stl) (14 x 70 x 5.8mm, [source](leg-v5.scad)) keeps V4's clearances, replaces the centre hole with a 5.5mm screwdriver hole, and adds two 2.6mm side holes with 4.4mm x 1.2mm head recesses on the outside face. Not yet printed.

## Leg V6 (replaces Leg V5) — current

Owner found that the servo bag has the right screws: two large pointed self-tapping screws (about 2.2mm thread, 7mm shank, 4.7mm flanged head) and one small screw (about 2.0mm thread, 3.5mm shank, 2.8mm head), measured from grid-sheet photos. [Leg V6](stl/leg-v6.stl) (14 x 70 x 8.5mm, [source](leg-v6.scad)): the large screws go up through the horn's 4th arm holes (13.4mm from centre) into 2.0mm blind pilot holes 5.6mm deep; the small screw holds the horn on the shaft, reached through a 6.0mm centre hole. Keeps V4 clearances (0.5mm per side, 10.0 x 1.5mm disc recess, 2.2mm pocket). The leg is 8.5mm thick so the 7mm screws stay blind. The large screw heads sit under the horn: their clearance to the servo case through the swing is not verified. Not yet printed.

## Leg test stand V2

The owner printed stand V1: the mount sat loose in the 4.3mm slot and leaned badly. The mount plate's real thickness is not measured, so [stand V2](stl/leg-stand-v2.stl) ([source](leg-stand-v2.scad)) has a 6.0mm slot, 4mm walls and two [wedges](stl/leg-stand-v2-wedge.stl) (print two) pushed in at the slot ends beside the plate. The wedges taper 0.8 to 3.8mm, covering plates about 2.2 to 4.7mm thick, and sit outside the servo body and ear screws. Slot depth stays 8mm because the servo body sticks out of the plate just above the tower top. Not yet printed.

## Puppy V2 (checkpoint 6, current)

![Puppy V2](dog-puppy-v2.png)

Owner feedback on V1 (2026-11-27): the dog must be able to lie flat with the legs swung at least 90 deg, and the head was too big for its small screen face. V2 fixes both:

- The body shell is slimmer (64 wide) than the legs, so the legs and their round thighs stay **outside** it and can swing all the way round. U-slots, open at the bottom, slide over the servo gear towers; the round thighs hide them. Lying down, the dog rests on its tummy and thighs.
- The breadboard and XIAO moved into the body, so the head is small (50 x 56 x 54) and the screen window (25 x 17) is most of the face. No neck, like a chubby puppy. Bigger, rounder ears.

Shared numbers live in `dog-puppy2-common.scad` (whole-dog coordinates: x forward, z=0 under the body frame).

| Part | File | Print |
|---|---|---|
| Body frame V2: same as V1 but the 4 deck posts moved in to x=+/-10 | `dog-body-v2.scad` | flat, no supports |
| Deck V3: PCA9685 at the back (header pointing to the tail), breadboard sideways at the front (XIAO USB-C to the left), 2 wire slots, 4 shell screw blocks | `dog-deck-v3.scad` | flat, no supports |
| Body shell V2: slim capsule, curly tail, 4 gear U-slots, USB-C hole on the left, OLED wire hole and 2 chin screw holes on top | `dog-shell-v2.scad` | open side down, tree supports |
| Head shell V2: small round head, big screen window, nose | `dog-head-v2.scad` (`part="shell"`) | open side down, tree supports |
| Chin plate V2: OLED slot, ear posts; screws to the body roof | `dog-head-v2.scad` (`part="chin"`) | flat, no supports |
| Ear V2 (print 2) | `dog-ears-v2.scad` | flat, no supports |
| Leg V8 (print 4): Leg V6 hip unchanged + round paw + 32mm round thigh | `leg-v8.scad` | pocket up, no supports |

Fasteners: 4 big servo-bag screws (deck to body); 6 M2 x 10 + nuts (PCA, MPU); 2 M2 x 10 + nuts (chin to body roof, before the shell goes on); 4 M2 x 10 (shell sides into the deck blocks); 2 M2 x 10 (ears through head into the chin posts).

Build order: servos into the body, deck on, boards on, chin plate onto the shell roof, shell down over the deck and screwed at the sides, OLED in, head shell and ears on, legs last.

Not checked yet: the 90 deg swing is clear of the body in the CAD, but whether each servo can reach +/-90 deg from straight down depends on where the horn lands on the spline; the program will find out per leg. Gear tower size, leg position and servo protrusion come from the SG90 drawing, not a measurement. Board sizes from the owner's grid photos (about +/-1mm). Nothing in this section has been printed or fitted.

Reproduce: `openscad -o cad/stl/<name>.stl cad/<file>.scad` (add `-D part="chin"` for the chin plate; the ear STL is `dog-ear-v2.stl`), then `python3 scripts/validate_meshes.py`. Picture: `cad/dog-v2-assembly.scad` with `-D pose="stand"` or `-D pose="lie"`.

## Puppy V1 (checkpoint 6, replaced by V2 before printing)

![Puppy V1](dog-puppy-v1.png)

The owner asked for a cute, lovable dog that still works. The tested parts stay inside; the cute parts go on the outside. Shared numbers live in `dog-puppy-common.scad` (whole-dog coordinates: x forward, z=0 under the body frame).

| Part | File | Print |
|---|---|---|
| Body frame: 4 servos (V4 openings), 4 deck posts, MPU6050 floor posts | `dog-body-v1.scad` | flat, no supports |
| Deck V2: PCA9685 (header forward), hollow neck for wires, 2 rear rests | `dog-deck-v2.scad` | flat, no supports |
| Body shell: one round bean (150 x 120) with curly tail; hides frame, servos and hips; legs poke out the open belly | `dog-shell-v1.scad` | open side down, tree supports |
| Head shell: round head, screen window, nose, USB hole; lower edge hugs the body | `dog-head-v1.scad` (`part="shell"`) | open side down, tree supports |
| Chin plate: OLED slot, mini breadboard (XIAO), ear posts | `dog-head-v1.scad` (`part="chin"`) | flat, no supports |
| Ear (print 2) | `dog-ears-v1.scad` | flat, no supports |
| Leg V7 (print 4): Leg V6 hip unchanged + round paw | `leg-v7.scad` | pocket up, no supports |

Fasteners: 4 big servo-bag screws (deck to body); 6 M2 x 10 + nuts (PCA, MPU); 4 M2 x 10 (chin to neck, ears through head into the chin posts).

Owner feedback (2026-11-27): the first versions looked boxy, then piecemeal; this version encloses the whole body.

Estimates, not measurements: board sizes from the owner's grid photos (about +/-1mm); PCA hole spacing 57.2 x 19.8 and MPU 15.5 (3.4mm holes forgive about 0.7mm); leg position and servo protrusion from the SG90 drawing. Nothing in this section has been printed or fitted. `dog-deck-v1.scad` (all electronics on one long deck) was replaced before printing; it is kept for history.

Reproduce: `openscad -o cad/stl/<name>.stl cad/<file>.scad` (add `-D part="chin"` for the chin plate), then `python3 scripts/validate_meshes.py`. Picture: `cad/dog-v1-assembly.scad`.
