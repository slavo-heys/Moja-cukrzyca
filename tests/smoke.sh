#!/usr/bin/env zsh
set -euo pipefail
# Simple smoke test: builds project, runs a scripted interactive session,
# and lists important output files for quick inspection.
cd "$(dirname "$0")/.." || exit 1
echo "Building project..."
make
echo "Running CLI with scripted inputs..."
printf "4

5

6

1
88
T
2
Automated test note
3
75
T
10

0
" | ./start
echo "--- Generated files ---"
ls -l dane.csv uwagi.csv waga.csv report.md report.pdf report.xlsx 2>/dev/null || true
echo "Smoke test completed."