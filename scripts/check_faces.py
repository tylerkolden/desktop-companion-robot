"""Check exported firmware frames against pixel drawings and GIF timing."""
from pathlib import Path
import re
from PIL import Image
from build_faces import face, NAMES, FRAME_COUNT, FRAME_MS

root=Path(__file__).resolve().parents[1]
header=(root/'lessons/lib/RobotLessons/src/FaceArt.h').read_text()
sequence_text=header.split('FACE_SEQUENCE[FACE_COUNT][FACE_FRAMES] PROGMEM = {')[1].split('};')[0]
sequences=[[int(n) for n in re.findall(r'\d+', row)]
           for row in re.findall(r'\{([^}]+)\}', sequence_text)]
bitmap_text=header.split('const uint8_t FACE_BITMAPS')[1]
data=bytes(int(n,16) for n in re.findall(r'0x([0-9a-f]{2})',bitmap_text))
assert len(sequences)==len(NAMES)==7
assert len(data)%1024==0
for name,sequence in zip(NAMES,sequences):
    assert len(sequence)==FRAME_COUNT
    assert len(set(sequence))>1, name
    for frame,index in enumerate(sequence):
        expected=face(name,frame)
        assert expected.size==(128,64) and expected.mode=='1'
        assert data[index*1024:(index+1)*1024]==expected.tobytes(), (name,frame)
assert 'LOVE' not in header and 'COOL' in header
with Image.open(root/'docs/faces/face-animations.gif') as gif:
    assert gif.info['loop']==0
    total=0
    for n in range(gif.n_frames):
        gif.seek(n); total+=gif.info['duration']
    assert total==FRAME_COUNT*FRAME_MS==4000
print('PASS all 280 firmware frames match their pixel source; seven distinct animations; GIF loop is 4000ms.')
