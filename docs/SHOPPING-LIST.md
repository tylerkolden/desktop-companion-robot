# Amazon-only shopping list

Reviewed September 13, 2026. Links identify the researched ASIN, not a guarantee of seller, price, inventory, accessory inclusion, or unchanged variants. Quantities below distinguish pieces used from packs purchased. Selected does not mean ordered or physically tested.

## Already reported owned

| Component | Used in V1 | Amazon listing | Receipt check |
|---|---:|---|---|
| XIAO ESP32C3, pre-soldered | 1 | [B0DRNSV5CS](https://www.amazon.com/dp/B0DRNSV5CS) | C3 model, installed headers, included antenna |
| SG90 positional servo | 4 | [B0C7KPXPQV](https://www.amazon.com/dp/B0C7KPXPQV) | At least four; horns and horn screws; not continuous-rotation units |
| SSD1306 I2C OLED, 128 x 64 | 1 | [B0F5WPZJ92](https://www.amazon.com/dp/B0F5WPZJ92) | Photos show installed four-pin header; confirm actual delivery |
| Pre-soldered GY-521 MPU6050 | 1 | [B0D2TJVMNY](https://www.amazon.com/dp/B0D2TJVMNY) | Installed header and labeled pin order |
| Mini breadboards | As needed, start with 2 | [B0146MGBWI](https://www.amazon.com/dp/B0146MGBWI) | Check contact layout; used only for logic/sensor distribution |
| Assorted Dupont jumpers | About 16-20, depending on layout | [B01EV70C78](https://www.amazon.com/dp/B01EV70C78) | Male/female types for installed headers and breadboards |
| Bambu Lab A1 | 1 | [B0D17TMWFB](https://www.amazon.com/dp/B0D17TMWFB) | Actual nozzle and available filament still to confirm |
| JLJLUP LiPo batteries | **0** | [B0FH9BLZWB](https://www.amazon.com/dp/B0FH9BLZWB) | Keep out of V1; unsuitable as the planned four-servo supply |

## Additional electronics and tools selected

| Item | Buy | Amazon link | Why / qualification |
|---|---:|---|---|
| Teyleten PCA9685 driver | 1 two-pack; use 1 board | [B0CNVBWX2M](https://www.amazon.com/dp/B0CNVBWX2M) | Product photo shows installed servo pins, one installed control header, capacitor and screw terminal. Other side's empty header holes need not be populated. |
| Fixed 5V/4A wall supply | 1 | [B0D65GK31F](https://www.amazon.com/dp/B0D65GK31F) | Correct advertised rating. Photo shows female barrel-to-terminal accessory; written package description is less explicit. Verify output label, polarity, accessory inclusion and load performance. Certification not independently verified. |
| Haerkn 20-AWG red/black copper wire | 1 pack | [B08F79YG8Q](https://www.amazon.com/dp/B08F79YG8Q) | Short adapter-to-driver power runs. Comes on spools; cut and strip. Tinned copper strands do not require user soldering. |
| WGGE WG-015 stripper/cutter | 1 | [B073YG65N2](https://www.amazon.com/dp/B073YG65N2) | Photo has 20-AWG setting on STRD side. Test on scrap and avoid severing strands. |
| AstroAI digital multimeter | 1 | [B01ISAMUA6](https://www.amazon.com/dp/B01ISAMUA6) | DC voltage, continuity and polarity checks; listed with leads and batteries. |
| Amazon Basics USB-A to USB-C data cable | 1 if needed | [B01GGKYKQM](https://www.amazon.com/dp/B01GGKYKQM) | Supports programming. Requires a USB-A host port. If computer only has USB-C, select a data-capable C-to-C cable instead. |
| Precision screwdriver set | 1 | [B07PV5D3VV](https://www.amazon.com/dp/B07PV5D3VV) | Includes 2.4/3.0mm flatheads and Phillips #00/#0/#1. Choose snug fit. |

## Conditional substitute, not an extra mandatory purchase

If the supply does not include its pictured terminal adapter, [CENTROPOWER B07C7VSRBG](https://www.amazon.com/dp/B07C7VSRBG) provides male/female 5.5 x 2.1mm pigtails with 18-AWG conductors. Use one **female** cable, after confirming the supply plug size and polarity. This can replace both the adapter and the short loose-wire connection. The listing's 12V label does not mean the passive cable creates 12V. Do not buy this pack unless needed; it contains many spares. Check terminal suitability for the prepared wire ends and perform a gentle pull test.

## Still required for the complete physical robot

- [ ] **Filament:** check what is already owned. Plan 1.75mm PLA for initial fit tests; final material/profile follows mechanical testing. No specific Amazon ASIN selected yet.
- [ ] **Fasteners:** servo horns/screws should come with the servos. Body screws, captive nuts, spacers and cable retention await CAD. Avoid buying arbitrary lengths now; no heat-set inserts required in the planned solder-free assembly.
- [ ] **USB power away from a computer:** existing USB supply may suffice for controller only; otherwise select an Amazon USB supply. Current bench plan assumes the computer powers the XIAO.
- [ ] **Measurement tool:** a millimeter ruler can start the measurement sheet; calipers are useful for mounting holes and fit dimensions if available.
- [ ] Verify the terminal adapter is included, control headers are installed, and computer cable matches the host.

**Conclusion:** the selected items support bench bring-up once delivered and checked. A complete enclosure shopping list cannot be frozen until the CAD and physical dimensions are verified.

## Not needed for V1

Soldering iron, solder, battery charger, boost converter, replacement controller, replacement display, camera, Raspberry Pi, or a separate servo tester. Firmware will provide controlled servo calibration. V2 purchases are deferred until measured power requirements are known.
