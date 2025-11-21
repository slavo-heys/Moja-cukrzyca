#!/usr/bin/env python3
"""
Export CSV data files to a single XLSX workbook with separate sheets.
Reads: dane.csv, waga.csv, uwagi.csv (semicolon-separated)
Writes: report.xlsx
"""
import pandas as pd
from pathlib import Path

files = {
    'Pomiar glukozy': 'dane.csv',
    'Pomiar wagi': 'waga.csv',
    'Uwagi': 'uwagi.csv',
}

out = Path('report.xlsx')
with pd.ExcelWriter(out, engine='openpyxl') as writer:
    for sheet, fname in files.items():
        f = Path(fname)
        if not f.exists():
            # create empty sheet with a message
            df = pd.DataFrame([['(brak pliku)']], columns=['Info'])
            df.to_excel(writer, sheet_name=sheet, index=False)
            continue
        try:
            df = pd.read_csv(f, sep=';', dtype=str)
        except Exception as e:
            df = pd.DataFrame([['(blad odczytu): ' + str(e)]], columns=['Info'])
            df.to_excel(writer, sheet_name=sheet, index=False)
            continue
        # drop any fully-empty trailing column created by trailing semicolons
        df = df.loc[:, ~df.columns.str.match('^Unnamed')]
        df.to_excel(writer, sheet_name=sheet, index=False)

print('Wygenerowano report.xlsx')
