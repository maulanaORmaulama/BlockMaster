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
//INI BUAT NGEHAPUS ISI DALAM KOTAK TUTORIAL AGAR BISA DIGANTI DENGAN TUTORIAL BERIKUTNYA
void hapusDalamKotak(int startX, int startY, int lebar, int tinggi) {
    string spasiPembersih(lebar - 2, ' ');

    for (int i = 1; i < tinggi - 1; i++) {
        setPosisiKursor(startX + 1, startY + i);
        cout << spasiPembersih;
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
// ==========================================
// 2. MENU UTAMA    
// ==========================================
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
            return sel; 
        }
    }
}
void blokgerak(int &x, int &y, int key){
	if (key == 75) x -= 2; // panah kiri
	if (key == 77) x += 2; // panah kanan
	if (key == 80) y += 1; // panah bawah
}
void rotasiblok(int bentuk[4][4]){
	int temp[4][4];
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			temp[j][3 - i] = bentuk[i][j]; // rotasi 90 derajat
		}
	}
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			bentuk[i][j] = temp [i][j];
		}
	}
}
void spasi(int &y){
	y = 20; // 20 adalah angka batas bawah kotak
}
// ==========================================
// 3. LAYAR GAMEPLAY (HUD)
// ==========================================
void layarGameplay() {
    clearScreen();
    setPosisiKursor(6 + offsetX, 3);  
    cout << "ROTATE      :" << BOLD <<" [ \u2191 ] "   << RESET;
    setPosisiKursor(6 + offsetX, 5);  
    cout << "SLIDE LEFT  :" << BOLD <<" [ ← ] "   << RESET;
    setPosisiKursor(6 + offsetX, 7);  
    cout << "SLIDE RIGHT :" << BOLD <<" [ → ] "   << RESET;
    setPosisiKursor(6 + offsetX, 9);  
    cout << "INSTANT DROP:" << BOLD << "[ SPACE ]" << RESET;
    setPosisiKursor(6 + offsetX, 11);  
    cout << "DROP        :" << BOLD <<" [ \u2193 ] " << RESET;
    setPosisiKursor(6 + offsetX, 13);  
    cout << "HOLD        :" << BOLD <<" [ C ] "   << RESET;
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
     cout << "Tekan tombol ESC untuk kembali ke Main Menu...";
    _getch();

    int blokX = 35;
    int blokY = 3;
    int bentuk[4][4];
    while(true) {
        if(_kbhit()){
            char key = _getch();
            if(key == 27) break; // Tekan ESC untuk keluar ke menu
            
            if(key == 0 || key == (char)224){
                key = _getch();
                if(key == 72) rotasiblok(bentuk);
                else blokgerak(blokX, blokY, key);
            } else if(key == 32){
                spasi(blokY);
            }
        }
    }
}
//===========================================FUNGSI BUAT tutorial=====================================
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
// ==========================================
// 4. TUTORIAL PENGGUNAAN KONTROL
// ==========================================
void tutorialGerak(int& halaman, bool& keluar) {
    hideKursor();
    int Kiri = 17, Kanan = 24;
    int posisi = Kanan;
    int arah = -1; // -1 = kiri, 1 = kanan

    setPosisiKursor(20 +offsetX, 5); cout << GREEN << "[1] MOVE            " << RESET;
    setPosisiKursor(20 +offsetX, 6); cout << "Use left and right navigation keyboard buttons to move " ;
    setPosisiKursor(20 +offsetX, 7); cout << "block to left and right.                               " ;

    while (true) {
    if (sleepAtauInterupsi(100, halaman, keluar)) {
        hapusDalamKotak(16 + offsetX, 3, 70, 17);
        return;
    }

    setPosisiKursor(20 + offsetX, 18);
    if (arah == -1)
        cout << "BUTTON:" << GREEN << "   [ ← ]" << RESET << "   [ → ]";
    else
        cout << "BUTTON:   [ ← ]" << GREEN << "   [ → ]" << RESET;

    setPosisiKursor(posisi + offsetX, 9);
    cout << PURPLE << "    [][] " << RESET;

    posisi += arah;

    if (posisi <= Kiri)
        arah = 1;
    else if (posisi >= Kanan)
        arah = -1;
}
}

void tutorialUbah(int& halaman, bool& keluar){
    hideKursor();

    setPosisiKursor(20 +offsetX, 5); cout << GREEN << "[2] CHANGE DIRECTION" << RESET;
    setPosisiKursor(20 +offsetX, 6); cout << "Use the top navigation keyboard buttons to change         " ;
    setPosisiKursor(20 +offsetX, 7); cout << "the block shape.                                          " ;

    for (int rotasi = 1; rotasi <= 3; rotasi++){
        if(rotasi == 1){
            if (sleepAtauInterupsi(100, halaman, keluar)) {
                hapusDalamKotak(16 + offsetX, 3, 70, 17);
                return;}   
            setPosisiKursor(20 + offsetX, 9 );  cout << PURPLE << "          [][]                               " << RESET;
            setPosisiKursor(20 + offsetX, 10 ); cout << PURPLE << "          []                                 " << RESET;
            
            }
          
        else if (rotasi == 2){
            if (sleepAtauInterupsi(300, halaman, keluar)) {
                hapusDalamKotak(16 + offsetX, 3, 70, 17);
                return;
            }
            setPosisiKursor(20 + offsetX, 9 ); cout << PURPLE << "          []                                 " << RESET;
            setPosisiKursor(20 + offsetX, 10 );cout << PURPLE << "          [][]                               " << RESET;
            
        }  
                
        else if (rotasi == 3){
            if (sleepAtauInterupsi(300, halaman, keluar)) {
                hapusDalamKotak(16 + offsetX, 3, 70, 17);
                return;
            } 
            setPosisiKursor(20 + offsetX, 9 ); cout << PURPLE << "            []                               " << RESET;
            setPosisiKursor(20 + offsetX, 10 );cout << PURPLE << "          [][]                               " << RESET;
            
        }
        setPosisiKursor(20 + offsetX, 18);  cout << "BUTTON:" << GREEN << "    [ \u2191 ]                         " << RESET;
        if (sleepAtauInterupsi(200, halaman, keluar)){
            hapusDalamKotak(16 + offsetX, 3, 70, 17);
            return;
        }
        setPosisiKursor(20 + offsetX, 18); cout << "BUTTON:" << "    [ \u2191 ]                     " << RESET;
    }
        
    
}

void tutorialTurunCepat(int& halaman, bool& keluar) {
    hideKursor();
    setPosisiKursor(20 + offsetX, 5); 
        cout << GREEN << "[3] DROP BLOCK" << RESET;
    setPosisiKursor(20 + offsetX, 6); 
        cout << "Use [\u2193] for Soft Drop or [Space] for instant Hard Drop";
    
    int startY = 9;
    int endY = 15;
    int posX = 25 + offsetX; 
 
    // ==========================================
    // SOFT DROP (Turun Perlahan)
    // ==========================================
    for (int y = startY; y <= endY; y++) {
        if (y > startY) {
            setPosisiKursor(posX, y - 1);
            cout << "        ";
        }
        setPosisiKursor(posX, y);
            cout << PURPLE << "[][][][]" << RESET;
        setPosisiKursor(20 + offsetX, 18);
            cout << "BUTTON:   " << GREEN << "[ \u2193 ]" << RESET << "   [ Space ]";
        if (sleepAtauInterupsi(100, halaman, keluar)) {
            hapusDalamKotak(16 + offsetX, 3, 70, 17);
            return;
        }
    }
    setPosisiKursor(posX, endY); cout << "        ";
    if (sleepAtauInterupsi(200, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
    // ==========================================
    //        HARD DROP (Turun Instan)
    // ==========================================
    
    setPosisiKursor(posX, startY); 
        cout << PURPLE << "[][][][]" << RESET;
    setPosisiKursor(20 + offsetX, 18);
        cout << "BUTTON:   [ \u2193 ]   " << GREEN << "[ Space ]" << RESET;
    if (sleepAtauInterupsi(300, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
    setPosisiKursor(posX, startY); 
        cout << "        "; 
    setPosisiKursor(posX, endY); 
        cout << PURPLE << "[][][][]" << RESET;
    if (sleepAtauInterupsi(300, halaman, keluar)) { hapusDalamKotak(16 + offsetX, 3, 70, 17); return; }
    setPosisiKursor(posX, endY); 
        cout << "        ";
    setPosisiKursor(20 + offsetX, 18);
        cout << "BUTTON:   [ \u2193 ]   [ Space ]        ";
}
void tutorialHold(int& halaman, bool& keluar) {
    hideKursor();
    
    setPosisiKursor(20 + offsetX, 5); cout << GREEN << "[4] HOLD BLOCK" << RESET;
    setPosisiKursor(20 + offsetX, 6); cout << "Press [ C ] to store the current block into the HOLD box";

    setPosisiKursor(22 + offsetX, 9);  cout << "+--- HOLD ---+";
    setPosisiKursor(22 + offsetX, 10); cout << "|            |";
    setPosisiKursor(22 + offsetX, 11); cout << "+------------+";

    int xDalam = 24 + offsetX;
    int xLuar = 45 + offsetX;   
    int yBlok = 10;             
    int yTombol = 18;           

    
    setPosisiKursor(xDalam, yBlok); cout << "        "; 

    setPosisiKursor(xLuar, yBlok);  cout << RED << "[][][][]" << RESET; 
    
    setPosisiKursor(20 + offsetX, yTombol); 
    cout << "BUTTON:   [ C ]";
    if (sleepAtauInterupsi(600, halaman, keluar)) {
        hapusDalamKotak(16 + offsetX, 3, 70, 17);
        return;
    }
    
    // Hapus balok dari luar
    setPosisiKursor(xLuar, yBlok);  cout << "        "; 
    
    // Munculkan balok di dalam kotak HOLD
    setPosisiKursor(xDalam, yBlok); cout << RED << "[][][][]" << RESET; 

    setPosisiKursor(20 + offsetX, yTombol); 
    cout << "BUTTON:   " << GREEN << "[ C ]" << RESET;
    if (sleepAtauInterupsi(600, halaman, keluar)) {
        hapusDalamKotak(16 + offsetX, 3, 70, 17);
        return;
    }
    
}
// ==========================================
// 5. MENU HOW TO PLAY
// ==========================================
void howToPlay() {
    clearScreen();

    int halaman = 1;
    bool keluar = false;

    gambarKotak(16 + offsetX, 3, 70, 17, "HOW TO PLAY");
    setPosisiKursor(26 + offsetX, 20); cout << "[N] Next Page  |  [B] Back Page  |  [K] Menu Exit           ";

    while (!keluar) {
        if (halaman == 1) {
            tutorialGerak(halaman, keluar);
        } 
        else if (halaman == 2) {
            tutorialUbah(halaman, keluar);
        } 
        else if (halaman == 3) {
            tutorialTurunCepat(halaman, keluar);
        } 
        else if (halaman == 4) {
            tutorialHold(halaman, keluar);
        }
    }
    clearScreen();
    showKursor();
}
// ==========================================
// 6. SCREEN ROUTER (MAIN LOOP)
// ==========================================
int main(){
    system("chcp 65001 > nul");
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
