# Your servo: listing dimensions

Product: **WWZMDiB SG90 Micro Servo Motor**, Amazon ASIN **B0C7KPXPQV**, 12-piece listing. Reviewed 2026-09-28 against the seller image and the owner's matching screenshot.

- [Exact Amazon listing](https://www.amazon.com/dp/B0C7KPXPQV)
- [Seller dimension illustration](https://m.media-amazon.com/images/I/61J0QABchCL._AC_SL1488_.jpg)

| Dimension | Listing evidence | CAD use |
|---|---|---|
| Body width | 12.5 mm explicitly labeled | 12.5 mm base aperture width |
| Body length | 7.9 + 14.7 = 22.6 mm between body-side extension lines | 22.6 mm base aperture length; derived, not directly labeled |
| Ear-to-ear span | 31.8 mm explicitly labeled | Reference only; conflicting chain below |
| Overall height shown | 32 mm | Reference only; not a body-height measurement |
| Below-ear vertical segment | 17.4 mm | Reference only |
| Ear thickness segment | 2.6 mm | Reference only |
| Cable length shown | 250 mm | Cable planning reference |
| Horn overall lengths | 33 / 30 / 18 mm | Reference; not hole spacing or pivot radius |

## A discrepancy in the seller drawing
The upper horizontal chain is 5 + 7.9 + 14.7 + 5 = 32.6 mm. The bottom ear-to-ear dimension says 31.8 mm. Subtracting two 5 mm ears from that bottom total would instead give a 21.8 mm body. Both cannot be exact at once. We use the direct body-side segment sum, 22.6 mm, for this first fit sample and explicitly retain the uncertainty. Do not use this drawing to claim verified shaft or mounting-hole positions; hole spacing/diameter are not dimensioned.

## Updated print
The three apertures are **22.6 x 12.5**, **23.0 x 12.9**, and **23.4 x 13.3 mm**, identified by one, two and three dimples. Added clearance is 0.0, 0.4 or 0.8 mm total in each dimension, not per side. The outer plate remains 94 x 28 x 2.4 mm. The zero-clearance opening is a comparison, not a promise of a sliding fit.

Source: [fit-tests.scad](fit-tests.scad). Print: [servo-fit-gauge.stl](stl/servo-fit-gauge.stl). These replace the prior generic 23.0 x 12.2 mm basis. Record the best printed fit before designing a structural cradle. No physical fit is claimed from the mesh checks.

## First physical fit result — 2026-09-30
Owner reports that the two-dot opening fits best. For this revised gauge that is **23.0 x 12.9mm nominal opening**, with 0.4mm total clearance over the listing-derived basis. Carry this opening into the next cradle prototype; do not treat it as the measured servo dimensions. The deeper cradle still needs a physical fit check. Print settings and actual printed aperture dimensions were not reported.
