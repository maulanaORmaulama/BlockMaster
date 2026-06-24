#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

// Konstanta
const int WIDTH = 12;
const int HEIGHT = 22;
int board[HEIGHT][WIDTH] = {0};

struct Block { int x, y; };

// Menggunakan ANSI Escape Codes untuk posisi kursor
void setCursorPosition(int x, int y) {
    cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
}

// Menggunakan ANSI Escape Codes untuk membersihkan layar
void clearScreen() {
    cout << "\033[2J";
}

// Fungsi draw sederhana
void draw(Block* b) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            setCursorPosition(j, i);
            if (i == b->y && j == b->x) cout << "[]";
            else if (board[i][j] == 1) cout << "##";
            else cout << (j == 0 || j == WIDTH - 1 || i == HEIGHT - 1 ? "||" : "  ");
        }
    }
}

void playClearAnimation(int row) {
    for (int blink = 0; blink < 3; blink++) {
        setCursorPosition(1, row);
        for (int j = 1; j < WIDTH - 1; j++) cout << "  ";
        this_thread::sleep_for(chrono::milliseconds(100));
        setCursorPosition(1, row);
        for (int j = 1; j < WIDTH - 1; j++) cout << "[]";
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void clearFullLines() {
    for (int i = HEIGHT - 2; i > 0; i--) {
        bool isFull = true;
        for (int j = 1; j < WIDTH - 1; j++) {
            if (board[i][j] == 0) isFull = false;
        }
        if (isFull) {
            playClearAnimation(i);
            for (int k = i; k > 1; k--) {
                for (int j = 1; j < WIDTH - 1; j++) board[k][j] = board[k - 1][j];
            }
            i++; 
        }
    }
}

int main() {
    Block player = {WIDTH / 2, 1};
    bool running = true;

    clearScreen();
    // Sembunyikan kursor (ANSI)
    cout << "\033[?25l";

    while (running) {
        draw(&player);

        // Catatan: Tanpa Windows.h, pendeteksian tombol real-time 
        // membutuhkan library tambahan seperti ncurses (Linux) 
        // atau termios. Untuk keperluan demo logika, kita gunakan loop sederhana:
        
        if (player.y < HEIGHT - 2 && board[player.y + 1][player.x] == 0) {
            player.y++;
        } else {
            board[player.y][player.x] = 1;
            clearFullLines();
            player = {WIDTH / 2, 1};
            if (board[1][WIDTH / 2] == 1) running = false;
        }

        this_thread::sleep_for(chrono::milliseconds(150));
    }

    // Kembalikan kursor (ANSI)
    cout << "\033[?25h";
    cout << "Game Over!" << endl;
    return 0;
}