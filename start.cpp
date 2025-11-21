#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<ctime>
#include<iomanip>
#include<cmath>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#define SZEROKOSC_MENU 123
#define WZROST 180 // wzrost w cm, uzywany do obliczen BMI
#define WIEK 56 // wiek uzywany do obliczen BMI

using namespace std;

// Kolory ANSI (używane w większości terminali unixowych)
const string COL_RESET = "\033[0m";
const string COL_BOLD = "\033[1m";
const string COL_RED = "\033[31m";
const string COL_GREEN = "\033[32m";
const string COL_YELLOW = "\033[33m";
const string COL_BLUE = "\033[34m";
const string COL_MAGENTA = "\033[35m";
const string COL_CYAN = "\033[36m";

// Helper: print colored text (wraps and resets)
inline string c(const string &col, const string &s) { return col + s + COL_RESET; }

// Trim helper
inline string trim_str(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return string("");
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

// Read a full line from stdin (returns trimmed)
inline string read_line() {
    string s;
    if (!std::getline(cin, s)) return string("");
    // strip CR if present
    if (!s.empty() && s.back() == '\r') s.pop_back();
    return trim_str(s);
}

// Ustawia rozmiar konsoli (nie gwarantowane we wszystkich terminalach)
void set_console_size(int cols, int rows) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    // Ustaw rozmiar bufora i okna
    COORD bufferSize;
    bufferSize.X = (SHORT)cols;
    bufferSize.Y = (SHORT)rows;
    SetConsoleScreenBufferSize(hOut, bufferSize);
    SMALL_RECT win;
    win.Left = 0;
    win.Top = 0;
    win.Right = (SHORT)(cols - 1);
    win.Bottom = (SHORT)(rows - 1);
    SetConsoleWindowInfo(hOut, TRUE, &win);
#else
    struct winsize w;
    w.ws_col = cols;
    w.ws_row = rows;
    w.ws_xpixel = 0;
    w.ws_ypixel = 0;
    // ioctl ustawia rozmiar w kernelu; terminal może zignorować zmianę
    ioctl(STDOUT_FILENO, TIOCSWINSZ, &w);
    // dodatkowo wyślij sekwencję xterm do zmiany rozmiaru (może działać w xterm/gnome-terminal)
    cout << "\033[8;" << rows << ";" << cols << "t" << flush;
#endif
}

// Prosta funkcja szacująca ile wierszy potrzeba do wyświetlenia głównego menu
int estimate_menu_glowne_height() {
    // Liczba linii wypisywanych przez menu_glowne przed zapytaniem o wybór
    // Zliczone ręcznie: 18 (w zależności od układu), dodajemy mały margines
    const int base_lines = 18;
    const int margin = 4;
    return base_lines + margin;
}


// Deklaracja funkcji
void sprawdz_plik_dane();
void sprawdz_plik_uwagi();
void sprawdz_plik_waga();
void czyscc_ekran();
void menu_glowne();
void menu_dodaj_pomiar_glukozy();
void menu_dodaj_uwage();
void menu_dodaj_pomiar_wagi();
void menu_wyswietl_historie_glukozy();
void menu_wyswietl_historie_uwagi();
void menu_wyswietl_historie_wagi();



// Główna funkcja programu ######################################
int main(){
    sprawdz_plik_dane();
    sprawdz_plik_uwagi();
    sprawdz_plik_waga();    
    // Spróbuj ustawić rozmiar konsoli dynamicznie na podstawie zawartości menu
    int required_rows = estimate_menu_glowne_height();
    set_console_size(SZEROKOSC_MENU + 2, required_rows);
    menu_glowne();

    return 0;
}
//###############################################################

// Funkcja sprawdzająca istnienie pliku o nazwie "dane.csv",
//  jesli plik nie istnieje to tworzy go z kolumnami: "DATA;PRZED_SNIADANIEM;PO_SNIADANIU;PO_OBIEDZIE;PO_KOLACJI;"
void sprawdz_plik_dane(){
    string nazwa_pliku = "dane.csv";
    FILE *plik = fopen(nazwa_pliku.c_str(), "r");
    if (plik == NULL) {
        plik = fopen(nazwa_pliku.c_str(), "w");
        if (plik != NULL) {
            fprintf(plik, "DATA;PRZED_SNIADANIEM;PO_SNIADANIU;PO_OBIEDZIE;PO_KOLACJI;\n");
            fclose(plik);
        } else {
            cerr << "Nie mozna utworzyc pliku: " << nazwa_pliku << endl;
        }
    } else {
        fclose(plik);
    }
}

// Funkcja sprawdzająca istnienie pliku o nazwie "uwagi.csv",
//  jesli plik nie istnieje to tworzy go z kolumnami: "DATA;GODZINA;TRESC;"
void sprawdz_plik_uwagi(){
    string nazwa_pliku = "uwagi.csv";
    FILE *plik = fopen(nazwa_pliku.c_str(), "r");
    if (plik == NULL) {
        plik = fopen(nazwa_pliku.c_str(), "w");
        if (plik != NULL) {
            fprintf(plik, "DATA;GODZINA;TRESC;\n");
            fclose(plik);
        } else {
            cerr << "Nie mozna utworzyc pliku: " << nazwa_pliku << endl;
        }
    } else {
        fclose(plik);
    }
}

// Funkcja sprawdzająca istnienie pliku o nazwie "waga.csv",
//  jesli plik nie istnieje to tworzy go z kolumnami: "DATA;GODZINA;WAGA;"
void sprawdz_plik_waga(){
    string nazwa_pliku = "waga.csv";
    FILE *plik = fopen(nazwa_pliku.c_str(), "r");
    if (plik == NULL) {
        plik = fopen(nazwa_pliku.c_str(), "w");
            if (plik != NULL) {
            fprintf(plik, "DATA;GODZINA;WAGA;BMI;INFO;\n");
            fclose(plik);
        } else {
            cerr << "Nie mozna utworzyc pliku: " << nazwa_pliku << endl;
        }
    } else {
        fclose(plik);
    }
}

// Funkcja czyszcząca ekran konsoli niezaleznie od systemu operacyjnego
void czyscc_ekran(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Funkcja wyświetlająca menu główne programu (działa w pętli aż do wybrania 0)
void menu_glowne(){
    const int szerokosc = SZEROKOSC_MENU;
    while (true) {
        czyscc_ekran();
        string wybor;
        cout << endl;
        cout << "+" << string(szerokosc, '=') << "+" << endl;
        cout << "|" << string((szerokosc), ' ') << "|" << endl;
        cout << "|" << string((szerokosc - 10) / 2, ' ') << c(COL_CYAN + COL_BOLD, "MENU GLOWNE") << string((szerokosc - 10) / 2, ' ') << "|" << endl;
        cout << "|" << string((szerokosc), ' ') << "|" << endl;
        cout << "+" << string(szerokosc, '=') << "+" << endl;
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 2, "", (szerokosc/2) - 2, "");
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 2, "1. Dodaj pomiar glukozy", (szerokosc/2) - 2, "4. Wyswietl historie pomiarow glukozy");
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 2, "", (szerokosc/2) - 2, "");
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 2, "2. Dodaj uwage", (szerokosc/2) - 2, "5. Wyswietl historie uwag");
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 2, "", (szerokosc/2) - 2, "");
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 2, "3. Dodaj pomiar wagi", (szerokosc/2) - 2, "6. Wyswietl historie pomiarow wagi");    
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 2, "", (szerokosc/2) - 2, "");
        cout << "+" << string(szerokosc, '=') << "+" << endl;
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 2, "", (szerokosc/2) - 2, "");
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 1, "0. Zakończ program", (szerokosc/2) - 2, "10. Wydrukuj raport w pdf");
        printf("| %-*s | %-*s |\n", (szerokosc/2) - 2, "", (szerokosc/2) - 2, "");
        cout << "+" << string(szerokosc, '=') << "+" << endl;
        printf("Wybierz opcje: ? ");
        wybor = read_line();
        // trim whitespace
        auto trim_local = [](string s){ size_t a = s.find_first_not_of(" \t\r\n"); if (a==string::npos) return string(""); size_t b = s.find_last_not_of(" \t\r\n"); return s.substr(a, b-a+1); };
        wybor = trim_local(wybor);
        if (wybor.empty()) continue; // ignore empty input and redraw menu

        if(wybor=="0" || wybor=="q" || wybor=="Q"){
            czyscc_ekran();
            cout << c(COL_GREEN, "Koniec programu. Do zobaczenia!") << endl;
            break; // wyjdz z petli i zakoncz funkcje
        } 
        else if(wybor=="1"){
            menu_dodaj_pomiar_glukozy();
        }
        else if(wybor=="4"){
            menu_wyswietl_historie_glukozy();
        }
        else if(wybor=="2"){
            menu_dodaj_uwage();
        }
        else if(wybor=="5"){
            menu_wyswietl_historie_uwagi();
        }
        else if(wybor=="3"){
            menu_dodaj_pomiar_wagi();
        }
        else if(wybor=="6"){
            menu_wyswietl_historie_wagi();
        }
        else if(wybor=="10"){
            extern void wydrukuj_raport_pdf();
            wydrukuj_raport_pdf();
        }
        else{
            cout << c(COL_YELLOW, string("Wybrano opcje: ") + wybor) << endl;
            cout << c(COL_YELLOW, "Funkcjonalnosc w trakcie implementacji...") << endl;
            // zamiast zamykać program, wracamy do menu aby użytkownik mógł kontynuować
            cout << c(COL_CYAN, "(wracam do menu)") << endl;
            // krótka pauza — poczekaj na Enter
            cout << "Naciśnij Enter...";
            (void)read_line();
            continue;
        }
        // po wykonaniu operacji - zaczekaj na naciśnięcie enter aby użytkownik mógł przeczytać komunikaty
        cout << "\nNaciśnij Enter aby wrócić do menu...";
        (void)read_line();
    }
}

//Funkcja wyswietlająca menu dodaj pomiar glukozy
void menu_dodaj_pomiar_glukozy(){
    czyscc_ekran();
    const int szerokosc = SZEROKOSC_MENU;
    string pomiar;
    cout<<endl;

    cout << "+" << string(szerokosc, '=') << "+" << endl;
    cout << "|" << string((szerokosc), ' ') << "|" << endl;
    cout << "|" << string((szerokosc - 24) / 2, ' ') << c(COL_MAGENTA + COL_BOLD, "MENU DODAJ POMIAR GLUKOZY") << string((szerokosc - 24) / 2, ' ') << "|" << endl;
    cout << "|" << string((szerokosc), ' ') << "|" << endl;
    cout << "+" << string(szerokosc, '=') << "+" << endl;

    // Pobierz wartość pomiaru od użytkownika
    printf("Podaj wartosc pomiaru glukozy (mg/dL): ? ");
    pomiar = read_line();

    // Walidacja: spróbuj przekonwertować na liczbę (ale zachowamy oryginalny format podany przez użytkownika)
    string pomiar_trim = pomiar;
    // trim
    auto trim = [](string s){
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == string::npos) return string("");
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    };
    pomiar_trim = trim(pomiar_trim);

    try {
        string for_parse = pomiar_trim;
        for (char &c : for_parse) if (c == ',') c = '.';
        // only validate parseable number; we don't need the numeric value here
        stod(for_parse);
    } catch (...) {
        cout << "Nieprawidlowy format liczby. Anulowano." << endl;
        return;
    }

    // Pobranie daty i godziny lokalnej
    time_t now = time(nullptr);
    tm local_tm = *localtime(&now);
    int hour = local_tm.tm_hour;
    // format daty: RRRR-MM-DD
    ostringstream date_ss;
    date_ss << put_time(&local_tm, "%Y-%m-%d");
    string today = date_ss.str();

    // Plik z danymi
    const string filename = "dane.csv";

    // Wczytaj wszystkie linie (jeśli istnieje)
    vector<string> lines;
    ifstream infile(filename);
    if (infile) {
        string line;
        while (getline(infile, line)) lines.push_back(line);
        infile.close();
    } else {
        // jeśli plik nie istnieje, dodaj nagłówek
        lines.push_back("DATA;PRZED_SNIADANIEM;PO_SNIADANIU;PO_OBIEDZIE;PO_KOLACJI;");
    }

    // Znajdź wiersz dla dzisiejszej daty
    int idx = -1;
    for (size_t i = 1; i < lines.size(); ++i) { // pomijamy nagłówek
        // sprawdź czy wiersz zaczyna się od "today;"
        if (lines[i].rfind(today + ";", 0) == 0) {
            idx = (int)i;
            break;
        }
    }

    // Przygotuj pola (DATA, PRZED..., PO..., PO..., PO...)
    vector<string> fields(5, "");
    if (idx == -1) {
        // nie znaleziono - dodaj nowy wiersz na końcu
        fields[0] = today;
        // pozostałe puste
        string newline;
        for (size_t i = 0; i < fields.size(); ++i) {
            newline += fields[i];
            newline += ";";
        }
        lines.push_back(newline);
        idx = (int)lines.size() - 1;
    } else {
        // parsuj istniejący wiersz
        string tmp = lines[idx];
        // usun ewentualny trailing ';' na końcu dla łatwiejszego split
        if (!tmp.empty() && tmp.back() == ';') tmp.pop_back();
        stringstream ss(tmp);
        string cell;
        int i = 0;
        while (getline(ss, cell, ';') && i < 5) {
            fields[i++] = cell;
        }
        // upewnij się, że mamy 5 pól
        while (i < 5) { fields[i++] = ""; }
    }

    // Wybierz kolumnę docelową w zależności od godziny
    // indeksy: 0=DATA,1=PRZED_SNIADANIEM,2=PO_SNIADANIU,3=PO_OBIEDZIE,4=PO_KOLACJI
    int target_col = 1; // domyślnie PRZED_SNIADANIEM
    if (hour >= 6 && hour < 10) target_col = 1;
    else if (hour >= 10 && hour < 12) target_col = 2;
    else if (hour >= 12 && hour < 15) target_col = 3;
    else if (hour >= 15) target_col = 4;
    else /* hour < 6 */ target_col = 1; // wcześnie rano traktujemy jako przed śniadaniem

    // Wstaw wartość: jeśli pole jest puste -> ustaw na dokładnie to, co podał użytkownik
    // jeśli pole niepuste -> zapytaj czy nadpisać
    // zachowaj oryginalny format (po przycięciu)
    if (!fields[target_col].empty()) {
        cout << c(COL_YELLOW, string("W tej kolumnie istnieje już wynik: '") + fields[target_col] + "'.\nCzy nadpisac? (T/N): ");
        string choice;
        choice = read_line();
        if (choice.empty() || (choice[0] != 'T' && choice[0] != 't')) {
            cout << c(COL_RED, "Nie nadpisano istniejącego wyniku.") << endl;
            return;
        }
    }
    fields[target_col] = pomiar_trim;

    // Odbuduj linię i zapisz do pamięci
    string rebuilt;
    for (size_t i = 0; i < fields.size(); ++i) {
        rebuilt += fields[i];
        rebuilt += ";";
    }
    lines[idx] = rebuilt;

    // Zapisz plik z powrotem
    ofstream outfile(filename, ios::trunc);
    if (!outfile) {
        cerr << "Nie mozna zapisac pliku: " << filename << endl;
        return;
    }
    for (const auto &l : lines) outfile << l << "\n";
    outfile.close();

    cout << c(COL_GREEN, string("Pobrano wartosc ") + pomiar_trim + " dla daty " + today + " w kolumnie ");
    switch (target_col) {
        case 1: cout << "PRZED_SNIADANIEM"; break;
        case 2: cout << "PO_SNIADANIU"; break;
        case 3: cout << "PO_OBIEDZIE"; break;
        case 4: cout << "PO_KOLACJI"; break;
        default: cout << "?"; break;
    }
    cout << ".\n";
}

void menu_dodaj_uwage(){
    czyscc_ekran();
    const int szerokosc = SZEROKOSC_MENU;
    string tresc;
    cout << endl;
    cout << "+" << string(szerokosc, '=') << "+" << endl;
    cout << "|" << string((szerokosc), ' ') << "|" << endl;
    cout << "|" << string((szerokosc - 12) / 2, ' ') << c(COL_MAGENTA + COL_BOLD, "DODAJ UWAGE") << string((szerokosc - 12) / 2, ' ') << "|" << endl;
    cout << "|" << string((szerokosc), ' ') << "|" << endl;
    cout << "+" << string(szerokosc, '=') << "+" << endl;

    printf("Podaj tresc uwagi: ? ");
    tresc = read_line();

    // trim
    auto trim = [](string s){
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == string::npos) return string("");
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    };
    tresc = trim(tresc);
    // zastąp średniki przecinkami aby nie łamać struktury CSV
    for (char &c : tresc) if (c == ';') c = ',';

    // data i godzina
    time_t now = time(nullptr);
    tm local_tm = *localtime(&now);
    ostringstream date_ss, time_ss;
    date_ss << put_time(&local_tm, "%Y-%m-%d");
    time_ss << setfill('0') << setw(2) << local_tm.tm_hour << ":" << setw(2) << local_tm.tm_min;
    string today = date_ss.str();
    string godz = time_ss.str();

    const string filename = "uwagi.csv";
    bool needHeader = false;
    ifstream infile(filename);
    if (!infile) needHeader = true;
    else {
        infile.seekg(0, ios::end);
        if (infile.tellg() <= 0) needHeader = true;
    }
    ofstream outfile(filename, ios::app);
    if (!outfile) { cerr << "Nie mozna otworzyc pliku: " << filename << endl; return; }
    if (needHeader) outfile << "DATA;GODZINA;TRESC;" << "\n";
    outfile << today << ";" << godz << ";" << tresc << ";" << "\n";
    outfile.close();

    cout << "Dodano uwage dla daty " << today << " o godzinie " << godz << ".\n";
}


void menu_dodaj_pomiar_wagi()
{
    czyscc_ekran();
    const int szerokosc = SZEROKOSC_MENU;
    string wpis;
    cout << endl;
    cout << "+" << string(szerokosc, '=') << "+" << endl;
    cout << "|" << string((szerokosc), ' ') << "|" << endl;
    cout << "|" << string((szerokosc - 24) / 2, ' ') << c(COL_MAGENTA + COL_BOLD, "DODAJ POMIAR WAGI") << string((szerokosc - 24) / 2, ' ') << "|" << endl;
    cout << "|" << string((szerokosc), ' ') << "|" << endl;
    cout << "+" << string(szerokosc, '=') << "+" << endl;

    // Jeśli plik istnieje, pokaż najnowszy (ostatni) rekord
    const string filename = "waga.csv";
    ifstream infile_latest(filename);
    if (infile_latest) {
        string line;
        string last;
        // pomiń nagłówek
        if (getline(infile_latest, line)) {
            while (getline(infile_latest, line)) {
                if (!line.empty()) last = line;
            }
        }
        infile_latest.close();
        if (!last.empty()) {
            vector<string> fields;
            string tmp;
            stringstream ssl(last);
            while (getline(ssl, tmp, ';')) fields.push_back(tmp);
            cout << c(COL_CYAN, "Ostatni rekord: ");
            if (fields.size() >= 3) cout << fields[0] << " " << fields[1] << " - " << fields[2] << " kg";
            if (fields.size() >= 4) cout << " BMI: " << fields[3];
            if (fields.size() >= 5) cout << " (" << fields[4] << ")";
            cout << "\n\n";
        }
    }

    printf("Podaj wartosc wagi (kg): ? ");
    wpis = read_line();

    // trim
    auto trim = [](string s){
        size_t a = s.find_first_not_of(" \t\r\n");
        if (a == string::npos) return string("");
        size_t b = s.find_last_not_of(" \t\r\n");
        return s.substr(a, b - a + 1);
    };
    wpis = trim(wpis);

    // validate number
    try {
        string for_parse = wpis;
        for (char &c : for_parse) if (c == ',') c = '.';
        double val = stod(for_parse);
        if (!(val > 0)) throw 0;
        // get date/time
        time_t now = time(nullptr);
        tm local_tm = *localtime(&now);
        ostringstream date_ss, time_ss;
        date_ss << put_time(&local_tm, "%Y-%m-%d");
        time_ss << setfill('0') << setw(2) << local_tm.tm_hour << ":" << setw(2) << local_tm.tm_min;
        string today = date_ss.str();
        string godz = time_ss.str();

        const string filename = "waga.csv";
        bool needHeader = false;
        ifstream infile(filename);
        if (!infile) needHeader = true;
        else {
            infile.seekg(0, ios::end);
            if (infile.tellg() <= 0) needHeader = true;
        }

        // Sprawdź, czy w danym dniu już istnieje rekord w pliku waga.csv
        bool exists_today = false;
        int exists_index = -1;
        vector<string> lines;
        if (!needHeader) {
            ifstream inall(filename);
            if (inall) {
                string line;
                while (getline(inall, line)) lines.push_back(line);
                inall.close();
            }
            // sprawdź wczytane linie (pomiń nagłówek)
            for (size_t i = 1; i < lines.size(); ++i) {
                if (lines[i].rfind(today + ";", 0) == 0) {
                    exists_today = true;
                    exists_index = (int)i;
                    break;
                }
            }
        }
        if (exists_today) {
            cout << c(COL_YELLOW, string("Rekord dla dzisiejszej daty już istnieje: \n  ") + lines[exists_index] + "\nCzy nadpisac? (T/N): ");
            string choice;
            choice = read_line();
            if (choice.empty() || (choice[0] != 'T' && choice[0] != 't')) {
                cout << c(COL_RED, "Nie nadpisano istniejącego wpisu.") << endl;
                return;
            }
            // jeśli użytkownik chce nadpisać, zostawiamy miejsce exists_index do zastąpienia
        }

        // Pobierz wzrost użytkownika (cm).
        // Jeśli w kodzie zdefiniowano makro WZROST (>0), użyj go i nie pytaj użytkownika.
        double height_cm = 0.0;
#if defined(WZROST)
        if (WZROST > 0) {
            height_cm = WZROST;
        }
#endif
        const string height_file = "user_height.txt";
        if (height_cm <= 0.0) {
            ifstream hfile_in(height_file);
            if (hfile_in) {
                string hline;
                if (getline(hfile_in, hline)) {
                    try {
                        for (char &c : hline) if (c == ',') c = '.';
                        height_cm = stod(hline);
                    } catch (...) { height_cm = 0.0; }
                }
                hfile_in.close();
            }
        }

        if (height_cm <= 0.0) {
            // zapytaj użytkownika o wzrost
            cout << "Nie znaleziono zapisanego wzrostu. Podaj wzrost w cm (np. 175): ? ";
            string hinput;
            hinput = read_line();
            try {
                for (char &c : hinput) if (c == ',') c = '.';
                height_cm = stod(hinput);
            } catch (...) {
                cout << "Nieprawidlowy wzrost. Anulowano." << endl;
                return;
            }
            // zapytaj czy zapisać
            cout << "Czy zapisac wzrost do pliku do dalszego uzycia? (T/N): ";
            string saveh;
            saveh = read_line();
            if (!saveh.empty() && (saveh[0]=='T' || saveh[0]=='t')) {
                ofstream hfile_out(height_file, ios::trunc);
                if (hfile_out) {
                    hfile_out << fixed << setprecision(1) << height_cm << "\n";
                    hfile_out.close();
                }
            }
        }

        // Oblicz BMI
        double height_m = height_cm / 100.0;
        double bmi = 0.0;
        if (height_m > 0.0) {
            string for_parse = wpis;
            for (char &c : for_parse) if (c == ',') c = '.';
            double val = stod(for_parse);
            bmi = val / (height_m * height_m);
        }

        // Przygotuj INFO: oblicz ile kg do BMI=25 (nadwaga) lub do BMI=18.5 (niedowaga)
        string info;
        if (height_m <= 0.0) {
            info = "brak wzrostu";
        } else {
            
            // parse wpis value
            string for_parse = wpis;
            for (char &c : for_parse) if (c == ',') c = '.';
            double val = stod(for_parse);
                // Określ progi BMI zależne od wieku
                int age = 0;
    #if defined(WIEK)
                age = WIEK;
    #endif
                double lower_bmi, upper_bmi;
                if (age < 45) {
                    lower_bmi = 18.5; upper_bmi = 24.9;
                } else if (age < 65) {
                    lower_bmi = 18.5; upper_bmi = 26.9; // dopuszczalnie wyższy górny próg dla średniego wieku
                } else {
                    lower_bmi = 22.0; upper_bmi = 27.9; // starsi mogą mieć nieco wyższy dopuszczalny zakres
                }

                // Oblicz różnicę w kg do granicy normy (górnej lub dolnej)
                if (bmi > upper_bmi) {
                    double diff = val - (upper_bmi * height_m * height_m);
                    int diff_round = (int)round(diff);
                    info = "nadwaga o " + to_string(diff_round) + " kg";
                } else if (bmi < lower_bmi) {
                    double diff = (lower_bmi * height_m * height_m) - val;
                    int diff_round = (int)round(diff);
                    info = "niedowaga o " + to_string(diff_round) + " kg";
                } else {
                    info = "w normie";
                }

                // Ocena ryzyka łącząca BMI i wiek
                string risk;
                if (bmi >= 35.0) {
                    risk = "bardzo wysokie ryzyko";
                } else if (bmi >= 30.0) {
                    risk = "wysokie ryzyko";
                } else if (bmi > upper_bmi) {
                    risk = "srednie ryzyko";
                } else if (bmi < lower_bmi) {
                    // niedowaga: ocena zależna od odchylenia
                    if ((lower_bmi - bmi) > 2.0) risk = "srednie ryzyko";
                    else risk = "niskie ryzyko";
                } else {
                    risk = "niskie ryzyko";
                }

                // Starszy wiek zwiększa ryzyko przy niekorzystnym BMI
                if (age >= 65 && bmi > upper_bmi) {
                    if (risk == "srednie ryzyko") risk = "wysokie ryzyko";
                }

                info += " - ryzyko: " + risk;
        }

        // Zapis: jeśli wczytaliśmy lines i istniał wpis, zastąp go, inaczej dopisz
        ostringstream bmi_ss;
        bmi_ss << fixed << setprecision(1) << bmi;
        string newline = today + ";" + godz + ";" + wpis + ";" + bmi_ss.str() + ";" + info + ";";
        if (!needHeader) {
            if (exists_today && exists_index >= 0) {
                lines[exists_index] = newline;
                ofstream outrep(filename, ios::trunc);
                if (!outrep) { cerr << "Nie mozna otworzyc pliku: " << filename << endl; return; }
                for (const auto &l : lines) outrep << l << "\n";
                outrep.close();
            } else {
                ofstream outfile(filename, ios::app);
                if (!outfile) { cerr << "Nie mozna otworzyc pliku: " << filename << endl; return; }
                outfile << newline << "\n";
                outfile.close();
            }
        } else {
            ofstream outfile(filename, ios::app);
            if (!outfile) { cerr << "Nie mozna otworzyc pliku: " << filename << endl; return; }
            outfile << "DATA;GODZINA;WAGA;BMI;INFO;" << "\n";
            outfile << newline << "\n";
            outfile.close();
        }

        cout << c(COL_GREEN, string("Dodano pomiar wagi: ") + wpis + " kg dla daty " + today + " o godzinie " + godz + ".") << "\n";
        // Wyświetl dodatkowe informacje dwie linie niżej: BMI i INFO
        cout << "\n\n";
        cout << c(COL_CYAN, string("BMI: ") + bmi_ss.str()) << "\n";
        // INFO kolorystycznie: zielone gdy w normie, żółte gdy nieduża odchyłka, czerwone gdy wysokie
        string info_col = COL_GREEN;
        if (info.find("wysokie") != string::npos || info.find("bardzo wysokie") != string::npos) info_col = COL_RED;
        else if (info.find("srednie") != string::npos) info_col = COL_YELLOW;
        cout << c(info_col, string("INFO: ") + info) << "\n";
    } catch (...) {
        cout << "Nieprawidlowy format wagi. Anulowano." << endl;
        return;
    }
}

// Generuje tekstowy raport z ostatnich N dni i próbuje skonwertować do PDF
void wydrukuj_raport_pdf() {
    czyscc_ekran();
    const int days = 30;
    time_t now = time(nullptr);
    time_t cutoff_t = now - (time_t)days * 24 * 3600;
    tm cut_tm = *localtime(&cutoff_t);
    ostringstream cut_ss;
    cut_ss << put_time(&cut_tm, "%Y-%m-%d");
    string cutoff = cut_ss.str();

    const string reportFile = "report.txt";
    const string reportMD = "report.md";
    ofstream rep(reportFile, ios::trunc);
    ofstream repmd(reportMD, ios::trunc);
    if (!rep || !repmd) { cerr << "Nie mozna utworzyc pliku raportu." << endl; return; }
    rep << "Raport - ostatnie " << days << " dni (od " << cutoff << ")\n\n";
    repmd << "# Raport - ostatnie " << days << " dni (od " << cutoff << ")\n\n";

    auto appendFiltered = [&](const string &fname, const string &title){
        rep << "== " << title << " ==\n";
        repmd << "## " << title << "\n\n";
        ifstream f(fname);
        bool any = false;
        if (f) {
            string header;
            if (getline(f, header)) {
                // parse header columns
                vector<string> cols;
                string cell;
                stringstream hh(header);
                while (getline(hh, cell, ';')) {
                    if (!cell.empty()) cols.push_back(cell);
                }
                // prepare markdown table header if we have columns
                if (!cols.empty()) {
                    repmd << "|";
                    for (const auto &cname : cols) repmd << " " << cname << " |";
                    repmd << "\n|";
                    for (size_t i = 0; i < cols.size(); ++i) repmd << " --- |";
                    repmd << "\n";
                }

                string line;
                while (getline(f, line)) {
                    if (line.size() < 10) continue;
                    string date = line.substr(0, 10);
                    if (date >= cutoff) {
                        rep << line << "\n";
                        // split line into cells and write markdown row
                        vector<string> cells;
                        stringstream ss(line);
                        string part;
                        while (getline(ss, part, ';')) cells.push_back(part);
                        if (!cells.empty()) {
                            repmd << "|";
                            for (size_t i = 0; i < cols.size(); ++i) {
                                string out = "";
                                if (i < cells.size()) out = cells[i];
                                // escape pipe characters if any
                                for (char &ch : out) if (ch == '|') ch = '/';
                                repmd << " " << out << " |";
                            }
                            repmd << "\n";
                        }
                        any = true;
                    }
                }
            }
            f.close();
        }
        if (!any) rep << "(brak danych)\n";
        rep << "\n";
        repmd << "\n";
    };

    appendFiltered("dane.csv", "Pomiar glukozy");
    appendFiltered("waga.csv", "Pomiar wagi");
    appendFiltered("uwagi.csv", "Uwagi");
    rep.close();
    repmd << "\n";
    repmd.close();

    // Spróbuj wygenerować PDF: preferuj pandoc, inaczej enscript+ps2pdf
    bool pdfCreated = false;
    if (system("pandoc --version > /dev/null 2>&1") == 0) {
        string cmd = string("pandoc ") + reportFile + " -o report.pdf";
        if (system(cmd.c_str()) == 0) pdfCreated = true;
    }
    if (!pdfCreated) {
        if (system("enscript --version > /dev/null 2>&1") == 0 && system("ps2pdf -h > /dev/null 2>&1") == 0) {
            string cmd = string("enscript -B -q -p - ") + reportFile + " | ps2pdf - report.pdf";
            if (system(cmd.c_str()) == 0) pdfCreated = true;
        }
    }

    if (pdfCreated) cout << "Wygenerowano plik report.pdf" << endl;
    else {
        // spróbuj jeszcze raz uruchomic pandoc na pliku markdown (jeśli jest zainstalowany)
        if (system("pandoc --version > /dev/null 2>&1") == 0) {
            string cmd = string("pandoc ") + reportMD + " -o report.pdf";
            if (system(cmd.c_str()) == 0) {
                cout << "Wygenerowano plik report.pdf (z markdown)." << endl;
                return;
            }
        }
        cout << "Utworzono tekstowy raport '" << reportFile << "'. Aby uzyskac PDF, zainstaluj 'pandoc' lub 'enscript' i 'ps2pdf'." << endl;
    }
}

void menu_wyswietl_historie_glukozy(){
    czyscc_ekran();
    const int szerokosc = SZEROKOSC_MENU;
    cout << "\n";
    cout << "+" << string(szerokosc, '=') << "+" << endl;
    cout << "|" << string((szerokosc - 26) / 2, ' ') << c(COL_BLUE + COL_BOLD, "HISTORIA POMIAROW GLUKOZY") << string((szerokosc - 26) / 2, ' ') << "|" << endl;
    cout << "+" << string(szerokosc, '=') << "+" << endl;

    const string filename = "dane.csv";
    ifstream f(filename);
    if (!f) {
        cout << c(COL_YELLOW, "(Brak pliku dane.csv)") << "\n";
    } else {
        string line;
        // header
        if (getline(f, line)) {
            // print header labels
            vector<string> cols;
            string cell;
            stringstream ss(line);
            while (getline(ss, cell, ';')) if (!cell.empty()) cols.push_back(cell);
            // print column headers
            cout << c(COL_CYAN + COL_BOLD, "|");
            for (size_t i = 0; i < cols.size(); ++i) {
                cout << " " << c(COL_BOLD, cols[i]) << " |";
            }
            cout << "\n" << string(szerokosc, '-') << "\n";
            // print rows
            while (getline(f, line)) {
                if (line.size() < 1) continue;
                stringstream rs(line);
                vector<string> cells;
                string part;
                while (getline(rs, part, ';')) cells.push_back(part);
                // date first
                if (cells.size() > 0) cout << c(COL_CYAN, cells[0]) << " ";
                for (size_t i = 1; i < cells.size(); ++i) {
                    string out = cells[i].empty() ? "-" : cells[i];
                    string col = COL_GREEN;
                    if (out == "-") col = COL_YELLOW;
                    cout << "| " << c(col, out) << " ";
                }
                cout << "\n";
            }
        }
        f.close();
    }
    cout << "\nNaciśnij Enter aby wrócić...";
    (void)read_line();
}

void menu_wyswietl_historie_uwagi(){
    czyscc_ekran();
    const int szerokosc = SZEROKOSC_MENU;
    cout << "\n";
    cout << "+" << string(szerokosc, '=') << "+" << endl;
    cout << "|" << string((szerokosc - 14) / 2, ' ') << c(COL_BLUE + COL_BOLD, "HISTORIA UWAG") << string((szerokosc - 14) / 2, ' ') << "|" << endl;
    cout << "+" << string(szerokosc, '=') << "+" << endl;

    const string filename = "uwagi.csv";
    ifstream f(filename);
    if (!f) {
        cout << c(COL_YELLOW, "(Brak pliku uwagi.csv)") << "\n";
    } else {
        string line;
        if (getline(f, line)) {
            // header
            cout << c(COL_BOLD, "DATA") << " | " << c(COL_BOLD, "GODZINA") << " | " << c(COL_BOLD, "TRESC") << "\n";
            cout << string(szerokosc, '-') << "\n";
            while (getline(f, line)) {
                if (line.size() < 1) continue;
                stringstream ss(line);
                string d, g, t;
                getline(ss, d, ';');
                getline(ss, g, ';');
                getline(ss, t, ';');
                if (d.empty()) d = "-";
                if (g.empty()) g = "-";
                if (t.empty()) t = "-";
                cout << c(COL_CYAN, d) << " | " << c(COL_GREEN, g) << " | " << c(COL_MAGENTA, t) << "\n";
            }
        }
        f.close();
    }
    cout << "\nNaciśnij Enter aby wrócić...";
    (void)read_line();
}

void menu_wyswietl_historie_wagi(){
    czyscc_ekran();
    const int szerokosc = SZEROKOSC_MENU;
    cout << "\n";
    cout << "+" << string(szerokosc, '=') << "+" << endl;
    cout << "|" << string((szerokosc - 16) / 2, ' ') << c(COL_BLUE + COL_BOLD, "HISTORIA POMIAROW WAGI") << string((szerokosc - 16) / 2, ' ') << "|" << endl;
    cout << "+" << string(szerokosc, '=') << "+" << endl;

    const string filename = "waga.csv";
    ifstream f(filename);
    if (!f) {
        cout << c(COL_YELLOW, "(Brak pliku waga.csv)") << "\n";
    } else {
        string line;
        if (getline(f, line)) {
            // header parse
            vector<string> cols;
            string cell;
            stringstream hh(line);
            while (getline(hh, cell, ';')) if (!cell.empty()) cols.push_back(cell);
            // print header
            for (size_t i = 0; i < cols.size(); ++i) cout << c(COL_BOLD, cols[i]) << (i+1<cols.size()?" | ":"\n");
            cout << string(szerokosc, '-') << "\n";
            while (getline(f, line)) {
                if (line.size() < 1) continue;
                stringstream ss(line);
                vector<string> cells;
                string part;
                while (getline(ss, part, ';')) cells.push_back(part);
                string date = cells.size()>0 && !cells[0].empty() ? cells[0] : "-";
                string godz = cells.size()>1 && !cells[1].empty() ? cells[1] : "-";
                string waga = cells.size()>2 && !cells[2].empty() ? cells[2] : "-";
                string bmi = cells.size()>3 && !cells[3].empty() ? cells[3] : "-";
                string info = cells.size()>4 && !cells[4].empty() ? cells[4] : "-";
                cout << c(COL_CYAN, date) << " | " << c(COL_GREEN, godz) << " | " << c(COL_MAGENTA, waga) << " | " << c(COL_YELLOW, bmi) << " | " << c(COL_RED, info) << "\n";
            }
        }
        f.close();
    }
    cout << "\nNaciśnij Enter aby wrócić...";
    (void)read_line();
}
