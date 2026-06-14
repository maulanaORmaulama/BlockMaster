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

void setKursorHome() {
    cout << "\033[H" << flush;
}
void hideKursor(){
    cout << "\033[?25l" << flush;
}
void showKursor(){
    cout << "\033[?25h" << flush;
}
void clearScreen(){
    cout << "\033[2J\033[3J\033[1;1H";
}
//==========================================FUNGSI UNTUK MENENTUKAN LOKASI KURSOR SAAT MEMBANGUN KOTAK ============
void setPosisiKursor(int x, int y) {
     cout << "\033[" << y << ";" << x << "H";
}
//==========================================FUNGSI GAMBAR KOTAK UI================================================
void gambarKotak(int startX, int startY, int width, int height,  string title = "") {
    setPosisiKursor(startX, startY);
     cout << CYAN << "+" <<  string(width - 2, '-') << "+" << RESET;
    
    for (int i = 1; i < height - 1; i++) {
        setPosisiKursor(startX, startY + i);
         cout << CYAN << "|" <<  string(width - 2, ' ') << "|" << RESET;
    }
    
    setPosisiKursor(startX, startY + height - 1);
     cout << CYAN << "+" <<  string(width - 2, '-') << "+" << RESET;

   if (title != "") {
        //==== 1. Hitung panjang teks ditambah 2 spasi bantalan di kiri-kanannya
        int panjangTeks = title.length() + 2; 
        
        //==== 2. Hitung jarak dari tembok kiri agar posisinya pas di tengah
        int offsetTengah = (width - panjangTeks) / 2;
        
        //==== 3. Pindahkan kursor ke posisi tengah atap, lalu cetak
        setPosisiKursor(startX + offsetTengah, startY);
         cout << YELLOW << " " << title << " " << RESET;
    }
}

//================================FUNGSI GAME OVER=======================================================
void gameOver() {
    gambarKotak(30 + offsetX, 2, 22, 22, "TETRIS"); 
    
    for (int i = 0; i < 5; i++) {
        
        setPosisiKursor(37 + offsetX, 12); 
         cout << RED << "\033[1m" << "GAME OVER" << RESET; 
         this_thread::sleep_for( chrono::milliseconds(400)); 
        
        setPosisiKursor(37+ offsetX, 12); 
         cout << "         "; 
        
         this_thread::sleep_for( chrono::milliseconds(100)); 
    }
    setPosisiKursor(37 + offsetX, 12); 
     cout << RED << "\033[1m" << "GAME OVER" << RESET;
    
    // Bersihkan buffer input agar ketikan user saat animasi berjalan tidak bocor
    while (_kbhit()) { _getch(); } 
    
    _getch(); // Tunggu input final dari user
}
//=======================FUNGSI MENU UTAMA=====================================
void gambarTampilanUtama() {
    clearScreen();

    gambarKotak(54 + offsetX, 6, 16, 10);
        setPosisiKursor(58 + offsetX, 6);  cout << YELLOW << "CREDITS" << RESET;
        setPosisiKursor(58 + offsetX, 8);  cout << "-ISNA";
        setPosisiKursor(58 + offsetX, 9);  cout << "-BINTANG";
        setPosisiKursor(58 + offsetX, 10);  cout << "-EZRA";
        setPosisiKursor(58 + offsetX, 11);  cout << "-NISA";
        setPosisiKursor(58 + offsetX, 12);  cout << "-SARAH";
        setPosisiKursor(58 + offsetX, 13);  cout << "-HABIB";
        setPosisiKursor(58 + offsetX, 14);  cout << "-AURA";

    gambarKotak(30 + offsetX, 3, 22, 17, ""); 
        setPosisiKursor(38 + offsetX, 5);  cout << RED << "T" << GREEN << "E" << YELLOW << "T" << BLUE << "R" << MAGENTA << "I" << CYAN << "S" << RESET;
        setPosisiKursor(31 + offsetX, 6);  cout << "--------------------";
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
            if (i == sel) {
                 cout << GREEN << "\033[1m" << "> " << opsi[i] << RESET;
            } else {
                 cout << WHITE << "  " << opsi[i] << "  " << RESET;
            }
        }
        setPosisiKursor(1, 25); 

        key = _getch();
        if (key == 0 || key == (char)224) { 
            key = _getch();
            if (key == 72) { sel--; if (sel < 0) sel = 2; } 
            if (key == 80) { sel++; if (sel > 2) sel = 0; } 
        } else if (key == 13) { 
            return sel; // 0 = Play, 1 = Score, 2 = Quit
        }
    }
}
//===========================FUNGSI MENU GAMEPLAY=========================================
void layarGameplay() {
    clearScreen();
        setPosisiKursor(9 + offsetX, 3);  cout  << "SPIN       :" << BOLD <<" ^"   << RESET;
        setPosisiKursor(9 + offsetX, 5);  cout  << "SLIDE LEFT :" << BOLD <<" <"   << RESET;
        setPosisiKursor(9 + offsetX, 7);  cout  << "SLIDE RIGHT:" << BOLD <<" >"   << RESET;
        setPosisiKursor(9 + offsetX, 9);  cout  << "HOLD       :" << BOLD <<" C"   << RESET;
        setPosisiKursor(9 + offsetX, 11);  cout << "DROP       :" << BOLD <<" SPACE" << RESET;
    gambarKotak(30 + offsetX, 2, 22, 22, "TETRIS");       
    gambarKotak(54 + offsetX, 2, 14, 6, "NEXT");         
    gambarKotak(54 + offsetX, 9, 14, 6, "HOLD");         
    gambarKotak(54 + offsetX, 16, 14, 8, "STATS");       
    
    setPosisiKursor(57 + offsetX, 18);  cout << "SCORE: " << GREEN << "0" << RESET;
    setPosisiKursor(57 + offsetX, 20);  cout << "LEVEL: " << WHITE << "1" << RESET;
    setPosisiKursor(57 + offsetX, 22);  cout << "LINES: " << WHITE << "0" << RESET;
    
    // Pesan sementara karena logika game belum dipasang
    setPosisiKursor(31 + offsetX, 12);  cout << RED << "[][][][][][][][][][]" << RESET;
    setPosisiKursor(35 + offsetX, 13);  cout << RED << "KOTAK GAMPELAY" << RESET;
    
    setPosisiKursor(25 + offsetX, 26);
     cout << "Tekan 'f' untuk simulasi Game Over...                  ";
    
    char key = _getch();

    if (key == 102 || key == 'f') { 
        gameOver();
    }
    setPosisiKursor(25 + offsetX, 26);
     cout << "Tekan tombol APAPUN untuk kembali ke Main Menu...";
    _getch();

}
//===========================================FUNGSI BUAT TOTURIAL=====================================
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
void toturialGerak(int& halaman, bool& keluar) {
    clearScreen();
    hideKursor();
    const int lebarDalam = 59;
    
    while (true) {
        for (int spasi = 20; spasi >= 4; spasi -= 2) {
            setKursorHome();
            cout << "+===========================================================+" << endl;
            cout << "|                       HOW TO PLAY                         |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "|" << GREEN << " [1] MOVE                                                  " << RESET << "|" << endl;
            cout << "| Use left and right navigation keyboard buttons to move    |" << endl;
            cout << "| block to left and right.                                  |" << endl;
            cout << "|                                                           |" << endl;
            cout << "|";
            for (int i = 0; i < spasi + 2; i++) cout << " ";
            cout << PURPLE << "[][]" << RESET;
            int sisaSpasiAtas = lebarDalam - (spasi + 2) - 4;
            for (int i = 0; i < sisaSpasiAtas; i++) cout << " ";
            cout << "|" << endl;
            cout << "|";
            for (int i = 0; i < spasi; i++) cout << " ";
            cout << PURPLE << "[][]" << RESET;
            int sisaSpasiBawah = lebarDalam - spasi - 4;
            for (int i = 0; i < sisaSpasiBawah; i++) cout << " ";
            cout << "|" << endl;
            cout << "|";
            for (int i = 0; i < lebarDalam; i++) cout << " ";
            cout << "|" << endl;
            cout << "|                                                           |" << endl;
            cout << "|                                                           |" << endl;
            cout << "| BUTTON:   " << GREEN << "[ \u2190 ]" << RESET << "   [ \u2192 ]                                   |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "[N] Next Page  |  [B] Back Page  |  [K] Menu Exit           " << endl;
            
            if (sleepAtauInterupsi(100, halaman, keluar)) return; 
        }

        if (sleepAtauInterupsi(300, halaman, keluar)) return;
        
        for (int spasi = 4; spasi <= 20; spasi += 2) {
            setKursorHome();
            cout << "+===========================================================+" << endl;
            cout << "|                       HOW TO PLAY                         |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "|" << GREEN << " [1] MOVE                                                  " << RESET << "|" << endl;
            cout << "| Use left and right navigation keyboard buttons to move    |" << endl;
            cout << "| block to left and right.                                  |" << endl;
            cout << "|                                                           |" << endl;
            cout << "|";
            for (int i = 0; i < spasi + 2; i++) cout << " ";
            cout << PURPLE << "[][]" << RESET;
            int sisaSpasiAtas = lebarDalam - (spasi + 2) - 4;
            for (int i = 0; i < sisaSpasiAtas; i++) cout << " ";
            cout << "|" << endl;
            cout << "|";
            for (int i = 0; i < spasi; i++) cout << " ";
            cout << PURPLE << "[][]" << RESET;
            int sisaSpasiBawah = lebarDalam - spasi - 4;
            for (int i = 0; i < sisaSpasiBawah; i++) cout << " ";
            cout << "|" << endl;
            cout << "|";
            for (int i = 0; i < lebarDalam; i++) cout << " ";
            cout << "|" << endl;
            cout << "|                                                           |" << endl;
            cout << "|                                                           |" << endl;
            cout << "| BUTTON:   [ \u2190 ]   " << GREEN << "[ \u2192 ]" << RESET << "                                   |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "[N] Next Page  |  [B] Back Page  |  [K] Menu Exit           " << endl;
            
            if (sleepAtauInterupsi(100, halaman, keluar)) return;
        }      
        if (sleepAtauInterupsi(300, halaman, keluar)) return; 
    }
}

void toturialUbah(int& halaman, bool& keluar){
    clearScreen();
    hideKursor();
    const int spasi = 25;
    const int lebarDalam = 59;

    while (true) {
        for(int rotasi = 0; rotasi < 4; rotasi++){
            setKursorHome();
            cout << "+===========================================================+" << endl;
            cout << "|                       HOW TO PLAY                         |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "|" << GREEN << " [2] CHANGE DIRECTION                                      " << RESET << "|" << endl;
            cout << "| Use the top navigation keyboard buttons to change         |" << endl;
            cout << "| the block shape.                                          |" << endl;
            cout << "|                                                           |" << endl;
            cout << "|";
            if(rotasi == 0 || rotasi == 1 || rotasi == 3){
                for(int i = 0; i < spasi + 2; i++) cout << " ";
                cout << PURPLE << "[]" << RESET;
                int sisa = lebarDalam - (spasi + 2) - 2;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            else if (rotasi == 2){
                for(int i = 0; i < spasi; i++) cout << " ";
                cout << PURPLE << "[][][]" << RESET;
                int sisa = lebarDalam - spasi - 6;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            cout << "|" << endl; 
            cout << "|";
            if(rotasi == 0){
                for(int i = 0; i < spasi; i++) cout << " ";
                cout << PURPLE << "[][][]" << RESET;
                int sisa = lebarDalam - spasi - 6;
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
                int sisa = lebarDalam - (spasi + 2) - 2;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            else if(rotasi == 3){
                for(int i = 0; i < spasi; i++) cout << " ";
                cout << PURPLE << "[][]" << RESET;
                int sisa = lebarDalam - spasi - 4;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            cout << "|" << endl;    
            cout << "|";
            if(rotasi == 1 || rotasi == 3){
                for(int i = 0; i < spasi + 2; i++) cout << " ";
                cout << PURPLE << "[]" << RESET;
                int sisa = lebarDalam - (spasi + 2) - 2;
                for(int i = 0; i < sisa; i++) cout << " ";
            }
            else {
                for(int i = 0; i < lebarDalam; i++) cout << " ";
            }
            cout << "|" << endl;
            cout << "|                                                           |" << endl;
            cout << "| BUTTON:   " << GREEN << "[\u2191]" << RESET << "                                             |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "[N] Next Page  |  [B] Back Page  |  [K] Menu Exit           " << endl;

            if (sleepAtauInterupsi(400, halaman, keluar)) return;
            
            cout << "\033[3A" << flush;
            cout << "| BUTTON:   [\u2191]                                             |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "\033[B" << flush;
            
            if (sleepAtauInterupsi(150, halaman, keluar)) return;
        }
    }
}

void toturialTurunCepat(int& halaman, bool& keluar){
    clearScreen();
    hideKursor();
    const int spasi = 23;
    const int lebarDalam = 59;

    while (true) {
        for(int tinggi = 0; tinggi <= 2; tinggi++){
            setKursorHome();
            cout << "+===========================================================+" << endl;
            cout << "|                       HOW TO PLAY                         |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "|" << GREEN << " [3] DROP BLOCK                                            " << RESET << "|" << endl;
            cout << "| Use [↓] for Soft Drop or [Space] for instant Hard Drop    |" << endl;
            cout << "|                                                           |" << endl;
            for(int b = 0; b < 3; b++){
                cout << "|";
                if(b == tinggi){
                    for(int i = 0; i < spasi; i++) cout << " ";
                    cout << RED << "[][][][]" << RESET;
                    int sisa = lebarDalam - spasi - 8;
                    for(int i = 0; i < sisa; i++) cout << " "; 
                }
                else{
                    for(int i = 0; i < lebarDalam; i++) cout << " ";
                }
                cout << "|" << endl;
            }
            cout << "|                                                           |" << endl;
            cout << "| BUTTON:   " << GREEN << "[ \u2193 ]" << RESET << "   [ Space ]                               |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "[N] Next Page  |  [B] Back Page  |  [K] Menu Exit           " << endl;

            if (sleepAtauInterupsi(250, halaman, keluar)) return;

            cout << "\033[3A\r| BUTTON:   [ \u2193 ]   [ Space ]                               |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "\033[B" << flush;

            if (sleepAtauInterupsi(100, halaman, keluar)) return;
        }

        if (sleepAtauInterupsi(400, halaman, keluar)) return;

        for(int f = 0; f < 2; f++){
            setKursorHome();
            cout << "+===========================================================+" << endl;
            cout << "|                       HOW TO PLAY                         |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "|" << GREEN << " [3] DROP BLOCK                                            " << RESET << "|" << endl;
            cout << "| Use [↓] for Soft Drop or [Space] for instant Hard Drop    |" << endl;
            cout << "|                                                           |" << endl;
            int tinggiHardDrop = (f == 0) ? 0 : 2;
            for(int b = 0; b < 3; b++){
                cout << "|";
                if(b == tinggiHardDrop){
                    for(int i = 0; i < spasi; i++) cout << " ";
                    cout << RED << "[][][][]" << RESET;
                    int sisa = lebarDalam - spasi - 8;
                    for(int i = 0; i < sisa; i++) cout << " ";
                }
                else{
                    for(int i = 0; i < lebarDalam; i++) cout << " ";
                }
                cout << "|" << endl;
            }
            cout << "|                                                           |" << endl;
            if (f == 0) {
                cout << "| BUTTON:   [ \u2193 ]   " << GREEN << "[ Space ]" << RESET << "                               |" << endl;
            } else {
                cout << "| BUTTON:   [ \u2193 ]   [ Space ]                               |" << endl;
            }
            cout << "+===========================================================+" << endl;
            cout << "[N] Next Page  |  [B] Back Page  |  [K] Menu Exit           " << endl;
            if(f == 0){
                if (sleepAtauInterupsi(150, halaman, keluar)) return;
            }
            else{
                if (sleepAtauInterupsi(600, halaman, keluar)) return;
            }
        }
    }
}
void toturialHold(int& halaman, bool& keluar){
    const int lebarDalam = 59;

    while (true) {
        for(int frame = 0; frame < 2; frame++){
            clearScreen();
            hideKursor();
            cout << "+===========================================================+" << endl;
            cout << "|                       HOW TO PLAY                         |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "|" << GREEN << " [4] HOLD BLOCK                                            " << RESET << "|" << endl;
            cout << "| Press [ C ] to store the current block into the HOLD box  |" << endl;
            cout << "|                                                           |" << endl;
            cout << "|   +--- " << YELLOW << "HOLD" << RESET << " ---+                                          |" << endl;
            cout << "|   |            | ";
            for(int i = 0; i < 15; i++) cout << " ";
            cout << RED << "[][][][]" << RESET;
            int sisa = lebarDalam - 18 - 15 - 8;
            for(int i = 0; i < sisa; i++) cout << " ";
            cout << "|" << endl;
            cout << "|   +------------+                                          |" << endl;
            cout << "|                                                           |" << endl;
            if(frame == 0) {
                cout << "| BUTTON:   " << GREEN << "[ C ]" << RESET << "                                           |" << endl;
            } else {
                cout << "| BUTTON:   [ C ]                                           |" << endl;
            }
            cout << "+===========================================================+" << endl;
            cout << "[N] Next Page  |  [B] Back Page  |  [K] Menu Exit           " << endl;

            if(frame == 0) {
                if (sleepAtauInterupsi(400, halaman, keluar)) return;
            } else {
                if (sleepAtauInterupsi(150, halaman, keluar)) return;
            }
        }
        for(int frame = 0; frame < 2; frame++){
            clearScreen();
            hideKursor();
            cout << "+===========================================================+" << endl;
            cout << "|                       HOW TO PLAY                         |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "|" << GREEN << " [4] HOLD BLOCK                                            " << RESET << "|" << endl;
            cout << "| Press [ C ] to store the current block into the HOLD box  |" << endl;
            cout << "|                                                           |" << endl;
            cout << "|   +--- " << YELLOW << "HOLD" << RESET << " ---+                                          |" << endl;
            cout << "|   |  " << RED << "[][][][]" << RESET << "  |                                          |" << endl;
            cout << "|   +------------+                                          |" << endl;
            cout << "|                                                           |" << endl;
            cout << "| BUTTON:   [ C ]                                           |" << endl;
            cout << "+===========================================================+" << endl;
            cout << "[N] Next Page  |  [B] Back Page  |  [K] Menu Exit           " << endl;
            if(frame == 0) {
                if (sleepAtauInterupsi(600, halaman, keluar)) return;
            } else {
                if (sleepAtauInterupsi(200, halaman, keluar)) return;
            }
        }
    }
}
//=================MENU HOW TO PLAY==================
void howToPlay() {
    system("chcp 65001 > nul");
    int halaman = 1;
    bool keluar = false;
    
    while (!keluar) {
        if (halaman == 1) {
            toturialGerak(halaman, keluar);
        } 
        else if (halaman == 2) {
            toturialUbah(halaman, keluar);
        } 
        else if (halaman == 3) {
            toturialTurunCepat(halaman, keluar);
        } 
        else if (halaman == 4) {
            toturialHold(halaman, keluar);
        }
    }
    clearScreen();
    showKursor();
}

//==================PENGATUR STATE DAN ROUTE===============
int main(){
     cout << "\033[?25l";
    bool aplikasiBerjalan = true;
    char key;
    while (aplikasiBerjalan) {
        
        int pilihanUser = menuUtama(); 

        if (pilihanUser == 0) {
            layarGameplay(); 
      
        } else if (pilihanUser == 1) {
            howToPlay();
            
        } else if (pilihanUser == 2) {
            clearScreen();
            setPosisiKursor(15 + offsetX, 2);
             cout << GREEN << "Terima kasih telah bermain Tetris!" << RESET << "\n";
            aplikasiBerjalan = false; //
        }
    }
    cout << "\033[?25h";
    return 0;
}