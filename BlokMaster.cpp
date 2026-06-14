#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <chrono>
#include <thread>
#include <string>
using namespace std;

// =========================WARNA WARNI==============================
#define RESET   "\033[0m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RED     "\033[1;31m"
#define ORANGE  "\033[38;2;255;165;0m"
#define PURPLE  "\033[38;2;128;0;128m"
#define CYAN    "\033[1;36m"

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


int main(){
   
}