"""Build the original illustrated EPUB and browser edition. Requires Pillow and Markdown."""
from pathlib import Path
import re, html, zipfile, uuid
import xml.etree.ElementTree as ET
from PIL import Image, ImageDraw, ImageFont
import markdown

ROOT = Path(__file__).resolve().parent
IMAGES = ROOT / 'images'
IMAGES.mkdir(exist_ok=True)
TITLE = 'Andrew and the Tiny Robot with Big Ideas'
STEM = 'Andrew-and-the-Tiny-Robot-with-Big-Ideas'
FONT = Path('C:/Windows/Fonts')
def font(size, bold=False):
    return ImageFont.truetype(str(FONT / ('arialbd.ttf' if bold else 'arial.ttf')), size)
def canvas(w=1200, h=850):
    im = Image.new('RGB', (w,h), 'white')
    return im, ImageDraw.Draw(im)
def text(d, xy, s, size=30, bold=False, fill='#172533'):
    d.text(xy,s,font=font(size,bold),fill=fill)
def box(d, rect, title, subtitle='', fill='#edf2f4'):
    d.rounded_rectangle(rect, radius=16, fill=fill, outline='#263746', width=4)
    x,y,x2,y2=rect
    text(d,(x+20,y+18),title,30,True)
    if subtitle: text(d,(x+20,y+63),subtitle,24)
def arrow(d, points):
    d.line(points,fill='#263746',width=5)
    x,y=points[-1]; px,py=points[-2]
    if x>px: tri=[(x,y),(x-15,y-9),(x-15,y+9)]
    elif x<px: tri=[(x,y),(x+15,y-9),(x+15,y+9)]
    elif y>py: tri=[(x,y),(x-9,y-15),(x+9,y-15)]
    else: tri=[(x,y),(x-9,y+15),(x+9,y+15)]
    d.polygon(tri,fill='#263746')

# Typographic cover with original geometric robot emblem (concept, not final CAD).
im,d=canvas(1600,2400)
d.rectangle((0,0,1600,2400),fill='#132634')
d.rectangle((90,105,1510,2295),outline='#7eddd3',width=4)
text(d,(155,180),'THE ANDREW ROBOT ADVENTURE',45,True,'#7eddd3')
for y,s,size in [(330,'ANDREW',145),(520,'AND THE TINY ROBOT',82),(645,'WITH BIG IDEAS',100)]:
    text(d,(150,y),s,size,True,'white')
text(d,(155,850),'Circuits. Code. Curiosity.',56,False,'#7eddd3')
d.rounded_rectangle((390,1120,1210,1660),80,fill='#eaf4f4')
d.rounded_rectangle((455,1190,1145,1555),45,fill='#132634')
for x in [570,905]: d.rounded_rectangle((x,1280,x+125,1410),35,fill='#7eddd3')
for a,b in [((430,1600),(270,1810)),((600,1660),(500,1850)),((1000,1660),(1100,1850)),((1170,1600),(1330,1810))]:
    d.line((a,b),fill='#7eddd3',width=28)
text(d,(155,1990),'A family guide to building',56,False,'white')
text(d,(155,2070),'a desktop companion',56,False,'white')
im.save(IMAGES/'cover.jpg',quality=92)

im,d=canvas(1200,890)
text(d,(45,30),'MEET ANDREW\'S ROBOT TEAM',42,True)
box(d,(335,125,865,240),'XIAO ESP32C3','Runs the program')
for x,t,s in [(35,'OLED','Shows the face'),(425,'MPU6050','Measures motion'),(815,'PCA9685','Times control pulses')]:
    box(d,(x,350,x+350,480),t,s)
    arrow(d,[(600,240),(600,290),(x+175,290),(x+175,350)])
box(d,(780,610,1170,745),'Four SG90 servos','Turn shafts to move parts')
arrow(d,[(990,480),(990,610)])
text(d,(45,615),'Also on the team:',30,True)
text(d,(45,665),'Power, wires, printed parts,',28)
text(d,(45,710),'tools, and your observations.',28)
text(d,(45,810),'Role map only. Wiring and power are separate questions.',26)
im.save(IMAGES/'team.png')

im,d=canvas(1200,820)
text(d,(45,25),'A FACE IS A MAP OF PIXELS',40,True)
arrow(d,[(190,140),(1040,140)]); text(d,(540,85),'x increases',28)
arrow(d,[(120,200),(120,650)]); text(d,(35,680),'y increases downward',27)
ox,oy,cell=200,210,49
for row in range(8):
    for col in range(16):
        on=(col in [3,4,10,11] and row in [2,3,4])
        d.rectangle((ox+col*cell,oy+row*cell,ox+(col+1)*cell,oy+(row+1)*cell),fill='#172533' if on else 'white',outline='#aab5bb',width=2)
text(d,(200,630),'(0, 0) is the top-left pixel.',28)
text(d,(200,730),'Practice grid: 16 x 8. Real display: 128 x 64.',28)
im.save(IMAGES/'pixels.png')

im,d=canvas(1200,720)
text(d,(45,35),'THE MESSAGE IS IN THE PULSE WIDTH',38,True)
text(d,(45,105),'Example: 50 cycles per second (50 Hz)',29)
# 20 ms interval spans 900 px; 1.5 ms is 67.5 px.
d.line([(135,395),(135,260),(203,260),(203,395),(1035,395),(1035,260)],fill='#172533',width=7)
text(d,(60,200),'HIGH',25,True); text(d,(60,425),'LOW',25,True)
d.line([(135,475),(1035,475)],fill='#172533',width=3)
for x in [135,1035]: d.line((x,460,x,490),fill='#172533',width=3)
text(d,(340,505),'One cycle = 20 milliseconds',31,True)
arrow(d,[(360,210),(230,210),(169,260)])
text(d,(365,185),'High for about 1.5 ms',28)
text(d,(45,610),'Nominal centre request; actual position needs calibration.',27)
im.save(IMAGES/'pulse.png')

im,d=canvas(1200,1280)
text(d,(40,25),'TWO SUPPLIES, SHARED GROUND',40,True)
text(d,(40,85),'Connection relationships - not physical pin positions',26)
box(d,(60,155,485,280),'Computer USB','Controller power + data')
box(d,(660,155,1140,280),'Regulated 5V adapter','Servo power only')
box(d,(60,365,485,480),'XIAO ESP32C3','Leave its 5V pin unconnected')
arrow(d,[(270,280),(270,365)])
box(d,(660,365,1140,500),'PCA9685 V+ / GND','Adapter + to V+; - to GND')
arrow(d,[(900,280),(900,365)])
arrow(d,[(900,500),(900,555)])
box(d,(660,555,1140,665),'Servo headers','Power + ground + pulses')
box(d,(60,760,1140,915),'Shared LOGIC connections','XIAO 3V3 / GND / GPIO6 (SDA) / GPIO7 (SCL)')
arrow(d,[(270,480),(270,760)])
text(d,(90,865),'to OLED, GY-521 and PCA9685 logic pins',28,True)
text(d,(60,975),'PCA VCC = 3.3V logic. PCA V+ = 5V servo rail.',29,True)
text(d,(60,1030),'PCA ground joins adapter negative and XIAO ground.',28)
text(d,(60,1100),'Do not join adapter +5V to the XIAO 5V pin.',29,True)
text(d,(60,1160),'Unplug both supplies before changing connections.',29,True)
im.save(IMAGES/'wiring.png')

im,d=canvas(1200,740)
text(d,(45,30),'MAKE THE NEXT QUESTION SMALLER',40,True)
for i,(t,s) in enumerate([('1. Measure','Observe the real part'),('2. Model','Change one dimension'),('3. Print','Use a small fit gauge')]):
    x=35+i*390; box(d,(x,155,x+350,295),t,s)
    if i<2: arrow(d,[(x+350,225),(x+387,225)])
box(d,(425,435,775,575),'4. Test','Record fit without force')
arrow(d,[(990,295),(990,505),(775,505)])
arrow(d,[(425,505),(210,505),(210,295)])
text(d,(40,640),'Revise using evidence. A mesh check is not a physical fit test.',28)
im.save(IMAGES/'iterate.png')

CSS='''body { font-family: Georgia, serif; line-height: 1.5; margin: 5%; color: #18242c; background: white; }
h1,h2,h3 { font-family: Arial, sans-serif; line-height: 1.2; text-align: left; }
h1 { font-size: 1.75em; } h2 { font-size: 1.45em; } h3 { font-size: 1.15em; margin-top: 1.5em; }
p { margin: 0.7em 0; } img { display:block; max-width:100%; height:auto; margin:1em auto; }
pre { white-space:pre-wrap; overflow-wrap:break-word; padding:0.7em; border:1px solid #88959d; font-size:0.85em; }
code { font-family:monospace; } a { color:#175a67; } li { margin:0.4em 0; }
.cover img { max-height:90vh; width:auto; } .subtitle { font-style:italic; }
@media screen { body { max-width:46em; margin:3em auto; padding:0 1em; } }
'''
raw=(ROOT/'manuscript.md').read_text(encoding='utf-8')
parts=re.split(r'^## ',raw,flags=re.M)[1:]
chapters=[]
for i,part in enumerate(parts):
    title,_,body=part.partition('\n')
    content=markdown.markdown('## '+part,extensions=['fenced_code'],output_format='xhtml')
    # Promote each chapter heading, retaining section headings inside it.
    content=content.replace('<h2>','<h1>',1).replace('</h2>','</h1>',1)
    chapters.append((f'chapter-{i:02}.xhtml',title,content))
def page(title,body):
    return '<?xml version="1.0" encoding="utf-8"?>\n<!DOCTYPE html>\n'+f'<html xmlns="http://www.w3.org/1999/xhtml" xmlns:epub="http://www.idpf.org/2007/ops" lang="en" xml:lang="en"><head><title>{html.escape(title)}</title><link rel="stylesheet" type="text/css" href="style.css"/></head><body>{body}</body></html>'
navitems=''.join(f'<li><a href="{f}">{html.escape(t)}</a></li>' for f,t,_ in chapters)
nav=page('Contents',f'<nav epub:type="toc" id="toc"><h1>Contents</h1><ol>{navitems}</ol></nav>')
cover=page(TITLE,'<section epub:type="cover" class="cover"><img src="images/cover.jpg" alt="Andrew and the Tiny Robot with Big Ideas. A family adventure in circuits, code, and creating a desktop companion."/></section>')
uid='urn:uuid:'+str(uuid.uuid5(uuid.NAMESPACE_URL,'andrew-robot-book-2026-09-15-edition-1'))
manifest=['<item id="nav" href="nav.xhtml" media-type="application/xhtml+xml" properties="nav"/>','<item id="cover" href="cover.xhtml" media-type="application/xhtml+xml"/>','<item id="css" href="style.css" media-type="text/css"/>','<item id="ncx" href="toc.ncx" media-type="application/x-dtbncx+xml"/>']
for i,(f,t,c) in enumerate(chapters): manifest.append(f'<item id="c{i}" href="{f}" media-type="application/xhtml+xml"/>')
for i,p in enumerate(sorted(IMAGES.iterdir())):
    typ='image/jpeg' if p.suffix=='.jpg' else 'image/png'
    prop=' properties="cover-image"' if p.name=='cover.jpg' else ''
    manifest.append(f'<item id="img{i}" href="images/{p.name}" media-type="{typ}"{prop}/>')
spine='<itemref idref="cover"/><itemref idref="nav"/>'+''.join(f'<itemref idref="c{i}"/>' for i in range(len(chapters)))
opf=f'''<?xml version="1.0" encoding="utf-8"?><package xmlns="http://www.idpf.org/2007/opf" version="3.0" unique-identifier="uid" xml:lang="en"><metadata xmlns:dc="http://purl.org/dc/elements/1.1/"><dc:identifier id="uid">{uid}</dc:identifier><dc:title>{TITLE}</dc:title><dc:language>en</dc:language><dc:creator>Andrew's Family Robotics Workshop</dc:creator><dc:description>A personal beginner robotics book written for Andrew with OpenAI Codex assistance.</dc:description><meta property="dcterms:modified">2026-09-15T12:00:00Z</meta></metadata><manifest>{''.join(manifest)}</manifest><spine toc="ncx">{spine}</spine></package>'''
ncx=f'''<?xml version="1.0" encoding="utf-8"?><ncx xmlns="http://www.daisy.org/z3986/2005/ncx/" version="2005-1"><head><meta name="dtb:uid" content="{uid}"/></head><docTitle><text>{TITLE}</text></docTitle><navMap>'''+''.join(f'<navPoint id="n{i}" playOrder="{i+1}"><navLabel><text>{html.escape(t)}</text></navLabel><content src="{f}"/></navPoint>' for i,(f,t,c) in enumerate(chapters))+'</navMap></ncx>'
container='<?xml version="1.0"?><container version="1.0" xmlns="urn:oasis:names:tc:opendocument:xmlns:container"><rootfiles><rootfile full-path="OEBPS/content.opf" media-type="application/oebps-package+xml"/></rootfiles></container>'
epub=ROOT/(STEM+'.epub')
with zipfile.ZipFile(epub,'w') as z:
    z.writestr('mimetype','application/epub+zip',compress_type=zipfile.ZIP_STORED)
    for name,data in [('META-INF/container.xml',container),('OEBPS/content.opf',opf),('OEBPS/nav.xhtml',nav),('OEBPS/cover.xhtml',cover),('OEBPS/toc.ncx',ncx),('OEBPS/style.css',CSS)]:
        z.writestr(name,data,compress_type=zipfile.ZIP_DEFLATED)
    for f,t,c in chapters: z.writestr('OEBPS/'+f,page(t,c),compress_type=zipfile.ZIP_DEFLATED)
    for p in IMAGES.iterdir(): z.write(p,'OEBPS/images/'+p.name,compress_type=zipfile.ZIP_DEFLATED)
toc=''.join(f'<li><a href="#c{i}">{html.escape(t)}</a></li>' for i,(f,t,c) in enumerate(chapters))
allbody=''.join(f'<section id="c{i}">{c}</section>' for i,(f,t,c) in enumerate(chapters))
(ROOT/(STEM+'.html')).write_text(f'<!doctype html><html lang="en"><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1"><title>{TITLE}</title><style>{CSS} section{{margin:4em 0}}</style><body><img src="images/cover.jpg" alt="Book cover"><nav><h1>Contents</h1><ol>{toc}</ol></nav>{allbody}</body></html>',encoding='utf-8')

# Structural checks: XML, navigation, local references, image descriptions, spine.
from posixpath import normpath, dirname
with zipfile.ZipFile(epub) as z:
    names=set(z.namelist())
    assert z.namelist()[0]=='mimetype' and z.getinfo('mimetype').compress_type==0
    for name in names:
        if name.endswith(('.xhtml','.opf','.ncx','.xml')):
            root=ET.fromstring(z.read(name))
            for element in root.iter():
                if element.tag.endswith('}img'): assert element.get('alt')
                for key in ['href','src']:
                    ref=element.get(key,'')
                    if ref and not re.match(r'^[a-z]+:',ref) and not ref.startswith('#'):
                        target=normpath(dirname(name)+'/'+ref.split('#')[0])
                        assert target in names,(name,ref)
    root=ET.fromstring(z.read('OEBPS/content.opf'))
    ns={'o':'http://www.idpf.org/2007/opf'}
    ids={e.get('id') for e in root.findall('o:manifest/o:item',ns)}
    assert all(e.get('idref') in ids for e in root.findall('o:spine/o:itemref',ns))
words=len(re.findall(r"\b[\w’'-]+\b",raw))
report=f'''Book QA report\nWords (including references/code): {words}\nReading sections: {len(chapters)} (20 numbered chapters plus front matter and 5 appendices)\nEPUB bytes: {epub.stat().st_size}\nPASS: mimetype first and uncompressed\nPASS: all XML/XHTML parses\nPASS: all local links and image targets resolve\nPASS: all images have alternative text\nPASS: manifest and reading-order references resolve\nOriginal images: {len(list(IMAGES.iterdir()))}\nNot tested: Amazon conversion, Kindle device rendering, hardware experiments.\nThis structural checker is not the official EPUBCheck validator.\n'''
(ROOT/'VALIDATION.txt').write_text(report,encoding='utf-8')
print(report)
