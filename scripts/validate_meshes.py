"""Dependency-free checks for the ASCII STL files exported by OpenSCAD.

Checks closed edges, consistent winding, one connected shell, positive volume
and expected overall dimensions. This does not establish physical fit.
"""
from collections import defaultdict
from pathlib import Path
import math
import re

ROOT = Path(__file__).resolve().parents[1]
EXPECTED = {"dog-body-v3.stl": (130.2, 48.0, 35.1), "dog-shell-v3.stl": (150.0, 64.0, 61.9591), "dog-tail-v1.stl": (32.0, 76.0, 8.5), "dog-body-v2.stl": (120.0, 48.0, 30.0), "dog-deck-v3.stl": (124.0, 59.0, 12.0), "dog-shell-v2.stl": (162.1934, 64.0, 68.8954), "dog-head-v2-shell.stl": (51.7111, 55.9308, 52.4808), "dog-head-v2-chin.stl": (45.0446, 50.8264, 37.478), "dog-ear-v2.stl": (27.0, 34.5, 3.0), "leg-v8.stl": (32.0, 71.0, 8.5), "dog-deck-v1.stl": (149, 48, 34.3), "dog-deck-v2.stl": (122.0, 48.0, 43.0), "dog-shell-v1.stl": (165.1289, 119.871, 73.8954), "dog-head-v1-shell.stl": (82.5218, 81.9056, 83.953), "dog-head-v1-chin.stl": (75.1567, 76.9134, 42.3), "dog-ear-v1.stl": (22.0, 37.7, 3.0), "leg-v7.stl": (18.0, 71.0, 8.5), "dog-body-v1.stl": (120, 48, 30), "leg-stand-v2.stl": (80, 60, 58), "leg-stand-v2-wedge.stl": (4.5, 14, 3.8), "leg-v6.stl": (14, 70, 8.5), "leg-v5.stl": (14, 70, 5.8), "leg-v4.stl": (14, 70, 5.8), "leg-v3.stl": (13, 70, 5.3), "leg-v2.stl": (13, 70, 5), "horn-gauge.stl": (85, 36, 3), "leg-v1.stl": (14, 70, 8), "leg-stand-v1.stl": (80, 60, 58), "servo-cradle-v4.stl": (44, 30, 4), "servo-cradle-v3.stl": (42, 28, 7.0), "servo-cradle-v2.stl": (42, 28, 7.2), "servo-cradle-v1.stl": (42, 24.9, 10), "servo-fit-gauge.stl": (94, 28, 2.4), "oled-fit-gauge.stl": (32, 32.3, 2.4)}

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
