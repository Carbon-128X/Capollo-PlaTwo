#include "fanorona.h"
#include <algorithm>

const int ROWS = 5;
const int COLS = 9;
const int CELLS = ROWS * COLS;
const int DRAW_LIMIT = 50;   // after 50 moves without any captures

// --------------------> 0   1   2   3   4   5   6   7
// --------------------> N   S   W   E   NW  SE  NE  SW <---------- 8 directions(0-3 straight, 4-7 diagonal)
const int dirRow[8] = { -1, +1,  0,  0, -1, +1, -1, +1 };
const int dirCol[8] = {  0,  0, -1, +1, -1, +1, +1, -1 };


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

int Fanorona::step(int pos, int dir) const {
    int row = rowOf(pos) + dirRow[dir];
    int column = colOf(pos) + dirCol[dir];

    if (!inBoard(row, column))
        return -1;

    return posOf(row, column);
}

bool Fanorona::connected(int pos, int dir) const {
    if (step(pos, dir) == -1)
        return false;

    if (dir >= 4 && !isStrong(pos))      // we always have straight lines, so we just check the diagonal ones
        return false;

    return true;
}

int Fanorona::dirBetween(int from, int to) const {
    int dirR = rowOf(to) - rowOf(from);
    int dirC = colOf(to) - colOf(from);

    for (int d = 0; d < 8; d++)
        if (dirRow[d] == dirR && dirCol[d] == dirC)
            return d;

    return -1;      // invalid (there is no dir between these ponits)
}

int Fanorona::countPieces(int player) const {
    int count = 0;
    for (int i = 0; i < CELLS; i++)
        if (board_[i] == player)
            count++;

    return count;
}

vector<int> Fanorona::approachTargets(int destination, int dir, int enemy) const {
    vector<int> targets;

    int enemy_pos = step(destination, dir);
    while (enemy_pos != -1 && board_[enemy_pos] == enemy)
    {
        targets.push_back(enemy_pos);
        enemy_pos = step(enemy_pos, dir);
    }

    return targets;
}

vector<int> Fanorona::withdrawTargets(int from, int dir, int enemy) const {
    vector<int> targets;

    int back = oppositeDir(dir);
    int enemy_pos = step(from, back);
    while (enemy_pos != -1 && board_[enemy_pos] == enemy)
    {
        targets.push_back(enemy_pos);
        enemy_pos = step(enemy_pos, back);
    }

    return targets;
}

bool Fanorona::canCapture(int from, int to) const {
    int enemy = otherPlayer(current_);
    int dir = dirBetween(from, to);

    if (!approachTargets(to, dir, enemy).empty() || !withdrawTargets(from, dir, enemy).empty())
        return true;
    
    return false;
}

