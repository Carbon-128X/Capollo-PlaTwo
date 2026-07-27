#include "nineMensMorris.h"

// ----------------------------------------- board data -----------------------------------------

const int MILLS[16][3] = {      // all of possible mills (sorted and based on positions)
    {0,1,2},   {2,3,4},   {4,5,6},   {6,7,0},
    {8,9,10},  {10,11,12},{12,13,14},{14,15,8},
    {16,17,18},{18,19,20},{20,21,22},{22,23,16},
    {1,9,17},  {3,11,19}, {5,13,21}, {7,15,23}
};


const vector<vector<int>> POS_NEIGHBORS = {      // neighbors of each position
    {1,7},        // 0
    {0,2,9},      // 1
    {1,3},        // 2
    {2,4,11},     // 3
    {3,5},        // 4
    {4,6,13},     // 5
    {5,7},        // 6
    {6,0,15},     // 7
    {9,15},       // 8
    {8,10,1,17},  // 9
    {9,11},       // 10
    {10,12,3,19}, // 11
    {11,13},      // 12
    {12,14,5,21}, // 13
    {13,15},      // 14
    {14,8,7,23},  // 15
    {17,23},      // 16
    {16,18,9},    // 17
    {17,19},      // 18
    {18,20,11},   // 19
    {19,21},      // 20
    {20,22,13},   // 21
    {21,23},      // 22
    {22,16,15}    // 23
};

// If 100 moves passed with no piece removed, the game is draw.
static const int DRAW_LIMIT = 100;


NineMensMorris::NineMensMorris() {
    for (int i = 0; i < 24; i++)
        board_[i] = 0;
    current_ = 1;
    toPlace_[0] = 0;
    toPlace_[1] = 9;
    toPlace_[2] = 9;
    awaitingRemoval_ = false;
    movesSinceRemoval_ = 0;
}


int NineMensMorris::otherPlayer(int player) const {
    if (player == 1)
        return 2;
    else
        return 1;
}

int NineMensMorris::countPieces(int player) const {
    int count = 0;
    for (int i = 0; i < 24; i++)
        if (board_[i] == player)
            count++;
    return count;
}

bool NineMensMorris::inPlacingPhase(int player) const {
    if (toPlace_[player] > 0)
        return true;
    else
        return false;
}

bool NineMensMorris::canFly(int player) const {
    if (!inPlacingPhase(player) && (countPieces(player) == 3)) // placing should be already done
        return true;
    else
        return false;
}

bool NineMensMorris::areNeighbours(int a, int b) const {
    const vector<int>& nb = POS_NEIGHBORS[a];
    int vectorSize = (int)nb.size();
    for (int i = 0; i < vectorSize; i++)
        if (nb[i] == b)
            return true;

    return false;
}

bool NineMensMorris::formsMill(int pos, int player) const {
    for (int i = 0; i < 16; i++)
        if (pos == MILLS[i][0] || pos == MILLS[i][1] || pos == MILLS[i][2])
            if (board_[MILLS[i][0]] == player && board_[MILLS[i][1]] == player && board_[MILLS[i][2]] == player)
                return true;

    return false;
}

bool NineMensMorris::isRemovable(int pos, int opponent) const {
    if (board_[pos] != opponent)
        return false;
    if (!formsMill(pos, opponent))
        return true;

    for (int i = 0; i < 24; i++)      // if it was in a mill, we should check all of enemy pieces if they're in mill
        if (board_[i] == opponent && !formsMill(i, opponent))      // if one of the others wasn't in mill, so pos isn't removable
            return false;
    return true;
}

vector<Move> NineMensMorris::movement(int player) const {      // legal moves but without checking gameOver(for after placing)
    vector<Move> moves;

    bool fly = canFly(player);

    for (int from = 0; from < 24; from++)
    {
        if (board_[from] != player)
            continue;

        if (fly)
        {
            for (int to = 0; to < 24; to++)
                if (board_[to] == 0)
                    moves.push_back(Move{ from, to });
        }
        else
        {
            const vector<int>& neighbor = POS_NEIGHBORS[from];

            for (int i = 0; i < (int)neighbor.size(); i++)
                if (board_[neighbor[i]] == 0)
                    moves.push_back(Move{ from, neighbor[i] });
        }
    }

    return moves;
}

// ----------------------------------------- overrided methods of GameMediator -----------------------------------------

string NineMensMorris::name() const {
    return "Nine Men's Morris";
}

int NineMensMorris::currentPlayer() const {
    return current_;
}

vector<Move> NineMensMorris::legalMoves() const {
    vector<Move> moves;
    if (isGameOver())
        return moves;

    if (awaitingRemoval_)
    {
        int opponent = otherPlayer(current_);
        for (int pos = 0; pos < 24; pos++)
            if (isRemovable(pos, opponent))
                moves.push_back(Move{ -1, pos });
        return moves;
    }

    if (inPlacingPhase(current_)) {
        for (int pos = 0; pos < 24; pos++)
            if (board_[pos] == 0)
                moves.push_back(Move{ -1, pos });
        return moves;
    }

    return movement(current_);      // moving / flying phase
}

bool NineMensMorris::applyMove(const Move& move) {
    if (isGameOver())
        return false;

    if (awaitingRemoval_)
    {
        int opponent = otherPlayer(current_);
        int target = move.to;

        if (target < 0 || target >= 24)
            return false;
        
        if (!isRemovable(target, opponent))
            return false;

        board_[target] = 0;
        awaitingRemoval_ = false;
        movesSinceRemoval_ = 0;      // reset for draw rule
        current_ = otherPlayer(current_);
        return true;
    }

    if (inPlacingPhase(current_))
    {
        int pos = move.to;

        if (pos < 0 || pos >= 24)
            return false;

        if (board_[pos] != 0)
            return false;


        board_[pos] = current_;
        toPlace_[current_]--;
        
        movesSinceRemoval_++;
        
        if (formsMill(pos, current_))
        {
            awaitingRemoval_ = true;      // same player should remove
            return true;
        }

        current_ = otherPlayer(current_);      // passing the turn if player didn't made a mill
        return true;
    }

    
    // move or fly
    if (move.from < 0 || move.from >= 24 || move.to < 0 || move.to >= 24)
        return false;

    if (board_[move.from] != current_)
        return false;

    if (board_[move.to] != 0)
        return false;

    if (!canFly(current_) && !areNeighbours(move.from, move.to))
        return false;

    board_[move.from] = 0;
    board_[move.to] = current_;
    movesSinceRemoval_++;

    if (formsMill(move.to, current_))
    {
        awaitingRemoval_ = true;
        return true;
    }

    current_ = otherPlayer(current_);
    return true;
}

bool NineMensMorris::isGameOver() const {
    return winner() != -1;
}

int NineMensMorris::winner() const {
    int loser = 0;

    if (!inPlacingPhase(1) && countPieces(1) < 3)
        loser = 1;

    else if (!inPlacingPhase(2) && countPieces(2) < 3)
        loser = 2;

    else if (!inPlacingPhase(current_) && !awaitingRemoval_ && countPieces(current_) >= 3 && movement(current_).empty())
        loser = current_;      // if the current player had no legal moves

    if (loser != 0)
        return otherPlayer(loser);      // the other player wins

    if (!awaitingRemoval_ && (movesSinceRemoval_ >= DRAW_LIMIT))
        return 0;      // draw
    
    return -1;      // game is not over yet
}

int NineMensMorris::score(int player) const {
    if (player == 1 || player == 2)
        return countPieces(player);
    return 0; // for invalid input
}


// ----------------------------------------- read access + save/load -----------------------------------------

int NineMensMorris::posStatus(int pos) const {
    if (pos < 0 || pos >= 24)
        return -1;
    return board_[pos];
}

int NineMensMorris::notPlaced(int player) const {
    if (player == 1 || player == 2)
        return toPlace_[player];
    return 0;
}

bool NineMensMorris::mustRemove() const {
    return awaitingRemoval_;
}


void NineMensMorris::loadState(const vector<int>& board, int current, int toPlace1, int toPlace2,
    bool awaitingRemoval, int movesSinceRemoval) {

    for (int i = 0; i < 24; i++)
        board_[i] = board[i];
    
    current_ = current;
    toPlace_[0] = 0;
    toPlace_[1] = toPlace1;
    toPlace_[2] = toPlace2;
    awaitingRemoval_ = awaitingRemoval;
    movesSinceRemoval_ = movesSinceRemoval;
}
