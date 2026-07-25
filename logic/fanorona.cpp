#include "fanorona.h"
#include <algorithm>

const int ROWS = 5;
const int COLS = 9;
const int CELLS = ROWS * COLS;
const int DRAW_LIMIT = 50;   // after 50 moves without any captures


Fanorona::Fanorona() {
    int start[45] = {
        2,2,2,2,2,2,2,2,2,
        2,2,2,2,2,2,2,2,2,
        1,2,1,2,0,1,2,1,2,
        1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1 };

    for (int i = 0; i < CELLS; i++)
        board_[i] = start[i];
    
    current_ = 1;
    chaining_ = false;
    activePos_ = -1;
    lastDir_ = -1;
    movesSinceCapture_ = 0;
}


int Fanorona::otherPlayer(int player) const {
    if (player == 1)
        return 2;
    else
        return 1;
}

int Fanorona::oppositeDir(int dir) const {
    if (dir < 0 || dir > 7)
        return -1;    // invalid direction
    
    if (dir % 2 == 0)      // opposite direction is 1 more or less
        return dir + 1;

    return dir - 1;
}

int Fanorona::rowOf(int pos) const {
    return pos / COLS;
}
int Fanorona::colOf(int pos) const {
    return pos % COLS;
}
int Fanorona::posOf(int r, int c) const {
    return r * COLS + c;
}

bool Fanorona::inBoard(int r, int c) const {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        return true;
    
    return false;
}

bool Fanorona::isStrong(int pos) const {      // A point has diagonal lines only when "row + col" is even.
    if ((rowOf(pos) + colOf(pos)) % 2 == 0)
        return true;
    
    return false;
}

