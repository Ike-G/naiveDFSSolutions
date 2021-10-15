#include <iostream>
#include <string>

bool possible(int board[9][9], int x, int y, int n) {
    int xo, yo;
    div_t io;
    xo = 3*div(x,3).quot;
    yo = 3*div(y,3).quot;
    for (int i = 0; i < 9; i++) {
        io = div(i,3);
        if (board[i][y] == n || board[x][i] == n || board[xo+io.quot][yo+io.rem] == n)
            return false;
    }
    return true;
}

void solve(int (&board)[9][9]) {
    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            if (board[x][y] == 0) {
                for (int n = 1; n <= 9; n++) {
                    if (possible(board, x, y, n)) {
                        board[x][y] = n;
                        solve(board);
                        board[x][y] = 0;
                    }
                }
                return;
            }
        }
    }

    for (int y = 8; y >= 0; y--) {
        std::cout << "[ ";
        for (int x = 0; x < 9; x++) {
            if (x % 3 == 0 && x != 0)
                std::cout << "| ";
            std::cout << board[x][y] << " ";
        }
        std::cout << "]" << std::endl;
        if (y % 3 == 0 && y != 0)
            std::cout << "[ --------------------- ]" << std::endl;
        
    }
}

int main() {
    int board[9][9];
    std::string s;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << "Input value at (" << i << ", " << j << ") [0 if empty]: ";
            std::cin >> s;
            board[i][j] = stoi(s);
        }
    }
    solve(board);
    return 0;
}