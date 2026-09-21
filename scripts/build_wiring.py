"""Rebuild the labelled wiring atlas and embedded documentation (requires Pillow).

Run from the repo: python scripts/build_wiring.py
These are custom educational illustrations, not Tinkercad simulations.
"""
from pathlib import Path
import re
import os
import shutil
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'docs/wiring'
IMAGES = OUT / 'images'
IMAGES.mkdir(parents=True, exist_ok=True)
COLORS = ['#bc2434', '#263342', '#007b83', '#7951ae']
FONT = next((p for p in [Path('C:/Windows/Fonts/arial.ttf'), Path('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf')] if p.exists()), None)
def font(n):
    return ImageFont.truetype(str(FONT), n) if FONT else ImageFont.load_default(size=n)
def canvas(title, subtitle):
    im = Image.new('RGB', (1600, 1000), '#f4f7fa')
    d = ImageDraw.Draw(im)
    d.text((45, 25), title, font=font(38), fill='#102b40')
    d.text((45, 85), subtitle, font=font(23), fill='#334c60')
    d.text((45, 935), 'Custom Tinkercad-style illustration | Functional pins, NOT physical header order | V1 proposed', font=font(22), fill='#334c60')
    return im, d
def board(d, rect, title, subtitle=''):
    d.rounded_rectangle(rect, 22, fill='#126b69', outline='#064341', width=3)
    x,y,_,_ = rect
    d.text((x+22,y+20), title, font=font(29), fill='white')
    d.text((x+22,y+65), subtitle, font=font(20), fill='white')
def save(im, name):
    im.save(IMAGES / (name+'.png'))

MODULES = {
    'oled': ('AITRIP SSD1306 OLED', 'B0F5WPZJ92 | 0.96 inch, 128 x 64'),
    'imu': ('GY-521 / MPU6050', 'B0D2TJVMNY | motion sensor'),
    'pca': ('Teyleten PCA9685', 'B0CNVBWX2M | logic only; V+ OFF'),
}
rows = [('3V3','VCC'), ('GND','GND'), ('D4 / GPIO6','SDA'), ('D5 / GPIO7','SCL')]
for key,(name,sub) in MODULES.items():
    im,d=canvas(name+' - four-wire lesson', 'Unplug the XIAO USB cable before changing wires. USB is the power source for this experiment.')
    board(d,(50,165,520,735),'Seeed XIAO ESP32C3','B0DRNSV5CS | USB-C power + data')
    board(d,(1080,165,1550,735),name,sub)
    # Breadboard-inspired junctions show electrical groups, not hole coordinates.
    d.rounded_rectangle((665,270,935,745),18,fill='white',outline='#a9b6c3',width=3)
    d.text((664,223),'Junction groups',font=font(26),fill='#102b40')
    for i,(left,right) in enumerate(rows):
        y=340+i*108
        d.text((90,y-17),left,font=font(28),fill='white')
        d.text((1325,y-17),right,font=font(28),fill='white')
        d.line([(470,y),(1120,y)],fill=COLORS[i],width=7)
        for x in [690,740,790,840,890]:
            d.ellipse((x-8,y-8,x+8,y+8),fill=COLORS[i],outline='#102b40',width=2)
        for x in [470,1120]: d.rectangle((x-9,y-9,x+9,y+9),fill='#e7bd62')
    d.text((65,780),'Each drawn row is ONE net. Different rows must be isolated. Check your breadboard unpowered.',font=font(25),fill='#102b40')
    note={'oled':'Read OLED labels: the supplied front-view photo shows GND / VCC / SCL / SDA; this diagram rearranges them.',
          'imu':'Leave XDA, XCL and INT open. AD0 defaults low on typical modules: verify address 0x68 / 0x69.',
          'pca':'VCC is 3.3V logic. Leave V+ and numbered outputs empty for this communication test. OE: verify pull-down; ground only if floating.'}[key]
    d.text((65,833),note,font=font(22),fill='#102b40')
    d.text((65,880),'XIAO 5V stays unused. Locate every terminal by its printed label, never by this drawing position.',font=font(24),fill='#a12832')
    save(im,key)

im,d=canvas('USB-only: identify / Hello / upload', 'All external header pins are unused during the bare-board lesson.')
board(d,(70,240,535,650),'Computer USB','Data-capable cable')
board(d,(1060,240,1530,650),'XIAO ESP32C3','USB-C socket; leave headers empty')
d.line([(535,435),(1060,435)],fill='#346aab',width=20)
d.text((590,370),'USB power + data',font=font(29),fill='#102b40')
d.text((80,735),'Use the USB data cable with the bare XIAO. Leave its header pins empty. Do not hand-wire USB contacts.',font=font(28),fill='#102b40')
d.text((80,805),'Connecting USB can reset the board. A detected COM port does not prove which program is installed.',font=font(25),fill='#102b40')
save(im,'usb')

im,d=canvas('Complete circuit: XIAO + PCA9685 + one SG90', 'USB and the 5V supply must be unpowered while wiring. Read actual board labels, not pictured positions.')
board(d,(40,150,490,410),'Regulated 5V supply','Low-voltage output only')
board(d,(1000,150,1560,760),'Teyleten PCA9685','VCC = logic; V+ = motor power')
d.rounded_rectangle((1020,270,1540,395),12,fill='#72b64a',outline='#1d4426',width=3)
d.text((1050,275),'GREEN TWO-SCREW BLOCK',font=font(24),fill='#102b40')
for y,label,col in [(325,'+ / V+',COLORS[0]),(375,'- / GND',COLORS[1])]:
    d.line([(490,y),(1070,y)],fill=col,width=7)
    d.ellipse((1060,y-10,1080,y+10),fill='#d9dfe3',outline='#102b40',width=2)
    d.text((1100,y-17),label,font=font(27),fill='#102b40')
d.text((545,280),'Positive +5V',font=font(25),fill=COLORS[0])
d.text((545,397),'Negative / return',font=font(25),fill=COLORS[1])
board(d,(40,450,490,810),'XIAO ESP32C3','Powered by computer USB')
for i,(left,right) in enumerate(rows):
    y=570+i*60
    d.text((65,y-16),left,font=font(26),fill='white')
    d.line([(460,y),(1050,y)],fill=COLORS[i],width=6)
    d.text((1080,y-16),right,font=font(27),fill='white')
d.text((535,510),'All FOUR logic wires stay connected',font=font(23),fill='#102b40')
d.text((1190,625),'Shared board GND',font=font(23),fill='white')
board(d,(1000,840,1560,910),'One SG90 on channel 0')
for x,label,col in [(1090,'GND',COLORS[1]),(1280,'V+ (+5V)',COLORS[0]),(1480,'PWM','#a05b00')]:
    d.line([(x,760),(x,840)],fill=col,width=6)
    d.text((x-30,790),label,font=font(21),fill=col)
d.text((45,845),'Keep XIAO 5V unused. Never connect supply +5V to XIAO 3V3.',font=font(23),fill='#a12832')
d.text((45,886),'Green-block GND, header GND and output GND share the board ground.',font=font(22),fill='#102b40')
save(im,'servo')

im,d=canvas('Meter: measure voltage without making a short', 'Adult check of LOW-VOLTAGE adapter output only. Disconnect robot electronics for this check.')
board(d,(70,220,580,650),'AstroAI multimeter','B01ISAMUA6 | DC volts, 20V range')
board(d,(1070,220,1530,650),'Adapter output','Barrel-to-terminal + and - labels')
for y,label,col in [(410,'Red V/ohm jack -> positive',COLORS[0]),(555,'Black COM jack -> negative',COLORS[1])]:
    d.line([(580,y),(1070,y)],fill=col,width=8)
    d.text((610,y-65),label,font=font(23),fill=col)
d.text((80,725),'Voltage probes go across the output. Expect approximately +5V with correct polarity.',font=font(28),fill='#102b40')
d.text((80,785),'Do not use the current jack or current mode across a supply. Do not open or probe mains wiring.',font=font(25),fill='#a12832')
d.text((80,845),'Continuity is a DIFFERENT test: disconnect all power, then check breadboard contact groups.',font=font(25),fill='#102b40')
save(im,'meter')

TABLES={
 'usb':[('Computer USB port','XIAO USB-C socket','Data cable; all headers empty')],
 **{k:[('XIAO '+a,name+' '+b,['3.3V logic','Shared ground','I2C data','I2C clock'][i]) for i,(a,b) in enumerate(rows)] for k,(name,_) in MODULES.items()},
 'servo':[('Adapter positive','PCA green block: + / V+','Regulated 5V; direct power wire'),('Adapter negative','PCA green block: - / GND','Direct power wire'),('XIAO 3V3','PCA VCC','Logic power, separate from V+'),('XIAO GND','PCA GND','Common reference'),('XIAO D4 / GPIO6','PCA SDA','I2C data'),('XIAO D5 / GPIO7','PCA SCL','I2C clock'),('PCA channel 0 GND','SG90 ground','Usually brown/black; verify'),('PCA channel 0 V+','SG90 power','Usually red; verify'),('PCA channel 0 PWM','SG90 signal','Usually orange/yellow; verify')],
 'meter':[('Black probe / COM jack','Adapter negative output','Voltage check only'),('Red probe / V-ohm jack','Adapter positive output','DC volts; never current mode')]
}
NOTES={
 'usb':'The pinout for this stage is USB-C socket only: **all XIAO header pins are unused**.',
 'oled':'Functional pinout: VCC, GND, SDA, SCL. The supplied display photo has **GND / VCC / SCL / SDA** across its top header when viewed from the front. Our illustration groups wires by function, not that physical order.',
 'imu':'Functional pinout: VCC, GND, SCL, SDA, XDA, XCL, AD0, INT. Leave XDA/XCL/INT open; verify AD0 default through scanning. Exact delivered header order is unconfirmed. Check regulator/pull-ups at 3.3V; do not substitute 5V to fix detection.',
 'pca':'The green two-screw block is a separate power input: one terminal is + / V+, the other is - / GND. It stays empty during this communication-only test; the driver electronics receive power through VCC. Follow the board markings, not a guessed left/right position. Functional pinout: VCC = logic 3.3V; V+ = separate output-power input; GND = common ground; SDA/SCL = I2C; OE = active-low output enable. Exact clone header order is unconfirmed. Verify OE pull-down; ground OE only if floating. Use only VCC, GND, SDA and SCL for this communication test. Leave V+ and the numbered output connectors empty.',
 'servo':'**Identify the green two-screw block on the PCA9685:** its + / V+ terminal accepts the regulated 5V supply positive wire; its - / GND terminal accepts the negative wire. They are not both V+. This block is not VCC, and it is different from the separate barrel-to-terminal connector on the supply cable. Confirm the actual board markings with power unplugged; the illustration does not establish left/right orientation. Functional servo pinout: ground, power, signal. Read the driver row labels before inserting the plug. **Keep all four XIAO logic wires connected**, as shown in this complete diagram: 3V3/VCC, GND/GND, D4/SDA and D5/SCL. The shared-ground wire is one of these four wires, not an extra fifth wire. Use only channel 0 for this experiment. USB and adapter grounds meet at PCA GND; adapter positive never meets XIAO 5V or 3V3. Grounded OE is not a reset interlock: unplug servo power for uploads, resets and horn fitting.',
 'meter':'Probe-jack pinout is functional, not the physical socket position of every meter variant. Select DC voltage before probing. Verify barrel size and polarity against the actual adapter; do not assume center-positive from a generic picture.'
}
def snippet(keys, parent, book=False, introduction=''):
    if not introduction and ('servo' in keys or 'meter' in keys):
        introduction = '**Parts for this power checkpoint:** a regulated 5V wall supply, its low-voltage connector and prepared wires, and a multimeter. For the motor connection, also identify the SG90 servo and PCA9685 driver. The supply powers the motor through the driver; the meter checks its output before connection. Identify these parts before following the diagram.'
    if 'servo' in keys or 'meter' in keys:
        preparation = '**Before assembling these connections: close the monitor and unplug the XIAO USB cable. Keep the newly introduced 5V supply unplugged from the wall until the voltage-check step. On a repeat session, unplug it before changing wires.**'
    else:
        preparation = '**Before changing wires: close the serial monitor if it is open, then unplug the XIAO USB cable. USB is the power source for this experiment.**'
    parts=['<!-- wiring:start -->']
    if introduction:
        parts += [introduction, '']
    parts += ['### Wiring visuals and pinouts','',
           preparation+' These are functional illustrations: locate terminals by their printed labels. Board shapes do not establish physical pin positions.','']
    for k in keys:
        image=('images/wiring-'+k+'.png') if book else Path(os.path.relpath(IMAGES/(k+'.png'),parent)).as_posix()
        parts += [f'![{k.upper()} wiring: labelled endpoints and separate power paths]({image})','',NOTES[k],'','| From / pin | To / pin | Meaning |','|---|---|---|']
        parts += [f'| {a} | {b} | {c} |' for a,b,c in TABLES[k]]
        parts += ['']
    parts += ['<!-- wiring:end -->','']
    return '\n'.join(parts)

atlas='''# Wiring atlas: read the labels, trace the wire

These diagrams document the proposed solder-free corded V1. They have not been physically tested. They are custom Tinkercad-style educational illustrations, **not an Autodesk Tinkercad circuit, screenshot or simulation**. They identify the specific purchased models but do not claim photorealistic or verified clone geometry.

## How to use a diagram

1. Find the exact module model and printed pin labels on your part.
2. Read the table and trace one labelled wire at a time. Colors are suggestions, not proof of function.
3. The illustrated breadboard rows mean isolated electrical junctions, not hole-by-hole placement. Check your actual board's contact groups with continuity while unpowered.
4. For integration, the OLED, IMU and PCA share the same four logic nets. Join like-labelled endpoints; never join different nets. Add and test one module at a time.
5. Check against the wiring specification and first-power-on sequence before applying power.

## XIAO ESP32C3 functional pinout

This table is a signal-name lookup, not a physical top/bottom view. USB orientation and clone header geometry must be checked against the manufacturer diagram and actual silkscreen.

| Board label | ESP32 GPIO / role | V1 use |
|---|---|---|
| D0 | GPIO2 | unused |
| D1 | GPIO3 | unused |
| D2 | GPIO4 | unused |
| D3 | GPIO5 | unused |
| D4 | GPIO6 / SDA | I2C data |
| D5 | GPIO7 / SCL | I2C clock |
| D6 | GPIO21 / TX | unused |
| D7 | GPIO20 / RX | unused |
| D8 | GPIO8 | unused |
| D9 | GPIO9 | unused |
| D10 | GPIO10 | unused |
| 3V3 | regulated logic supply | module VCC |
| GND | ground | shared reference |
| 5V | USB-related supply rail | **leave unconnected** |
| USB-C | USB power and data | computer data cable |

Battery pads and the JLJLUP JST1.25 battery are intentionally outside this V1 connection plan. There is no approved battery hookup here.

## Sources and verification limits

- [Seeed XIAO ESP32C3 pin definitions](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/).
- [Adafruit PCA9685 power and signal roles](https://learn.adafruit.com/16-channel-pwm-servo-driver/pinouts): reference circuit, not proof of Teyleten clone layout.
- AITRIP OLED header order comes from the owner's product photo; verify the received part matches.
- GY-521 and Teyleten physical header positions, terminal orientation and breadboard contact groups still require inspection of the delivered hardware. No hole-coordinate placement diagram is approved yet.

## Rebuild

From the repository root, install `requirements-book.txt`, then run `python scripts/build_wiring.py`. This regenerates PNGs, connection tables and marked documentation sections from this script. Run `python docs/book/build_book.py` afterwards to update HTML and EPUB. Do not hand-edit marked sections.

'''
(OUT/'README.md').write_text(atlas+snippet(['usb','oled','imu','pca','servo','meter'],OUT),encoding='utf-8')

# Explicit coverage: comprehensive guides plus small repeated instructions.
coverage={
 'README.md':['usb','oled','imu','pca','servo'],
 'docs/WIRING.md':['oled','imu','pca','servo','meter'],
 'docs/BUILD.md':['usb','oled','imu','pca','servo','meter'],
 'docs/FIRST-POWER-ON.md':['usb','oled','imu','pca','servo','meter'],
 'lessons/README.md':['usb','oled','imu','pca','servo','meter'],
 'docs/COMMANDS.md':['usb','servo','pca'],
 'docs/TROUBLESHOOTING.md':['usb','oled','imu','pca','servo'],
 'docs/START-HERE.md':['usb'],
 'docs/SHOPPING-LIST.md':['servo','pca','meter'],
}
placements={
 'docs/WIRING.md':{'Power architecture':['servo'],'Logic connections':['oled','imu','pca'],'Meter setup for the first power check':['meter']},
 'docs/BUILD.md':{'2. Prepare the power connection':['servo','meter'],'3. Establish USB programming':['usb'],'4. Bring up low-current modules':['oled','imu','pca'],'5. Calibrate a single servo':['servo']},
 'docs/FIRST-POWER-ON.md':{'Upload':['pca'],'Face and motion-sensor checks':['oled','imu','pca'],'One servo, no leg attached':['servo','meter']},
 'lessons/README.md':{'0. Identify before overwriting':['usb'],'1. Hello: a program lives on a board':['usb'],'2. Scanner: devices have addresses':['oled'],'3. Face: pixels make expressions':['oled'],'4. Motion: gravity is a clue':['imu'],'5A. Test the driver by itself':['pca'],'5C. Combine the three verified modules':['oled','imu','pca'],'5B. Introduce one servo and its power supply':['servo','meter']},
}
for relative,keys in coverage.items():
    p=ROOT/relative
    if not p.exists(): continue
    text=p.read_text(encoding='utf-8-sig')
    text=re.sub(r'\n?<!-- wiring:start -->.*?<!-- wiring:end -->\n?', '',text,flags=re.S)
    # A prominently placed pack ensures the visual/table accompanies instructions.
    first,sep,rest=text.partition('\n')
    link=Path(os.path.relpath(OUT/'README.md',p.parent)).as_posix()
    intro=f'Wiring reference: [full pinout atlas and verification limits]({link}). Use the stage matching your lesson; do not connect every pictured module at once.\n'
    text=first+'\n\n'+intro+'\n'+rest.lstrip()
    if relative in placements:
        for heading,subset in placements[relative].items():
            target='## '+heading+'\n'
            assert target in text,(relative,heading)
            intro = ''
            if relative == 'lessons/README.md' and heading.startswith('5A.'):
                intro = '**Starting state:** lesson 4 left XIAO + GY-521 running Motion. **New part:** the PCA9685 driver generates timed control signals. First test its logic communication alone, using the same four XIAO connections you already learned.'
            elif (relative == 'lessons/README.md' and heading.startswith('5B.')) or (relative == 'docs/FIRST-POWER-ON.md' and heading == 'One servo, no leg attached'):
                intro = '**Starting state:** only the XIAO and independently tested PCA9685 logic are connected, with Companion running. The tested OLED and GY-521 are set aside. **New parts:** one SG90 servo motor, a regulated 5V wall power supply, its low-voltage barrel-to-screw-terminal connector, prepared power wires, and the multimeter. The servo turns its shaft in response to the driver signal. The separate supply provides motor power; USB continues powering the XIAO. Identify these parts first. The wall supply has not been connected yet.'
            text=text.replace(target,target+'\n'+snippet(subset,p.parent,introduction=intro)+'\n',1)
    else:
        text=first+'\n\n'+intro+snippet(keys,p.parent)+'\n'+rest.lstrip()
    # Avoid accumulating the index sentence on regeneration.
    marker='Wiring reference: [full pinout atlas and verification limits]'
    seen=False; lines=[]
    for line in text.splitlines():
        if line.startswith(marker):
            if seen: continue
            seen=True
        lines.append(line)
    p.write_text(re.sub(r'\n{3,}', '\n\n', '\n'.join(lines))+'\n',encoding='utf-8')

# Keep the EPUB self-contained: copy PNGs and embed tables inside relevant chapters.
book=ROOT/'docs/book'
for k in TABLES: shutil.copyfile(IMAGES/(k+'.png'),book/'images'/('wiring-'+k+'.png'))
p=book/'manuscript.md'
text=re.sub(r'\n?<!-- wiring:start -->.*?<!-- wiring:end -->\n?', '',p.read_text(encoding='utf-8'),flags=re.S)
chapter_keys={4:['usb'],5:['oled'],6:['imu'],7:['servo','pca'],8:['pca','servo'],10:['oled','imu','pca','servo'],11:['meter'],12:['usb'],14:['usb','oled','imu','pca','servo'],15:['servo','pca'],17:['usb','oled','imu','pca','servo']}
for number,keys in chapter_keys.items():
    text=re.sub(r'(^## '+str(number)+r'\. [^\n]+\n)',lambda m:m[1]+'\n'+snippet(keys,book,True)+'\n',text,count=1,flags=re.M)
p.write_text(re.sub(r'\n{3,}', '\n\n', text),encoding='utf-8')
print(f'Updated {len(coverage)} guides, {len(chapter_keys)} book chapters, atlas and {len(TABLES)} illustrations.')

