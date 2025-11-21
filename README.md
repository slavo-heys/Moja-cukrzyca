Projekt Cukrzyca_2 - proste narzędzie CLI

Szybkie instrukcje:

Budowanie:

```bash
cd /home/black/CPP_Project/Cukrzyca_2
make
```

Uruchomienie:

```bash
./start
```

Cele:
- `make` kompiluje program `start`
- `make run` uruchamia program
- `make clean` usuwa binaria i pliki raportu

Eksport raportu:
- W menu głównym wybierz opcję `10` (Wydrukuj raport w pdf).
- Program wygeneruje `report.txt` zawierający dane z ostatnich 30 dni z plików `dane.csv`, `waga.csv`, `uwagi.csv`.
- Jeśli masz zainstalowany `pandoc`, program spróbuje wygenerować `report.pdf` automatycznie.
- Alternatywnie, zainstaluj `enscript` i `ps2pdf` (część pakietu `ghostscript`), wtedy program spróbuje użyć `enscript | ps2pdf`.

Filtr historii:
- Raport uwzględnia tylko wpisy z ostatnich 30 dni (format daty w plikach: `YYYY-MM-DD`).

Uwagi:
- Narzędzie stara się być "best-effort"; nie wszystkie terminale respektują ustawianie rozmiaru okna.
- Jeśli chcesz dodać dodatkowe formaty eksportu (np. CSV->Excel), powiedz, a dodam obsługę.
