#!/usr/bin/env python3
"""Dump the UPS 작화 xls memory-map sheet to UTF-8 TSV.

Usage:
  python scripts/dump_xls.py
  python scripts/dump_xls.py path/to/file.xls
"""
from __future__ import annotations

import os
import sys

try:
    import xlrd
except ImportError:
    sys.stderr.write("pip install xlrd\n")
    sys.exit(1)

SHEET_NAME = "사용중인 전체 메모리번지 정리(최종)"
DEFAULT_DOC = os.path.normpath(
    os.path.join(os.path.dirname(__file__), "..", "..", "..", "..", "doc")
)


def find_xls(doc_dir: str) -> str:
    if len(sys.argv) > 1:
        return sys.argv[1]
    for name in os.listdir(doc_dir):
        lower = name.lower()
        if lower.endswith(".xls") and not lower.endswith(".xlsx"):
            if "작화" in name or "UPS" in name or "ups" in name.lower():
                return os.path.join(doc_dir, name)
    raise FileNotFoundError("no 작화 .xls in " + doc_dir)


def cell_str(cell) -> str:
    if cell.ctype == xlrd.XL_CELL_EMPTY:
        return ""
    v = cell.value
    if cell.ctype == xlrd.XL_CELL_NUMBER:
        if float(v).is_integer():
            return str(int(v))
        return str(v)
    return str(v).replace("\t", " ").replace("\r", " ").replace("\n", " | ")


def main() -> int:
    doc_dir = DEFAULT_DOC
    xls = find_xls(doc_dir)
    wb = xlrd.open_workbook(xls, encoding_override="cp949")
    if SHEET_NAME not in wb.sheet_names():
        sys.stderr.write("sheets: " + ", ".join(wb.sheet_names()) + "\n")
        return 1
    sh = wb.sheet_by_name(SHEET_NAME)
    out_dir = os.path.join(os.path.dirname(__file__), "..", "dumps")
    os.makedirs(out_dir, exist_ok=True)
    out_path = os.path.join(out_dir, "memory_map.tsv")
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(f"# file={os.path.basename(xls)} sheet={SHEET_NAME} rows={sh.nrows}\n")
        for r in range(sh.nrows):
            row = [cell_str(sh.cell(r, c)) for c in range(sh.ncols)]
            while row and row[-1] == "":
                row.pop()
            if any(row):
                f.write(str(r) + "\t" + "\t".join(row) + "\n")
    print(out_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
