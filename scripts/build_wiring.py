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
    im,d=canvas(name+' - four-wire lesson', 'Unplug USB before wiring. Battery, servo supply and servos remain disconnected.')
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
          'pca':'VCC is 3.3V logic. V+ is the separate servo rail. OE: verify pull-down; ground only if floating.'}[key]
    d.text((65,833),note,font=font(22),fill='#102b40')
    d.text((65,880),'XIAO 5V stays unused. Locate every terminal by its printed label, never by this drawing position.',font=font(24),fill='#a12832')
    save(im,key)

im,d=canvas('USB-only: identify / Hello / upload', 'All external header pins are unused during the bare-board lesson.')
board(d,(70,240,535,650),'Computer USB','Data-capable cable')
board(d,(1060,240,1530,650),'XIAO ESP32C3','USB-C socket; leave headers empty')
d.line([(535,435),(1060,435)],fill='#346aab',width=20)
d.text((590,370),'USB power + data',font=font(29),fill='#102b40')
d.text((80,735),'No battery. No OLED, IMU or PCA9685. No servo supply. Do not hand-wire USB contacts.',font=font(28),fill='#102b40')
d.text((80,805),'Connecting USB can reset the board. A detected COM port does not prove which program is installed.',font=font(25),fill='#102b40')
save(im,'usb')

im,d=canvas('Servo power: one unloaded SG90 on channel 0', 'Wire with BOTH supplies unplugged. Energize only at the adult-supervised first-power-on checkpoint.')
board(d,(40,180,500,455),'Regulated 5V adapter','Low-voltage barrel terminal adapter')
board(d,(1040,180,1560,625),'Teyleten PCA9685','V+ and VCC are different supplies')
for y,label,color in [(330,'+5V  to  V+',COLORS[0]),(400,'negative  to  GND',COLORS[1])]:
    d.line([(500,y),(1040,y)],fill=color,width=9)
    d.text((545,y-37),label,font=font(26),fill=color)
board(d,(40,535,500,820),'XIAO on USB','Logic connections from PCA diagram')
d.line([(500,660),(740,660),(740,535),(1040,535)],fill=COLORS[1],width=6)
d.text((560,680),'GND shared',font=font(25),fill=COLORS[1])
board(d,(1040,720,1560,890),'One SG90 servo','Unloaded shaft; no horn or leg')
for x,label,col in [(1100,'GND',COLORS[1]),(1270,'V+',COLORS[0]),(1440,'PWM','#a05b00')]:
    d.line([(x,625),(x,720)],fill=col,width=7)
    d.text((x+12,655),label,font=font(23),fill=col)
d.text((560,195),'Direct terminal path',font=font(26),fill='#102b40')
d.text((550,245),'No breadboard servo power',font=font(23),fill='#a12832')
d.text((560,750),'Never join adapter +5V',font=font(23),fill='#a12832')
d.text((560,790),'to XIAO 5V or 3V3.',font=font(23),fill='#a12832')
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
 'servo':[('Adapter positive','PCA V+ screw terminal','Regulated 5V; direct power wire'),('Adapter negative','PCA GND screw terminal','Direct power wire'),('XIAO GND','PCA GND','Common reference'),('PCA channel 0 GND','SG90 ground','Usually brown/black; verify'),('PCA channel 0 V+','SG90 power','Usually red; verify'),('PCA channel 0 PWM','SG90 signal','Usually orange/yellow; verify')],
 'meter':[('Black probe / COM jack','Adapter negative output','Voltage check only'),('Red probe / V-ohm jack','Adapter positive output','DC volts; never current mode')]
}
NOTES={
 'usb':'The pinout for this stage is USB-C socket only: **all XIAO header pins are unused**. No external modules or battery.',
 'oled':'Functional pinout: VCC, GND, SDA, SCL. The supplied display photo has **GND / VCC / SCL / SDA** across its top header when viewed from the front. Our illustration groups wires by function, not that physical order.',
 'imu':'Functional pinout: VCC, GND, SCL, SDA, XDA, XCL, AD0, INT. Leave XDA/XCL/INT open; verify AD0 default through scanning. Exact delivered header order is unconfirmed. Check regulator/pull-ups at 3.3V; do not substitute 5V to fix detection.',
 'pca':'Functional pinout: VCC = logic 3.3V; V+ = separate servo power; GND = common ground; SDA/SCL = I2C; OE = active-low output enable. Exact clone header order is unconfirmed. Verify OE pull-down; ground OE only if floating. V+ and servo plugs remain disconnected in this logic-only stage.',
 'servo':'Functional servo pinout: ground, power, signal. Read the driver row labels before inserting the plug. **Also complete the PCA logic diagram** (3V3/VCC, GND, SDA, SCL). Start with only channel 0; later 0/1/2/3 map to front-left/front-right/rear-left/rear-right. USB and adapter grounds meet at PCA GND; adapter positive never meets XIAO 5V or 3V3. Grounded OE is not a reset interlock: unplug servo power for uploads, resets and horn fitting.',
 'meter':'Probe-jack pinout is functional, not the physical socket position of every meter variant. Select DC voltage before probing. Verify barrel size and polarity against the actual adapter; do not assume center-positive from a generic picture.'
}
def snippet(keys, parent, book=False):
    parts=['<!-- wiring:start -->','### Wiring visuals and pinouts','',
           '**Before changing wires: unplug USB and the servo adapter. Battery stays disconnected.** These custom Tinkercad-style illustrations are not a Tinkercad project or simulation. Board shapes are illustrative; terminal positions are functional, not verified physical placement. Read actual board labels.','']
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
 'docs/FIRST-POWER-ON.md':{'Upload':['usb'],'Face and motion-sensor checks':['oled','imu','pca'],'One servo, no leg attached':['servo','meter']},
 'lessons/README.md':{'0. Identify before overwriting':['usb'],'1. Hello: a program lives on a board':['usb'],'2. Scanner: devices have addresses':['oled','imu'],'3. Face: pixels make expressions':['oled'],'4. Motion: gravity is a clue':['imu'],'5. Integration and one unloaded servo':['pca','servo','meter']},
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
            text=text.replace(target,target+'\n'+snippet(subset,p.parent)+'\n',1)
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
