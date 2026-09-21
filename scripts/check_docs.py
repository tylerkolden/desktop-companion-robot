"""Standard-library check of local Markdown link destinations (not web URLs)."""
from pathlib import Path
import os
import re
from urllib.parse import unquote

root = Path(__file__).resolve().parents[1]
ignored = {'.git', '.tools', '.venv', 'build', 'notes', '__pycache__'}
failures = []
count = 0
for folder, dirs, files in os.walk(root):
    dirs[:] = [d for d in dirs if d not in ignored]
    for filename in files:
        if not filename.endswith('.md'):
            continue
        source = Path(folder) / filename
        text = source.read_text(encoding='utf-8-sig')
        for destination in re.findall(r'\]\(([^)]+)\)', text):
            destination = destination.strip('<>').split('#')[0]
            if not destination or re.match(r'^[a-zA-Z]+:', destination):
                continue
            count += 1
            if not (source.parent / unquote(destination)).exists():
                failures.append(f'{source.relative_to(root)}: {destination}')
if failures:
    raise SystemExit('\n'.join(failures))
print(f'PASS {count} local Markdown destinations. Fragment anchors/web URLs not checked.')
