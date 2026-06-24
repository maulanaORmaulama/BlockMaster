#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <chrono>
#include <thread>
#include <string>
#include <fstream>
using namespace std;

int offsetX = 40;
const int LEBAR_PAPAN  = 10;   
const int TINGGI_PAPAN = 20;  
int papan[TINGGI_PAPAN][LEBAR_PAPAN];
const int PAPAN_LAYAR_X = 31; 
const int PAPAN_LAYAR_Y = 3;
const int JUMLAH_TIPE = 7;
const string FILE_HIGHSCORE = "highscore.txt";

// =========================WARNA WARNI==============================
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"
#define ORANGE  "\033[38;5;208m"
#define PURPLE  "\033[38;2;128;0;128m"

//====================================score=======================================
struct gameState{
    int score = 0;
    int line = 0;
    int level = 1;
};
// =================KURSOR & LAYAR================================
void setKursorHome() { cout << "\033[H" << flush; }
void hideKursor() { cout << "\033[?25l" << flush; }
void showKursor()    { cout << "\033[?25h" << flush; }
void clearScreen()   { cout << "\033[2J\033[3J\033[1;1H"; }

void setPosisiKursor(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";
}

void gambarKotak(int startX, int startY, int width, int height, string title = "") {
    setPosisiKursor(startX, startY);
    cout << CYAN << "+" << string(width - 2, '-') << "+" << RESET;
    for (int i = 1; i < height - 1; i++) {
        setPosisiKursor(startX, startY + i);
        cout << CYAN << "|" << string(width - 2, ' ') << "|" << RESET;
    }
    setPosisiKursor(startX, startY + height - 1);
    cout << CYAN << "+" << string(width - 2, '-') << "+" << RESET;
    if (title != "") {
        int panjangTeks  = title.length() + 2;
        int offsetTengah = (width - panjangTeks) / 2;
        setPosisiKursor(startX + offsetTengah, startY);
        cout << YELLOW << " " << title << " " << RESET;
    }
}
int hitungDataFile() {
    ifstream file(FILE_HIGHSCORE);
    int jumlah = 0;
    int skor;
    if (file.is_open()) {
        while (file >> skor) {
            jumlah++;
        }
        file.close();
    }
    return jumlah;
}
int* bacaHighScore() {
    int jumlahData = hitungDataFile();
    int ukuranArray = (jumlahData < 7) ? 7 : jumlahData;
    int* listSkor = new int[ukuranArray];
    for (int i = 0; i < ukuranArray; i++) {
        listSkor[i] = 0;
    }
    ifstream file(FILE_HIGHSCORE);
    if (file.is_open()) {
        int i = 0;
        int skor;
        while (file >> skor && i < ukuranArray) {
            listSkor[i] = skor;
            i++;
        }
        file.close();
    }
    for (int i = 0; i < ukuranArray - 1; i++) {
        for (int j = 0; j < ukuranArray - i - 1; j++) {
            if (listSkor[j] < listSkor[j + 1]) {
                int temp = listSkor[j];
                listSkor[j] = listSkor[j + 1];
                listSkor[j + 1] = temp;
            }
        }
    }
    
    return listSkor;
}
void simpanHighScore(int skorBaru) {
    int jumlahDataLama = hitungDataFile();
    int ukuranLama = (jumlahDataLama < 7) ? 7 : jumlahDataLama;
    int* dataLama = bacaHighScore();
    int ukuranBaru = ukuranLama + 1;
    int* listSkor = new int[ukuranBaru];
    for (int i = 0; i < ukuranLama; i++) {
        listSkor[i] = dataLama[i];
    }
    listSkor[ukuranBaru - 1] = skorBaru;
    delete[] dataLama;
    for (int i = 0; i < ukuranBaru - 1; i++) {
        for (int j = 0; j < ukuranBaru - i - 1; j++) {
            if (listSkor[j] < listSkor[j + 1]) {
                int temp = listSkor[j];
                listSkor[j] = listSkor[j + 1];
                listSkor[j + 1] = temp;
            }
        }
    }
    ofstream file(FILE_HIGHSCORE);
    if (file.is_open()) {
        for (int i = 0; i < 7; i++) {
            file << listSkor[i] << "\n";
        }
        file.close();
    }
    delete[] listSkor;
}
void hapusDalamKotak(int startX, int startY, int lebar, int tinggi) {
    string spasiPembersih(lebar - 2, ' ');
    for (int i = 1; i < tinggi - 1; i++) {
        setPosisiKursor(startX + 1, startY + i);
        cout << spasiPembersih;
    }
}
int semuaBentuk[JUMLAH_TIPE][4][4] = {
    // I - RED 
    {{0,0,0,0},
     {1,1,1,1},
     {0,0,0,0},
     {0,0,0,0}},
     
    // O - YELLOW 
    {{0,0,0,0},
     {0,1,1,0},
     {0,1,1,0},
     {0,0,0,0}},
     
    // T - MAGENTA 
    {{0,0,0,0},
     {0,1,0,0},
     {1,1,1,0},
     {0,0,0,0}},
     
    // S - GREEN 
    {{0,0,0,0},
     {0,1,1,0},
     {1,1,0,0},
     {0,0,0,0}},
     
    // Z - GREEN 
    {{0,0,0,0},
     {1,1,0,0},
     {0,1,1,0},
     {0,0,0,0}},
     
    // J - ORANGE 
    {{0,0,0,0},
     {1,0,0,0},
     {1,1,1,0},
     {0,0,0,0}},
     
    // L - ORANGE 
    {{0,0,0,0},
     {0,0,1,0},
     {1,1,1,0},
     {0,0,0,0}}
};
string warnaBlok[JUMLAH_TIPE] = {
    RED, YELLOW, MAGENTA, GREEN, GREEN, ORANGE, ORANGE
};
bool posisiValid(int bentuk[4][4], int px, int py) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (bentuk[i][j] == 0) continue;
            int nx = px + j;
            int ny = py + i;
            if (nx < 0 || nx >= LEBAR_PAPAN || ny >= TINGGI_PAPAN)
                return false;
            if (ny >= 0 && papan[ny][nx] != 0)
                return false;
        }
    }
    return true;
}
void kunciBlok(int bentuk[4][4], int px, int py, int tipe) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (bentuk[i][j] != 0)
                papan[py + i][px + j] = tipe + 1;
}
void hapusBarisPenuh(gameState &state) {
    int barisBersih = 0;
    for (int i = TINGGI_PAPAN - 1; i >= 0; i--) {
        bool penuh = true;
        for (int j = 0; j < LEBAR_PAPAN; j++) {
            if (papan[i][j] == 0) {
                penuh = false;
                break;
            }
        }

        if (penuh) {
            for (int y = i; y > 0; y--) {
                for (int x = 0; x < LEBAR_PAPAN; x++) {
                    papan[y][x] = papan[y - 1][x];
                }
            }
            for (int x = 0; x < LEBAR_PAPAN; x++) {
                papan[0][x] = 0;
            }
            barisBersih++;
            i++;
        }
    }
    if (barisBersih > 0) {
        int baseScore = 0;
        switch (barisBersih) {
            case 1: baseScore = 100;  break;
            case 2: baseScore = 300;  break;
            case 3: baseScore = 500;  break;
            case 4: baseScore = 800;  break; 
            default: baseScore = 800; break; 
        }
        state.line += barisBersih;
        state.score += baseScore * state.level;
        if (state.line >= state.level * 10) {
        state.level++;
        }
        setPosisiKursor(56 + offsetX, 14); cout << "SCORE: " << GREEN << state.score << "  " << RESET;
        setPosisiKursor(56 + offsetX, 16); cout << "LEVEL: " << WHITE << state.level << "  " << RESET;
        setPosisiKursor(56 + offsetX, 18); cout << "LINES: " << WHITE << state.line << "  " << RESET;
    }
}
void spawnBlokBaru(int bentukAktif[4][4], int bentuknext[4][4], int &blokX, int &blokY) {
    blokX     = LEBAR_PAPAN / 2 - 2;  
    blokY     = 0;     
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            bentukAktif[i][j] = bentuknext[i][j];
        }
    }
}
void bloknext(int bentuknext[4][4], int &tipeAktif, int &blokX, int &blokY) {
    tipeAktif = rand() % JUMLAH_TIPE; 
    blokX     = LEBAR_PAPAN / 2 - 2;  
    blokY     = 0;                     

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            bentuknext[i][j] = semuaBentuk[tipeAktif][i][j];
        }
    }
}
void renderPapan(int bentukAktif[4][4], int tipeAktif, int blokX, int blokY) {
    int tampilan[TINGGI_PAPAN][LEBAR_PAPAN];
    for (int i = 0; i < TINGGI_PAPAN; i++)
        for (int j = 0; j < LEBAR_PAPAN; j++)
            tampilan[i][j] = papan[i][j];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (bentukAktif[i][j] != 0) {
                int ny = blokY + i;
                int nx = blokX + j;
                if (ny >= 0 && ny < TINGGI_PAPAN && nx >= 0 && nx < LEBAR_PAPAN)
                    tampilan[ny][nx] = tipeAktif + 1;
            }
    string bufferPapan = "";
    for (int i = 0; i < TINGGI_PAPAN; i++) {
        bufferPapan += "\033[" + to_string(PAPAN_LAYAR_Y + i) + ";" + to_string(PAPAN_LAYAR_X + offsetX) + "H";
        
        for (int j = 0; j < LEBAR_PAPAN; j++) {
            int val = tampilan[i][j];
            if (val == 0) {
                bufferPapan += string(GRAY) + " ." + string(RESET);  
            } else {
                bufferPapan += warnaBlok[val - 1] + "[]" + string(RESET);
            }
        }
    }
    cout << bufferPapan << flush;
}
void renderHold(int holdTipe){
    int startX = 17 + offsetX;
    float startY = 7.5;
    for(int i=0;i<4;i++){
        setPosisiKursor(startX, startY+i);
        cout << "        ";
    }

    if(holdTipe == -1)
        return;

    for(int i=0;i<4;i++){
        setPosisiKursor(startX, startY+i);

        for(int j=0;j<4;j++)
        {
            if(semuaBentuk[holdTipe][i][j])
            {
                cout << warnaBlok[holdTipe]
                     << "[]"
                     << RESET;
            }else{
                cout << "  ";
            }
        }
    }
}
void gameOver(int skorAkhir) {
    simpanHighScore(skorAkhir);

    gambarKotak(30 + offsetX, 2, 22, 22, "TETRIS");
    for (int i = 0; i < 5; i++) {
        setPosisiKursor(37 + offsetX, 12);
        cout << RED << "\033[1m" << "GAME OVER" << RESET;
        this_thread::sleep_for(chrono::milliseconds(200));
        setPosisiKursor(37 + offsetX, 12);
        cout << "         ";
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    setPosisiKursor(37 + offsetX, 12);
    cout << RED << "\033[1m" << "GAME OVER" << RESET;
    while (_kbhit()) { _getch(); }
    _getch();
}
void gameLoopDenganGravitasi() {
    gameState state;
    for(int i = 0; i < TINGGI_PAPAN; i++) {
        for(int j = 0; j < LEBAR_PAPAN; j++) {
            papan[i][j] = 0;
        }
    }
    for (int i = 0; i < TINGGI_PAPAN; i++) {
        for (int j = 0; j < LEBAR_PAPAN; j++) {
            papan[i][j] = 0;
        }
    }

    srand(time(0)); 
    int bentukAktif[4][4];
    int bentuknext[4][4];
    int tipeAktif, tipeNext, blokX, blokY; 
    int holdTipe = -1;
    bool sudahHold = false; 
    bloknext(bentuknext, tipeNext, blokX, blokY); 
    tipeAktif = tipeNext;
    spawnBlokBaru(bentukAktif, bentuknext, blokX, blokY);
    bloknext(bentuknext, tipeNext, blokX, blokY); 
    gambarKotak(54 + offsetX, 6, 14, 6, "NEXT");
    for (int i = 0; i < 4; i++) {
        setPosisiKursor(58 + offsetX, 7 + i);
        for (int j = 0; j < 4; j++) {
            if (bentuknext[i][j] == 1) cout << warnaBlok[tipeNext] << "[]" << RESET; 
            else cout << "  ";
        }
    }
    auto waktuTerakhir = chrono::steady_clock::now();
    int intervalMs = 500;
    bool gameOverFlag = false;
    int jarakSoftDrop = 0;
    while (!gameOverFlag) {
        intervalMs = 500 - ((state.level - 1) * 50);
        if (intervalMs < 100) {
            intervalMs = 100; 
        }
        auto sekarang = chrono::steady_clock::now();
        int selisihMs = (int)chrono::duration_cast<chrono::milliseconds>(sekarang - waktuTerakhir).count();

        if (selisihMs >= intervalMs) {
            waktuTerakhir = sekarang;
            if (posisiValid(bentukAktif, blokX, blokY + 1)) {
                blokY++;
            } else {
                state.score += jarakSoftDrop;
                jarakSoftDrop = 0;
                setPosisiKursor(56 + offsetX, 14); cout << "SCORE: " << GREEN << state.score << "    " << RESET;
                kunciBlok(bentukAktif, blokX, blokY, tipeAktif);
                hapusBarisPenuh(state);
                tipeAktif = tipeNext; 
                spawnBlokBaru(bentukAktif, bentuknext, blokX, blokY);
                bloknext(bentuknext, tipeNext, blokX, blokY);
                sudahHold = false; 
                gambarKotak(54 + offsetX, 6, 14, 6, "NEXT");
                for (int i = 0; i < 4; i++) {
                    setPosisiKursor(58 + offsetX, 7 + i);
                    for (int j = 0; j < 4; j++) {
                        if (bentuknext[i][j] == 1) cout << warnaBlok[tipeNext] << "[]" << RESET;
                        else cout << "  ";
                    }
                }
                if (!posisiValid(bentukAktif, blokX, blokY)) {
                    gameOverFlag = true;
                    break;
                }
            }
        }

        if (_kbhit()) {
            char k = _getch();
            if ((k == 'c' || k == 'C') && !sudahHold){
                sudahHold = true;
                if (holdTipe == -1){
                    holdTipe = tipeAktif;
                    spawnBlokBaru(bentukAktif, bentuknext, blokX, blokY);
                    bloknext(bentuknext, tipeAktif, blokX, blokY); 
                }
                else{
                    int temp = holdTipe;
                    holdTipe = tipeAktif;
                    tipeAktif = temp;
                    blokX = LEBAR_PAPAN / 2 - 2;
                    blokY = 0;

                    for(int i=0;i<4;i++){
                        for(int j=0;j<4;j++){
                            bentukAktif[i][j] =
                                semuaBentuk[tipeAktif][i][j];
                        }
                    }

                    if(!posisiValid(bentukAktif,blokX,blokY)){
                        gameOverFlag = true;
                    }
                }
            }
            if (k == 27) break;
            if (k == 0 || k == (char)224) {
                k = _getch();
                if (k == 72) {
                    int temp[4][4];
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            temp[j][3 - i] = bentukAktif[i][j];
                    if (posisiValid(temp, blokX, blokY))
                        for (int i = 0; i < 4; i++)
                            for (int j = 0; j < 4; j++)
                                bentukAktif[i][j] = temp[i][j];
                }
                if (k == 75 && posisiValid(bentukAktif, blokX - 1, blokY)) blokX--;
                if (k == 77 && posisiValid(bentukAktif, blokX + 1, blokY)) blokX++; 
                if (k == 80 && posisiValid(bentukAktif, blokX, blokY + 1)) {
                    blokY++;
                    jarakSoftDrop++;
                    setPosisiKursor(56 + offsetX, 14); cout << "SCORE: " << GREEN << state.score << "    " << RESET;
                    waktuTerakhir = chrono::steady_clock::now();
                }
            }
            if (k == 32) {
                int jarakJatuh = 0;
                while (posisiValid(bentukAktif, blokX, blokY + 1)){
                    blokY++;
                    jarakJatuh++;
                }
                state.score += (jarakJatuh * 2);
                setPosisiKursor(56 + offsetX, 14); cout << "SCORE: " << GREEN << state.score << "    " << RESET;
                kunciBlok(bentukAktif, blokX, blokY, tipeAktif);
                hapusBarisPenuh(state);
                tipeAktif = tipeNext; 
                spawnBlokBaru(bentukAktif, bentuknext, blokX, blokY);
                bloknext(bentuknext, tipeNext, blokX, blokY);
                sudahHold = false; 
                gambarKotak(54 + offsetX, 6, 14, 6, "NEXT");
                for (int i = 0; i < 4; i++) {
                    setPosisiKursor(58 + offsetX, 7 + i);
                    for (int j = 0; j < 4; j++) {
                        if (bentuknext[i][j] == 1) cout << warnaBlok[tipeNext] << "[]" << RESET;
                        else cout << "  ";
                    }
                }
                if (!posisiValid(bentukAktif, blokX, blokY)) {
                    gameOverFlag = true;
                }
            }
        }
        renderPapan(bentukAktif, tipeAktif, blokX, blokY);
        renderHold(holdTipe);
        this_thread::sleep_for(chrono::milliseconds(16));
        
    }
    int skorAkhir = state.score;
    gameOver(skorAkhir);
}
void layarGameplay() {
    clearScreen();
    gambarKotak(30 + offsetX, 2, 22, 22, "TETRIS");       
    gambarKotak(54 + offsetX, 6, 14, 6, "NEXT");         
    gambarKotak(14 + offsetX, 6, 14, 6, "HOLD");         
    gambarKotak(10 + offsetX, 12, 18, 8, "HIGH SCORE");
int* scores = bacaHighScore();
for (int i = 0; i < 3; i++) {
    setPosisiKursor(15 + offsetX, 14 + i);
    if (i == 0) {
        cout << GREEN  << "1. " << scores[i] << RESET;
    } 
    else if (i == 1) {
        cout << YELLOW << "2. " << scores[i] << RESET;
    } 
    else {
        cout << ORANGE  << "3. " << scores[i] << RESET;
    }
}
delete[] scores;
    gambarKotak(54 + offsetX, 12, 18, 8, "STATS");
    setPosisiKursor(56 + offsetX, 14);  cout << "SCORE: " << GREEN << "0" << "     " << RESET;
    setPosisiKursor(56 + offsetX, 16);  cout << "LEVEL: " << WHITE << "1" << "     " << RESET;
    setPosisiKursor(56 + offsetX, 18);  cout << "LINES: " << WHITE << "0" << "     " << RESET;
    gameLoopDenganGravitasi(); 
}
void gambarTampilanUtama() {
    clearScreen();
    gambarKotak(54 + offsetX, 6, 16, 10);
    setPosisiKursor(58 + offsetX, 6);  cout << YELLOW << "CREDITS" << RESET;
    setPosisiKursor(58 + offsetX, 8);  cout << "-ISNA";
    setPosisiKursor(58 + offsetX, 9);  cout << "-BINTANG";
    setPosisiKursor(58 + offsetX, 10); cout << "-EZRA";
    setPosisiKursor(58 + offsetX, 11); cout << "-NISA";
    setPosisiKursor(58 + offsetX, 12); cout << "-SARAH";
    setPosisiKursor(58 + offsetX, 13); cout << "-HABIB";
    setPosisiKursor(58 + offsetX, 14); cout << "-AURA";

    gambarKotak(30 + offsetX, 3, 22, 17, "");
    setPosisiKursor(38 + offsetX, 5); cout << RED << "T" << GREEN << "E" << YELLOW << "T" << BLUE << "R" << MAGENTA << "I" << CYAN << "S" << RESET;
    setPosisiKursor(31 + offsetX, 6); cout << "--------------------";
}

int menuUtama() {
    clearScreen();
    string opsi[] = {
        "  PLAY GAME  ",
        " HOW TO PLAY ",
        "  QUIT GAME  "
    };
    int sel = 0;
    char key;
    gambarTampilanUtama();
    gambarKotak(12 + offsetX, 6, 16, 10);
    hapusDalamKotak(11 + offsetX, 3, 18, 15);
    gambarKotak(12 + offsetX, 6, 16, 10);
    gambarKotak(12 + offsetX, 6, 16, 10, "TOP SCORES");
int* scores = bacaHighScore();
for (int i = 0; i < 7; i++) {
    setPosisiKursor(16 + offsetX, 8 + i);   
    if (i == 0) {
        cout << GREEN  << i + 1 << ". " << scores[i] << RESET;
    } 
    else if (i == 1) {
        cout << YELLOW << i + 1 << ". " << scores[i] << RESET;
    }
    else if (i == 2){
        cout << ORANGE << i + 1 << ". " << scores[i] << RESET;
    } 
    else {
        cout << WHITE  << i + 1 << ". " << scores[i] << RESET;
    }
}
delete[] scores;

    while (true) {
        for (int i = 0; i < 3; i++) {
            setPosisiKursor(33 + offsetX, 9 + (i * 2));
            if (i == sel)
                cout << GREEN << "\033[1m" << "> " << opsi[i] << RESET;
            else
                cout << WHITE << "  " << opsi[i] << "  " << RESET;
        }
        setPosisiKursor(1, 25);
        key = _getch();
        if (key == 0 || key == (char)224) {
            key = _getch();
            if (key == 72) { sel--; if (sel < 0) sel = 2; }
            if (key == 80) { sel++; if (sel > 2) sel = 0; }
        } else if (key == 13) {
            return sel;
        }
    }
}

bool sleepAtauInterupsi(int totalWaktuMs, int& halaman, bool& keluar) {
    int waktuCheck = 10; 
    int akumulasi = 0;
    
    while (akumulasi < totalWaktuMs) {
        this_thread::sleep_for(chrono::milliseconds(waktuCheck));
        akumulasi += waktuCheck;
        
        if (_kbhit()) {
            char tombol = _getch();
            if (tombol == 'n' || tombol == 'N') {
                if (halaman < 4) halaman++;
                return true; 
            }
            else if (tombol == 'b' || tombol == 'B') {
                if (halaman > 1) halaman--;
                return true; 
            }
            else if (tombol == 'k' || tombol == 'K') {
                keluar = true;
                return true; 
            }
        }
    }
    return false; 
}

void tutorialGerak(int& halaman, bool& keluar) {
    hideKursor();
    int Kiri = 17, Kanan = 24;
    int posisi = Kanan;
    int arah = -1;
    const int lebarDalam = 59;
    
    setPosisiKursor(20 + offsetX, 5); cout << GREEN << "[1] MOVE            " << RESET;
    setPosisiKursor(20 + offsetX, 6); cout << "Use left and right navigation keyboard buttons to move ";
    setPosisiKursor(20 + offsetX, 7); cout << "block to left and right.                               ";
    
    while (true) {
        if (sleepAtauInterupsi(100, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
        setPosisiKursor(20 + offsetX, 18);
        if (arah == -1) cout << "BUTTON:" << GREEN << "   [ \u2190 ]" << RESET << "   [ \u2192 ]";
        else            cout << "BUTTON:   [ \u2190 ]" << GREEN << "   [ \u2192 ]" << RESET;
        
        setPosisiKursor(16 + offsetX + 1, 9);
        for (int i = 0; i < posisi + 2; i++) cout << " ";
        cout << GREEN << "[][]" << RESET;
        int sisaSpasiAtas = lebarDalam - (posisi + 2) - 4;
        for (int i = 0; i < sisaSpasiAtas; i++) cout << " ";
        
        setPosisiKursor(16 + offsetX + 1, 10);
        for (int i = 0; i < posisi; i++) cout << " ";
        cout << GREEN << "[][]" << RESET;
        int sisaSpasiBawah = lebarDalam - posisi - 4;
        for (int i = 0; i < sisaSpasiBawah; i++) cout << " ";
        
        posisi += arah;
        if (posisi <= Kiri)  arah =  1;
        else if (posisi >= Kanan) arah = -1;
    }
}

void tutorialUbah(int& halaman, bool& keluar) {
    hideKursor();
    const int spasi = 25;
    const int lebarDalam = 59;
    
    setPosisiKursor(20 + offsetX, 5); cout << GREEN << "[2] CHANGE DIRECTION" << RESET;
    setPosisiKursor(20 + offsetX, 6); cout << "Use the top navigation keyboard buttons to change          ";
    setPosisiKursor(20 + offsetX, 7); cout << "the block shape.                                           ";
    
    while (true) {
        for (int rotasi = 0; rotasi < 4; rotasi++) {
            setPosisiKursor(16 + offsetX + 1, 9);  for(int i = 0; i < lebarDalam; i++) cout << " ";
            setPosisiKursor(16 + offsetX + 1, 10); for(int i = 0; i < lebarDalam; i++) cout << " ";
            setPosisiKursor(16 + offsetX + 1, 11); for(int i = 0; i < lebarDalam; i++) cout << " ";

            setPosisiKursor(16 + offsetX + 1, 9);
            if(rotasi == 0 || rotasi == 1 || rotasi == 3){
                for(int i = 0; i < spasi + 2; i++) cout << " ";
                cout << PURPLE << "[]" << RESET;
                int sisa = lebarDalam - (spasi + 2) - 4;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            else if (rotasi == 2){
                for(int i = 0; i < spasi; i++) cout << " ";
                cout << PURPLE << "[][][]" << RESET;
                int sisa = lebarDalam - spasi - 4;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            
            setPosisiKursor(16 + offsetX + 1, 10);
            if(rotasi == 0){
                for(int i = 0; i < spasi; i++) cout << " ";
                cout << PURPLE << "[][][]" << RESET;
                int sisa = lebarDalam - spasi - 4;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            else if(rotasi == 1){
                for(int i = 0; i < spasi; i++) cout << " ";
                cout << PURPLE << "[][]" << RESET;
                int sisa = lebarDalam - spasi - 4;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            else if(rotasi == 2){
                for(int i = 0; i < spasi + 2; i++) cout << " ";
                cout << PURPLE << "[]" << RESET;
                int sisa = lebarDalam - (spasi + 2) - 4;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            else if(rotasi == 3){
                for(int i = 0; i < spasi; i++) cout << " ";
                cout << PURPLE << "[][]" << RESET;
                int sisa = lebarDalam - spasi - 4;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            
            setPosisiKursor(16 + offsetX + 1, 11);
            if(rotasi == 1 || rotasi == 3){
                for(int i = 0; i < spasi + 2; i++) cout << " ";
                cout << PURPLE << "[]" << RESET;
                int sisa = lebarDalam - (spasi + 2) - 4;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            else {
                for(int i = 0; i < lebarDalam; i++) cout << " ";
            }
            
            setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:" << GREEN << "    [ \u2191 ]                         " << RESET;
            if (sleepAtauInterupsi(400, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            
            setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:" << "    [ \u2191 ]                     " << RESET;
            if (sleepAtauInterupsi(150, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
        }
    }
}

void tutorialTurunCepat(int& halaman, bool& keluar) {
    hideKursor();
    const int spasi = 23;
    const int lebarDalam = 59;
    
    setPosisiKursor(20 + offsetX, 5); cout << GREEN << "[3] DROP BLOCK" << RESET;
    setPosisiKursor(20 + offsetX, 6); cout << "Use [\u2193] for Soft Drop or [Space] for instant Hard Drop";
    
    while (true) {
        for (int tinggi = 0; tinggi <= 2; tinggi++) {
            for (int b = 0; b < 3; b++) {
                setPosisiKursor(16 + offsetX + 1, 9 + b);
                if (b == tinggi) {
                    for (int i = 0; i < spasi; i++) cout << " ";
                    cout << RED << "[][][][]" << RESET;
                    int sisa = lebarDalam - spasi - 4;
                    for (int i = 0; i < sisa; i++) cout << " ";
                } else {
                    for (int i = 0; i < lebarDalam; i++) cout << " ";
                }
            }
            setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:   " << GREEN << "[ \u2193 ]" << RESET << "   [ Space ]                               ";
            if (sleepAtauInterupsi(250, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            
            setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:   [ \u2193 ]   [ Space ]                               ";
            if (sleepAtauInterupsi(100, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
        }
        
        if (sleepAtauInterupsi(400, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
        
        for (int f = 0; f < 2; f++) {
            int tinggiHardDrop = (f == 0) ? 0 : 2;
            for (int b = 0; b < 3; b++) {
                setPosisiKursor(16 + offsetX + 1, 9 + b);
                if (b == tinggiHardDrop) {
                    for (int i = 0; i < spasi; i++) cout << " ";
                    cout << RED << "[][][][]" << RESET;
                    int sisa = lebarDalam - spasi - 4;
                    for (int i = 0; i < sisa; i++) cout << " ";
                } else {
                    for (int i = 0; i < lebarDalam; i++) cout << " ";
                }
            }
            
            setPosisiKursor(20 + offsetX, 18);
            if (f == 0) cout << "BUTTON:   [ \u2193 ]   " << GREEN << "[ Space ]" << RESET << "                               ";
            else        cout << "BUTTON:   [ \u2193 ]   [ Space ]                               ";
            
            if (f == 0) {
                if (sleepAtauInterupsi(150, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            } else {
                if (sleepAtauInterupsi(600, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            }
        }
    }
}

void tutorialHold(int& halaman, bool& keluar) {
    hideKursor();
    const int lebarDalam = 59;
    
    setPosisiKursor(20 + offsetX, 5); cout << GREEN << "[4] HOLD BLOCK" << RESET;
    setPosisiKursor(20 + offsetX, 6); cout << "Press [ C ] to store the current block into the HOLD box";
    
    while (true) {
        for (int frame = 0; frame < 2; frame++) {
            setPosisiKursor(16 + offsetX + 1, 9);
            cout << "   +--- " << YELLOW << "HOLD" << RESET << " ---+                                          ";
            
            setPosisiKursor(16 + offsetX + 1, 10);
            cout << "   |            | ";
            for (int i = 0; i < 15; i++) cout << " ";
            cout << RED << "[][][][]" << RESET;
            int sisa = lebarDalam - 18 - 15 - 4;
            for (int i = 0; i < sisa; i++) cout << " ";
            
            setPosisiKursor(16 + offsetX + 1, 11);
            cout << "   +------------+                                          ";
            
            setPosisiKursor(20 + offsetX, 18);
            if (frame == 0) cout << "BUTTON:   " << GREEN << "[ C ]" << RESET << "                                           ";
            else            cout << "BUTTON:   [ C ]                                           ";
            
            if (frame == 0) {
                if (sleepAtauInterupsi(400, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            } else {
                if (sleepAtauInterupsi(150, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            }
        }
        
        for (int frame = 0; frame < 2; frame++) {
            setPosisiKursor(16 + offsetX + 1, 9);
            cout << "   +--- " << YELLOW << "HOLD" << RESET << " ---+                                          ";
            
            setPosisiKursor(16 + offsetX + 1, 10);
            cout << "   |  " << RED << "[][][][]" << RESET << "  |                                          ";
            
            setPosisiKursor(16 + offsetX + 1, 11);
            cout << "   +------------+                                          ";
            
            setPosisiKursor(20 + offsetX, 18);
            cout << "BUTTON:   [ C ]                                           ";
            
            if (frame == 0) {
                if (sleepAtauInterupsi(600, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            } else {
                if (sleepAtauInterupsi(200, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            }
        }
    }
}
void howToPlay() {
    clearScreen();
    int halaman = 1;
    bool keluar = false;
    gambarKotak(16 + offsetX, 3, 70, 17, "HOW TO PLAY");
    setPosisiKursor(26 + offsetX, 20); cout << "[N] Next Page  |  [B] Back Page  |  [K] Menu Exit           ";
    while (!keluar) {
        if      (halaman == 1) tutorialGerak(halaman, keluar);
        else if (halaman == 2) tutorialUbah(halaman, keluar);
        else if (halaman == 3) tutorialTurunCepat(halaman, keluar);
        else if (halaman == 4) tutorialHold(halaman, keluar);
    }
    clearScreen();
    showKursor();
}
int main() {
    // buatMusikTetris();
    // buatMusikGameOver();
    system("chcp 65001 > nul");
    cout << "\033[?25l";

    system("chcp 65001 > nul");
    cout << "\033[?25l";
    bool aplikasiBerjalan = true;

    while (aplikasiBerjalan) {
        int pilihanUser = menuUtama();

        if (pilihanUser == 0) {
            layarGameplay();     
        } else if (pilihanUser == 1) {
            howToPlay();
        } else if (pilihanUser == 2) {
            clearScreen();
            setPosisiKursor(15 + offsetX, 2); cout << GREEN << R"( _____ _   _    _    _   _ _  __ __   _____  _   _ )" << RESET << "\n";
            setPosisiKursor(15 + offsetX, 3); cout << GREEN << R"(|_   _| | | |  / \  | \ | | |/ / \ \ / / _ \| | | |)" << RESET << "\n";
            setPosisiKursor(15 + offsetX, 4); cout << GREEN << R"(  | | | |_| | / _ \ |  \| | ' /   \ V / | | | | | |)" << RESET << "\n";
            setPosisiKursor(15 + offsetX, 5); cout << GREEN << R"(  | | |  _  |/ ___ \| |\  | . \    | || |_| | |_| |)" << RESET << "\n";
            setPosisiKursor(15 + offsetX, 6); cout << GREEN << R"(  |_| |_| |_/_/   \_\_| \_|_|\_\   |_| \___/ \___/ )" << RESET << "\n";
            aplikasiBerjalan = false;
        }
    }
    cout << "\033[?25h";
    return 0;
}