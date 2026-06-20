#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h> 

using namespace std;

const int W = 12, H = 22;
int b[H][W] = {0};

struct Pos { int x, y; };

void moveCursor(int x, int y) {
    cout << "\033[" << y + 1 << ";" << (x * 2) + 1 << "H";
}

void playClearAnimation(int row) {
    for (int blink = 0; blink < 3; blink++) {
        moveCursor(1, row);
        for (int j = 1; j < W - 1; j++) cout << "  ";
        this_thread::sleep_for(chrono::milliseconds(100));
        
        moveCursor(1, row);
        for (int j = 1; j < W - 1; j++) cout << "[]";
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void clearFullLines() {
    for (int i = H - 2; i > 0; i--) {
        bool full = true;
        for (int j = 1; j < W - 1; j++) {
            if (b[i][j] == 0) full = false;
        }

        if (full) {
            playClearAnimation(i);
            for (int k = i; k > 1; k--) {
                for (int j = 1; j < W - 1; j++) {
                    b[k][j] = b[k - 1][j];
                }
            }
            cout << "\033[2J"; // 
            i++; 
        }
    }
}

void draw(Pos p) {
    moveCursor(0, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (j == 0 || j == W - 1 || i == H - 1) cout << "[]";
            else if (i == p.y && j == p.x) cout << "[]";
            else if (b[i][j]) cout << "[]";
            else cout << "  ";
        }
        cout << endl;
    }
}

int main() {
    Pos p = {W / 2, 1};
    cout << "\033[?25l\033[2J"; // 

    while (true) {
        draw(p);

        // Input Keyboard
        if (_kbhit()) {
            char key = _getch();
            if (key == 'a' && p.x > 1 && !b[p.y][p.x - 1]) p.x--;        // Kiri
            if (key == 'd' && p.x < W - 2 && !b[p.y][p.x + 1]) p.x++;    // Kanan
            if (key == 's' && p.y < H - 2 && !b[p.y + 1][p.x]) p.y++;    // Bawah
        }

        // Logika Jatuh 
        if (p.y < H - 2 && !b[p.y + 1][p.x]) {
            p.y++;
        } else {
            b[p.y][p.x] = 1; // Kunci posisi
            clearFullLines(); // Cek baris
            p = {W / 2, 1};   // Reset posisi ke atas
            if (b[1][W / 2]) break; // Game Over
        }
        
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    
    cout << "\033[" << H + 1 << ";0HGAME OVER!" << endl;
    return 0;
}