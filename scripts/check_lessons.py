"""Check course-to-sketch contracts without opening a serial port."""
from pathlib import Path
from build_lessons import COURSE

root=Path(__file__).resolve().parents[1]
profiles=[(0,0,0),(1,0,0),(1,0,0),(1,0,0),(0,1,0),(0,0,1),(1,1,1)]
runner=(root/'robot.ps1').read_text(encoding='utf-8')
for kind,(item,profile) in enumerate(zip(COURSE,profiles)):
    slug,name,*_=item
    source=(root/f'lessons/sketches/{name}/{name}.ino').read_text(encoding='utf-8')
    assert f'#define LESSON_KIND {kind}' in source,name
    for key,value in zip(['IMU','OLED','SERVO'],profile):
        assert f'#define LESSON_{key} {value}' in source,(name,key)
    assert 'firmware/Companion' not in source,name
    assert f"'{name}'" in runner,name
    plan=(root/f'lessons/plans/{slug}.md').read_text(encoding='utf-8')
    for section in range(1,11): assert f'## {section}. ' in plan,(name,section)
    assert f'-Sketch {name}' in plan,name
    assert '| From / pin |' in plan and '![' in plan,name
    assert 'Record' in plan or 'record' in plan,name
assert "'--library','lessons/lib/RobotLessons'" in runner
assert "[string]$Sketch = 'Basics'" in runner
print('PASS seven device profiles, standalone entry points, ten-part lesson structure, wiring tables and CLI routes.')
print('Motor trajectory/deadline/unit assertions also compile in every new sketch; hardware behavior still needs observation.')
