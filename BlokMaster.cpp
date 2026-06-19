#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <chrono>
#include <thread>
#include <string>
using namespace std;

int offsetX = 50;

// =========================WARNA WARNI==============================
#define BOLD    "\033[1m"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"
#define PURPLE  "\033[38;2;128;0;128m"

// ==========================================
// 1. FUNGSI UTILITAS (KURSOR & LAYAR)
// ==========================================
void setKursorHome() { cout << "\033[H" << flush; }
void hideKursor()    { cout << "\033[?25l" << flush; }
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

void hapusDalamKotak(int startX, int startY, int lebar, int tinggi) {
    string spasiPembersih(lebar - 2, ' ');
    for (int i = 1; i < tinggi - 1; i++) {
        setPosisiKursor(startX + 1, startY + i);
        cout << spasiPembersih;
    }
}

// ==========================================
// [BAGIAN BINTANG] KONSTANTA PAPAN
// ==========================================
const int LEBAR_PAPAN  = 10;   // 10 kolom
const int TINGGI_PAPAN = 20;   // 20 baris
int papan[TINGGI_PAPAN][LEBAR_PAPAN];  // 0=kosong, 1-7=warna blok

// Posisi kiri-atas area permainan di layar (dalam karakter)
// Papan dimulai di dalam kotak TETRIS (x=31+offsetX, y=3)
const int PAPAN_LAYAR_X = 31; // kolom awal (relatif, nanti ditambah offsetX)
const int PAPAN_LAYAR_Y = 3;  // baris awal

// ==========================================
// [BAGIAN BINTANG] DATA 7 TETROMINO
// ==========================================
const int JUMLAH_TIPE = 7;

int semuaBentuk[JUMLAH_TIPE][4][4] = {
    // I - CYAN
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
    {{0,1,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    // S - GREEN
    {{0,1,1,0},
     {1,1,0,0},
     {0,0,0,0},
     {0,0,0,0}},
    // Z - RED
    {{1,1,0,0},
     {0,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    // J - BLUE
    {{1,0,0,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}},
    // L - WHITE
    {{0,0,1,0},
     {1,1,1,0},
     {0,0,0,0},
     {0,0,0,0}}
};

// Warna tiap tipe (indeks sama dengan semuaBentuk)
string warnaBlok[JUMLAH_TIPE] = {
    CYAN, YELLOW, MAGENTA, GREEN, RED, BLUE, WHITE
};

// ==========================================
// [BAGIAN BINTANG] CEK POSISI VALID
// Dipanggil SEBELUM setiap gerakan/rotasi.
// Mengembalikan true jika blok boleh ada di (px, py).
// ==========================================
bool posisiValid(int bentuk[4][4], int px, int py) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (bentuk[i][j] == 0) continue;
            int nx = px + j;
            int ny = py + i;
            // Keluar batas kiri/kanan/bawah
            if (nx < 0 || nx >= LEBAR_PAPAN || ny >= TINGGI_PAPAN)
                return false;
            // Nabrak blok yang sudah terkunci
            if (ny >= 0 && papan[ny][nx] != 0)
                return false;
        }
    }
    return true;
}

// ==========================================
// [BAGIAN BINTANG] KUNCI BLOK KE PAPAN
// Dipanggil saat blok tidak bisa turun lagi.
// ==========================================
void kunciBlok(int bentuk[4][4], int px, int py, int tipe) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (bentuk[i][j] != 0)
                papan[py + i][px + j] = tipe + 1; // simpan 1-7
}

void hapusBarisPenuh() {
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

            i++;
        }
    }
}

// ==========================================
// [BAGIAN BINTANG] SPAWN BLOK BARU (ACAK)
// Pilih satu dari 7 tetromino secara random,
// taruh di baris paling atas tengah papan.
// ==========================================
void spawnBlokBaru(int bentukAktif[4][4], int &tipeAktif, int &blokX, int &blokY) {
    tipeAktif = rand() % JUMLAH_TIPE;  // 0-6 acak
    blokX     = LEBAR_PAPAN / 2 - 2;  // tengah papan
    blokY     = 0;                     // baris paling atas

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            bentukAktif[i][j] = semuaBentuk[tipeAktif][i][j];
}

// ==========================================
// [BAGIAN BINTANG] RENDER PAPAN + BLOK AKTIF
// Menggambar semua sel papan dan blok yang
// sedang jatuh ke layar konsol.
// ==========================================
void renderPapan(int bentukAktif[4][4], int tipeAktif, int blokX, int blokY) {
    // Buat salinan papan untuk ditambah blok aktif
    int tampilan[TINGGI_PAPAN][LEBAR_PAPAN];
    for (int i = 0; i < TINGGI_PAPAN; i++)
        for (int j = 0; j < LEBAR_PAPAN; j++)
            tampilan[i][j] = papan[i][j];

    // Tempelkan blok aktif ke tampilan
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (bentukAktif[i][j] != 0) {
                int ny = blokY + i;
                int nx = blokX + j;
                if (ny >= 0 && ny < TINGGI_PAPAN && nx >= 0 && nx < LEBAR_PAPAN)
                    tampilan[ny][nx] = tipeAktif + 1;
            }

    // Gambar sel per sel (tiap sel = 2 karakter "[]")
    for (int i = 0; i < TINGGI_PAPAN; i++) {
        setPosisiKursor(PAPAN_LAYAR_X + offsetX, PAPAN_LAYAR_Y + 1 + i);
        for (int j = 0; j < LEBAR_PAPAN; j++) {
            int val = tampilan[i][j];
            if (val == 0) {
                cout << "  ";  // kosong
            } else {
                cout << warnaBlok[val - 1] << "[]" << RESET;
            }
        }
    }
    cout << flush;
}

// ==========================================
// FUNGSI GAME OVER (dari kode asli)
// ==========================================
void gameOver() {
    gambarKotak(30 + offsetX, 2, 22, 22, "TETRIS");
    for (int i = 0; i < 5; i++) {
        setPosisiKursor(37 + offsetX, 12);
        cout << RED << "\033[1m" << "GAME OVER" << RESET;
        this_thread::sleep_for(chrono::milliseconds(400));
        setPosisiKursor(37 + offsetX, 12);
        cout << "         ";
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    setPosisiKursor(37 + offsetX, 12);
    cout << RED << "\033[1m" << "GAME OVER" << RESET;
    while (_kbhit()) { _getch(); }
    _getch();
}

// ==========================================
// [BAGIAN BINTANG] GAME LOOP UTAMA
// Inilah tempat gravitasi bekerja:
//   - Timer 500ms → blok turun otomatis
//   - Input user non-blocking (_kbhit)
//   - posisiValid() dicek sebelum semua gerak
// ==========================================
void gameLoopDenganGravitasi() {
    // Reset papan
    for (int i = 0; i < TINGGI_PAPAN; i++)
        for (int j = 0; j < LEBAR_PAPAN; j++)
            papan[i][j] = 0;

    srand(time(0)); // inisialisasi random SEKALI

    int bentukAktif[4][4];
    int tipeAktif, blokX, blokY;
    spawnBlokBaru(bentukAktif, tipeAktif, blokX, blokY); // spawn pertama

    // Timer gravitasi
    auto waktuTerakhir = chrono::steady_clock::now();
    int intervalMs = 500; // blok turun tiap 500ms

    bool gameOverFlag = false;

    while (!gameOverFlag) {

        // ── [GRAVITASI] Cek apakah sudah waktunya turun ──────
        auto sekarang = chrono::steady_clock::now();
        int selisihMs = (int)chrono::duration_cast<chrono::milliseconds>
                        (sekarang - waktuTerakhir).count();

        if (selisihMs >= intervalMs) {
            waktuTerakhir = sekarang;

            if (posisiValid(bentukAktif, blokX, blokY + 1)) {
                blokY++; // turun 1 baris
            } else {
                // Tidak bisa turun → kunci ke papan
                kunciBlok(bentukAktif, blokX, blokY, tipeAktif);

                hapusBarisPenuh();
                // Spawn blok baru
                spawnBlokBaru(bentukAktif, tipeAktif, blokX, blokY);

                // Kalau langsung nabrak = game over
                if (!posisiValid(bentukAktif, blokX, blokY)) {
                    gameOverFlag = true;
                    break;
                }
            }
        }

        // ── [INPUT] Non-blocking, tidak freeze game ───────────
        if (_kbhit()) {
            char k = _getch();

            if (k == 27) break; // ESC keluar

            if (k == 0 || k == (char)224) {
                k = _getch();

                if (k == 72) { // ↑ rotasi
                    int temp[4][4];
                    for (int i = 0; i < 4; i++)
                        for (int j = 0; j < 4; j++)
                            temp[j][3 - i] = bentukAktif[i][j];
                    if (posisiValid(temp, blokX, blokY))
                        for (int i = 0; i < 4; i++)
                            for (int j = 0; j < 4; j++)
                                bentukAktif[i][j] = temp[i][j];
                }
                if (k == 75 && posisiValid(bentukAktif, blokX - 1, blokY)) blokX--; // ←
                if (k == 77 && posisiValid(bentukAktif, blokX + 1, blokY)) blokX++; // →
                if (k == 80 && posisiValid(bentukAktif, blokX, blokY + 1)) blokY++; // ↓ soft drop
            }

            if (k == 32) { // SPACE = hard drop (langsung ke bawah)
                while (posisiValid(bentukAktif, blokX, blokY + 1))
                    blokY++;
                kunciBlok(bentukAktif, blokX, blokY, tipeAktif);
                hapusBarisPenuh();
                spawnBlokBaru(bentukAktif, tipeAktif, blokX, blokY);
                if (!posisiValid(bentukAktif, blokX, blokY)) {
                    gameOverFlag = true;
                }
            }
        }

        // ── [RENDER] Gambar papan + blok aktif ke layar ───────
        renderPapan(bentukAktif, tipeAktif, blokX, blokY);

        // Jeda kecil agar CPU tidak 100%
        this_thread::sleep_for(chrono::milliseconds(16));
    }

    gameOver();
}

// ==========================================
// FUNGSI LAYAR GAMEPLAY
// Menggambar HUD lalu langsung masuk game loop
// ==========================================
void layarGameplay() {
    clearScreen();
    hideKursor();

    // Panduan kontrol (kiri)
    setPosisiKursor(6 + offsetX, 3);  cout << "ROTATE      :" << BOLD << " [ \u2191 ] "   << RESET;
    setPosisiKursor(6 + offsetX, 5);  cout << "SLIDE LEFT  :" << BOLD << " [ \u2190 ] "   << RESET;
    setPosisiKursor(6 + offsetX, 7);  cout << "SLIDE RIGHT :" << BOLD << " [ \u2192 ] "   << RESET;
    setPosisiKursor(6 + offsetX, 9);  cout << "INSTANT DROP:" << BOLD << "[ SPACE ]"       << RESET;
    setPosisiKursor(6 + offsetX, 11); cout << "DROP        :" << BOLD << " [ \u2193 ] "   << RESET;

    // Kotak-kotak HUD (dari kode asli)
    gambarKotak(30 + offsetX, 2, 22, 22, "TETRIS");
    gambarKotak(54 + offsetX, 2, 14, 6,  "NEXT");
    gambarKotak(54 + offsetX, 9, 14, 6,  "HOLD");
    gambarKotak(54 + offsetX, 16, 14, 8, "STATS");

    setPosisiKursor(57 + offsetX, 18); cout << "SCORE: " << GREEN << "0"   << RESET;
    setPosisiKursor(57 + offsetX, 20); cout << "LEVEL: " << WHITE << "1"   << RESET;
    setPosisiKursor(57 + offsetX, 22); cout << "LINES: " << WHITE << "0"   << RESET;

    setPosisiKursor(25 + offsetX, 25);
    cout << "ESC = Kembali ke menu";

    // ── Masuk ke game loop gravitasi (BAGIAN BINTANG) ──────────
    gameLoopDenganGravitasi();

    showKursor();
}

// ==========================================
// FUNGSI MENU (dari kode asli, tidak diubah)
// ==========================================
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
    setPosisiKursor(15 + offsetX, 6);  cout << YELLOW << "TOP SCORES" << RESET;
    setPosisiKursor(15 + offsetX, 8);  cout << GREEN  << "1. 5,000"   << RESET;
    setPosisiKursor(15 + offsetX, 9);  cout << "2. 4,000";

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

// ==========================================
// FUNGSI TUTORIAL (dari kode asli, tidak diubah)
// ==========================================
bool sleepAtauInterupsi(int totalWaktuMs, int& halaman, bool& keluar) {
    int waktuCheck = 10;
    int akumulasi  = 0;
    while (akumulasi < totalWaktuMs) {
        this_thread::sleep_for(chrono::milliseconds(waktuCheck));
        akumulasi += waktuCheck;
        if (_kbhit()) {
            char tombol = _getch();
            if      (tombol == 'n' || tombol == 'N') { if (halaman < 4) halaman++; return true; }
            else if (tombol == 'b' || tombol == 'B') { if (halaman > 1) halaman--; return true; }
            else if (tombol == 'k' || tombol == 'K') { keluar = true; return true; }
        }
    }
    return false;
}

void tutorialGerak(int& halaman, bool& keluar) {
    hideKursor();
    int Kiri = 17, Kanan = 24;
    int posisi = Kanan;
    int arah = -1;
    setPosisiKursor(20 + offsetX, 5); cout << GREEN << "[1] MOVE            " << RESET;
    setPosisiKursor(20 + offsetX, 6); cout << "Use left and right navigation keyboard buttons to move ";
    setPosisiKursor(20 + offsetX, 7); cout << "block to left and right.                               ";
    while (true) {
        if (sleepAtauInterupsi(100, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
        setPosisiKursor(20 + offsetX, 18);
        if (arah == -1) cout << "BUTTON:" << GREEN << "   [ \u2190 ]" << RESET << "   [ \u2192 ]";
        else            cout << "BUTTON:   [ \u2190 ]" << GREEN << "   [ \u2192 ]" << RESET;
        setPosisiKursor(posisi + offsetX, 9);
        cout << PURPLE << "    [][] " << RESET;
        posisi += arah;
        if (posisi <= Kiri)  arah =  1;
        else if (posisi >= Kanan) arah = -1;
    }
}

void tutorialUbah(int& halaman, bool& keluar) {
    hideKursor();
    setPosisiKursor(20 + offsetX, 5); cout << GREEN << "[2] CHANGE DIRECTION" << RESET;
    setPosisiKursor(20 + offsetX, 6); cout << "Use the top navigation keyboard buttons to change         ";
    setPosisiKursor(20 + offsetX, 7); cout << "the block shape.                                          ";
    for (int rotasi = 1; rotasi <= 3; rotasi++) {
        if (rotasi == 1) {
            if (sleepAtauInterupsi(100, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            setPosisiKursor(20 + offsetX, 9);  cout << PURPLE << "          [][]                               " << RESET;
            setPosisiKursor(20 + offsetX, 10); cout << PURPLE << "          []                                 " << RESET;
        } else if (rotasi == 2) {
            if (sleepAtauInterupsi(300, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            setPosisiKursor(20 + offsetX, 9);  cout << PURPLE << "          []                                 " << RESET;
            setPosisiKursor(20 + offsetX, 10); cout << PURPLE << "          [][]                               " << RESET;
        } else if (rotasi == 3) {
            if (sleepAtauInterupsi(300, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
            setPosisiKursor(20 + offsetX, 9);  cout << PURPLE << "            []                               " << RESET;
            setPosisiKursor(20 + offsetX, 10); cout << PURPLE << "          [][]                               " << RESET;
        }
        setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:" << GREEN << "    [ \u2191 ]                         " << RESET;
        if (sleepAtauInterupsi(200, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
        setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:" << "    [ \u2191 ]                     " << RESET;
    }
}

void tutorialTurunCepat(int& halaman, bool& keluar) {
    hideKursor();
    setPosisiKursor(20 + offsetX, 5); cout << GREEN << "[3] DROP BLOCK" << RESET;
    setPosisiKursor(20 + offsetX, 6); cout << "Use [\u2193] for Soft Drop or [Space] for instant Hard Drop";
    int startY = 9, endY = 15;
    int posX = 25 + offsetX;
    for (int y = startY; y <= endY; y++) {
        if (y > startY) { setPosisiKursor(posX, y - 1); cout << "        "; }
        setPosisiKursor(posX, y); cout << PURPLE << "[][][][]" << RESET;
        setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:   " << GREEN << "[ \u2193 ]" << RESET << "   [ Space ]";
        if (sleepAtauInterupsi(100, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
    }
    setPosisiKursor(posX, endY); cout << "        ";
    if (sleepAtauInterupsi(200, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
    setPosisiKursor(posX, startY); cout << PURPLE << "[][][][]" << RESET;
    setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:   [ \u2193 ]   " << GREEN << "[ Space ]" << RESET;
    if (sleepAtauInterupsi(300, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
    setPosisiKursor(posX, startY); cout << "        ";
    setPosisiKursor(posX, endY);   cout << PURPLE << "[][][][]" << RESET;
    if (sleepAtauInterupsi(300, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
    setPosisiKursor(posX, endY); cout << "        ";
    setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:   [ \u2193 ]   [ Space ]        ";
}

void tutorialHold(int& halaman, bool& keluar) {
    hideKursor();
    setPosisiKursor(20 + offsetX, 5); cout << GREEN << "[4] HOLD BLOCK" << RESET;
    setPosisiKursor(20 + offsetX, 6); cout << "Press [ C ] to store the current block into the HOLD box";
    setPosisiKursor(22 + offsetX, 9);  cout << "+--- HOLD ---+";
    setPosisiKursor(22 + offsetX, 10); cout << "|            |";
    setPosisiKursor(22 + offsetX, 11); cout << "+------------+";
    int xDalam = 24 + offsetX, xLuar = 45 + offsetX;
    int yBlok = 10, yTombol = 18;
    setPosisiKursor(xDalam, yBlok); cout << "        ";
    setPosisiKursor(xLuar,  yBlok); cout << RED << "[][][][]" << RESET;
    setPosisiKursor(20 + offsetX, yTombol); cout << "BUTTON:   [ C ]";
    if (sleepAtauInterupsi(600, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
    setPosisiKursor(xLuar,  yBlok); cout << "        ";
    setPosisiKursor(xDalam, yBlok); cout << RED << "[][][][]" << RESET;
    setPosisiKursor(20 + offsetX, yTombol); cout << "BUTTON:   " << GREEN << "[ C ]" << RESET;
    if (sleepAtauInterupsi(600, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
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

// ==========================================
// 6. MAIN
// ==========================================
int main() {
    system("chcp 65001 > nul");
    cout << "\033[?25l";
    bool aplikasiBerjalan = true;

    while (aplikasiBerjalan) {
        int pilihanUser = menuUtama();

        if (pilihanUser == 0) {
            layarGameplay();         // <-- masuk gameplay + gravitasi
        } else if (pilihanUser == 1) {
            howToPlay();
        } else if (pilihanUser == 2) {
            clearScreen();
            setPosisiKursor(15 + offsetX, 2);
            cout << GREEN << "Terima kasih telah bermain Tetris!" << RESET << "\n";
            aplikasiBerjalan = false;
        }
    }

    cout << "\033[?25h";
    return 0;
}