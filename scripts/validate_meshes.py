"""Dependency-free checks for the ASCII STL files exported by OpenSCAD.

Checks closed edges, consistent winding, one connected shell, positive volume
and expected overall dimensions. This does not establish physical fit.
"""
from collections import defaultdict
from pathlib import Path
import math
import re

ROOT = Path(__file__).resolve().parents[1]
EXPECTED = {"servo-cradle-v1.stl": (42, 24.9, 10), "servo-fit-gauge.stl": (94, 28, 2.4), "oled-fit-gauge.stl": (32, 32.3, 2.4)}

def validate(path, expected):
    vertices = [tuple(map(float, v.split())) for v in
                re.findall(r"\bvertex\s+([^\n]+)", path.read_text())]
    assert vertices and len(vertices) % 3 == 0, f"{path}: invalid triangle data"
    assert all(math.isfinite(x) for v in vertices for x in v)
    triangles = [vertices[i:i + 3] for i in range(0, len(vertices), 3)]
    edges = defaultdict(list)
    signed_volume = 0.0
    for index, (a, b, c) in enumerate(triangles):
        assert len({a, b, c}) == 3, "degenerate triangle"
        cross = (b[1]*c[2]-b[2]*c[1], b[2]*c[0]-b[0]*c[2], b[0]*c[1]-b[1]*c[0])
        signed_volume += sum(a[i]*cross[i] for i in range(3)) / 6
        for p, q in ((a,b), (b,c), (c,a)):
            edges[tuple(sorted((p,q)))].append((index, p < q))
    adjacency = defaultdict(set)
    for incident in edges.values():
        assert len(incident) == 2, "open or non-manifold edge"
        (a, direction_a), (b, direction_b) = incident
        assert direction_a != direction_b, "inconsistent winding"
        adjacency[a].add(b); adjacency[b].add(a)
    seen, pending = set(), [0]
    while pending:
        index = pending.pop()
        if index not in seen:
            seen.add(index); pending.extend(adjacency[index] - seen)
    assert len(seen) == len(triangles), "disconnected shell"
    assert signed_volume > 0, "nonpositive volume"
    bounds = tuple(max(v[i] for v in vertices)-min(v[i] for v in vertices) for i in range(3))
    assert all(abs(a-b) < 0.01 for a,b in zip(bounds,expected)), (bounds, expected)
    print(f"PASS {path.name}: {len(triangles)} triangles, bounds {bounds}, volume {signed_volume:.1f} mm^3")

if __name__ == "__main__":
    for name, bounds in EXPECTED.items():
        validate(ROOT / "cad" / "stl" / name, bounds)
