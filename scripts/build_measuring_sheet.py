"""Printable millimetre measuring sheet for photographing small parts.

Print at 100% ("Actual size"). Corner squares give exact reference points so a
straight-down photo can be scaled and perspective-corrected. Output: docs/measure/.
"""
from pathlib import Path
from reportlab.lib.pagesizes import letter
from reportlab.lib.units import mm
from reportlab.pdfgen import canvas

OUT = Path(__file__).resolve().parents[1] / "docs" / "measure" / "horn-measuring-sheet.pdf"

def grid(c, x0, y0, w, h, label):
    """w x h mm grid with origin at (x0,y0) mm from page bottom-left."""
    for i in range(w + 1):
        c.setLineWidth(0.6 if i % 10 == 0 else 0.35 if i % 5 == 0 else 0.12)
        c.setStrokeGray(0 if i % 10 == 0 else 0.35 if i % 5 == 0 else 0.6)
        c.line((x0 + i) * mm, y0 * mm, (x0 + i) * mm, (y0 + h) * mm)
    for j in range(h + 1):
        c.setLineWidth(0.6 if j % 10 == 0 else 0.35 if j % 5 == 0 else 0.12)
        c.setStrokeGray(0 if j % 10 == 0 else 0.35 if j % 5 == 0 else 0.6)
        c.line(x0 * mm, (y0 + j) * mm, (x0 + w) * mm, (y0 + j) * mm)
    c.setFillGray(0); c.setFont("Helvetica", 6)
    for i in range(0, w + 1, 10):
        c.drawCentredString((x0 + i) * mm, (y0 - 3.5) * mm, str(i))
    for j in range(0, h + 1, 10):
        c.drawRightString((x0 - 1.5) * mm, (y0 + j - 1) * mm, str(j))
    # Corner reference squares: 4 x 4 mm, outside the grid, centres exactly (w+10) x (h+10) mm apart.
    for cx, cy in [(x0 - 5, y0 - 5), (x0 + w + 5, y0 - 5), (x0 - 5, y0 + h + 5), (x0 + w + 5, y0 + h + 5)]:
        c.rect((cx - 2) * mm, (cy - 2) * mm, 4 * mm, 4 * mm, stroke=0, fill=1)
    c.setFont("Helvetica-Bold", 10)
    c.drawString(x0 * mm, (y0 + h + 9) * mm, label)

def main():
    c = canvas.Canvas(str(OUT), pagesize=letter)
    c.setTitle("Horn measuring sheet")
    c.setFont("Helvetica-Bold", 14)
    c.drawString(20 * mm, 265 * mm, "Robot dog: horn measuring sheet")
    c.setFont("Helvetica", 9)
    lines = [
        "1. Print at 100% / Actual size (NOT 'Fit to page'). Check the 100 mm bar below with a ruler.",
        "2. Box A: lay the horn flat, hub up, along the long lines. Box B: tape the horn on its edge.",
        "3. Phone straight above, about 25 cm away, no zoom, bright light, all 4 black squares in the photo.",
        "4. Take one photo of each box and send them. Squares sit 10 mm outside each grid corner.",
    ]
    for k, t in enumerate(lines):
        c.drawString(20 * mm, (258 - 5 * k) * mm, t)
    # 100 mm check bar
    c.setLineWidth(1.2); c.line(20 * mm, 232 * mm, 120 * mm, 232 * mm)
    for x in (20, 120):
        c.line(x * mm, 230 * mm, x * mm, 234 * mm)
    c.drawString(124 * mm, 231 * mm, "<- this bar must measure exactly 100 mm")
    grid(c, 30, 140, 120, 70, "A: horn lying flat (hub facing up)")
    grid(c, 30, 40, 120, 60, "B: horn standing on its edge (side view)")
    c.setFont("Helvetica", 7)
    c.drawString(20 * mm, 15 * mm, "Grid: 1 mm fine, 5 mm medium, 10 mm bold. Source: scripts/build_measuring_sheet.py")
    c.showPage(); c.save()
    print(f"wrote {OUT}")

if __name__ == "__main__":
    main()
