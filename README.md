Projekt Cukrzyca_2 - proste narzędzie CLI

Szybkie instrukcje

1) Kompilacja (Linux / macOS)

```bash
cd /home/black/CPP_Project/Cukrzyca_2
make
```

- Alternatywnie (ręcznie z definicjami makr):

```bash
g++ -std=c++17 -O2 -Wall -Wextra -DWZROST=175 -DWIEK=56 start.cpp -o start
```

2) Kompilacja (Windows)

- MinGW (MSYS2): otwórz MSYS2 MinGW shell i uruchom polecenia analogiczne do Linux:

```bash
g++ -std=c++17 -O2 -Wall -Wextra start.cpp -o start.exe
```

- MSVC (Visual Studio Developer Command Prompt): użyj `cl` lub utwórz projekt Visual Studio; łatwiej jest użyć MinGW dla prostego CLI.

3) Uruchomienie programu

```bash
# w katalogu projektu
./start         # Linux/macOS
start.exe       # Windows (jeśli skompilowano jako exe)
```

4) Makefile – wygodne cele

- `make` — kompiluje program (`start`).
- `make run` — uruchamia `./start`.
- `make clean` — usuwa binaria i raporty.
- `make xlsx` — generuje `report.xlsx` z danych CSV (wymaga Pythona i zależności z `requirements.txt`).

5) Eksport i raporty

- W menu głównym wybierz `10` aby wygenerować raport tekstowy (`report.txt`) i markdown (`report.md`).
- Jeśli masz zainstalowany `pandoc`, program spróbuje wygenerować `report.pdf` automatycznie.
- Aby wygenerować arkusz Excel (`report.xlsx`) uruchom:

```bash
make xlsx
# lub
./.venv/bin/python export_xlsx.py
```

6) Zależności opcjonalne

- `pandoc` — do wygenerowania ładnego `report.pdf` z `report.md`.
	- Linux (Debian/Ubuntu/Kali): `sudo apt install pandoc`
	- macOS (Homebrew): `brew install pandoc`
	- Windows: pobierz ze strony pandoc lub użyj `chocolatey`.

- `enscript` + `ghostscript` (alternatywa): `enscript | ps2pdf`.

- Python (opcjonalnie dla `make xlsx`): projekt tworzy lokalne venv i używa `pandas` + `openpyxl`.
	- Utworzenie venv i instalacja:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

7) Pliki danych

- `dane.csv` — pomiary glukozy (kolumny: `DATA;PRZED_SNIADANIEM;PO_SNIADANIU;PO_OBIEDZIE;PO_KOLACJI;`).
- `waga.csv` — pomiary wagi (kolumny: `DATA;GODZINA;WAGA;BMI;INFO;`).
- `uwagi.csv` — notatki (kolumny: `DATA;GODZINA;TRESC;`).

8) Interakcja programu

- Program działa w pętli — po wykonaniu akcji wraca do menu.
- Wpisz `0` w menu aby zakończyć program.
- Przy próbie nadpisania istniejącego pomiaru (glukozy/wagi) program zapyta o potwierdzenie `(T/N)`.

9) Kolory i terminal

- Program używa kodów ANSI do kolorowania komunikatów; kolorowanie działa w większości terminali unixowych (Linux/macOS). W Windows 10+ z PowerShell/Windows Terminal również powinno działać. Jeśli kolory przeszkadzają, można je usunąć modyfikując `start.cpp`.

10) Dalsze kroki i rozwój

- Możliwości rozszerzeń: lepsze formatowanie PDF (pandoc templates), eksport XLSX z formatowaniem, edycja istniejących wpisów, synchronizacja z repozytorium zdalnym.

Masz pytania lub chcesz, żebym dodał do README instrukcję konfiguracji środowiska Windows (MSVC) lub skrypt `make setup-env`? 
