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

vector<Move> Fanorona::allSteps(int player) const {      // all possible steps
    vector<Move> moves;

    for (int pos = 0; pos < CELLS; pos++)
    {
        if (board_[pos] != player)
            continue;
        for (int dir = 0; dir < 8; dir++)
        {
            if (!connected(pos, dir))
                continue;
            int des = step(pos, dir);
            if (board_[des] == 0)
                moves.push_back(Move{ pos, des });
        }
    }

    return moves;
}

vector<Move> Fanorona::capturingSteps(int player) const {
    vector<Move> moves;
    vector<Move> all = allSteps(player);
    for (int i = 0; i < (int)all.size(); i++)
        if (canCapture(all[i].from, all[i].to))
            moves.push_back(all[i]);

    return moves;
}

vector<Move> Fanorona::chainMoves() const {
    vector<Move> moves;

    for (int dir = 0; dir < 8; dir++)
    {
        int to = step(activePos_, dir);


        if (dir == lastDir_)      // can't repeat a direction
            continue;

        if (!connected(activePos_, dir))
            continue;

        if (board_[to] != 0)
            continue;

        bool wasVisited = false;
        for (auto it = visited_.begin(); it != visited_.end(); it++)    // checking if "to" was already visited
            if (*it == to)
            {
                wasVisited = true;
                break;
            }
        if (wasVisited)
            continue;

        if (canCapture(activePos_, to))
            moves.push_back(Move{ activePos_, to });
    }
    return moves;
}

void Fanorona::doCapture(int from, int to) {
    int dir = dirBetween(from, to);
    int enemy = otherPlayer(current_);

    vector<int> approach = approachTargets(to, dir, enemy);
    vector<int> withdraw = withdrawTargets(from, dir, enemy);

    board_[to] = current_;
    board_[from] = 0;

    vector<int> captured;
    if (approach.size() >= withdraw.size())
        captured = approach;
    else
        captured = withdraw;

    for (int i = 0; i < (int)captured.size(); i++)
        board_[captured[i]] = 0;

    movesSinceCapture_ = 0;
}

void Fanorona::endTurn() {
    chaining_ = false;
    activePos_ = -1;
    lastDir_ = -1;
    visited_.clear();

    current_ = otherPlayer(current_);
}



string Fanorona::name() const {
    return "Fanorona";
}

int Fanorona::currentPlayer() const {
    return current_;
}

vector<Move> Fanorona::legalMoves() const {
    vector<Move> moves;

    if (isGameOver())
        return moves;

    if (chaining_) {
        moves = chainMoves();
        moves.push_back(Move{ -1, -1 });   // for the option "finish my turn"
        return moves;
    }

    vector<Move> caps = capturingSteps(current_);
    if (!caps.empty())
        return caps;    // capturing is forced if it's possible

    return allSteps(current_);
}

bool Fanorona::applyMove(const Move& move) {
    if (isGameOver())
        return false;

    if (chaining_)
    {
        if (move.from == -1 && move.to == -1)      // player chooses to stop
        {
            endTurn();
            return true;
        }

        vector<Move> chains = chainMoves();
        if (std::find(chains.begin(), chains.end(), move) == chains.end())
            return false;

        doCapture(move.from, move.to);
        activePos_ = move.to;
        visited_.push_back(move.to);
        lastDir_ = dirBetween(move.from, move.to);

        if (chainMoves().empty())
            endTurn();

        return true;
    }


    vector<Move> caps = capturingSteps(current_);
    if (!caps.empty())
    {
        if (std::find(caps.begin(), caps.end(), move) == caps.end())
            return false;

        doCapture(move.from, move.to);
        chaining_ = true;
        activePos_ = move.to;
        visited_.clear();
        visited_.push_back(move.from);
        visited_.push_back(move.to);
        lastDir_ = dirBetween(move.from, move.to);
        if (chainMoves().empty())
            endTurn();

        return true;
    }

    
    vector<Move> all = allSteps(current_);
    if (std::find(all.begin(), all.end(), move) == all.end())
        return false;

    board_[move.to] = current_;
    board_[move.from] = 0;
    movesSinceCapture_++;
    endTurn();

    return true;
}

int Fanorona::winner() const {
    int loser = 0;
    if (countPieces(1) == 0)
        loser = 1;
    else if (countPieces(2) == 0)
        loser = 2;
    else if (!chaining_ && capturingSteps(current_).empty() && allSteps(current_).empty())      // no legal moves
        loser = current_;

    if (loser != 0)
        return otherPlayer(loser);

    if (movesSinceCapture_ >= DRAW_LIMIT)
        return 0;

    return -1;      // still going on
}

bool Fanorona::isGameOver() const {
    return winner() != -1;
}

int Fanorona::score(int player) const {
    if (player == 1 || player == 2)
        return countPieces(player);
    return 0;      // invalid
}



void Fanorona::forceNextPlayer() {
    endTurn();
}



int Fanorona::rows() const {
    return ROWS;
}
int Fanorona::cols() const {
    return COLS;
}

int Fanorona::posStatus(int pos) const {
    if (pos < 0 || pos >= CELLS)
        return -1;      // invalid
    return board_[pos];
}

bool Fanorona::isStrongPoint(int pos) const {
    if (pos < 0 || pos >= CELLS)
        return false;

    return isStrong(pos);
}

bool Fanorona::inCaptureChain() const {
    return chaining_;
}

void Fanorona::loadState(const vector<int>& board, int current, int movesSinceCapture) {
    if (board.size() != CELLS)
        return;      // if saved info was incomplete
    
    for (int i = 0; i < CELLS; i++)
        board_[i] = board[i];

    current_ = current;
    movesSinceCapture_ = movesSinceCapture;
    chaining_ = false;      // games are always saved between turns
    activePos_ = -1;
    visited_.clear();
    lastDir_ = -1;
}
