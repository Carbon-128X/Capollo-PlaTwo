#include "dotsAndBoxes.h"

DotsAndBoxes::DotsAndBoxes(int rows, int cols) {
    rows_ = rows;
    cols_ = cols;
    current_ = 1;
    for (int i = 0; i < 3; i++)
        score_[i] = 0;

    // (rows-1) * (cols-1) boxes, and no one owns them yet
    boxOwner_.assign((rows_ - 1) * (cols_ - 1), 0);
}


int DotsAndBoxes::dotIndex(int r, int c) const {
    return r * cols_ + c;
}

int DotsAndBoxes::boxIndex(int r, int c) const {
    return r * (cols_ - 1) + c;
}

bool DotsAndBoxes::areNeighbours(int a, int b) const {
    int row_a = a / cols_;
    int col_a = a % cols_;
    int row_b = b / cols_;
    int col_b = b % cols_;

    if (row_a == row_b && (col_a == col_b - 1 || col_a == col_b + 1)) // in the same row, next or previos col
        return true;
    if (col_a == col_b && (row_a == row_b - 1 || row_a == row_b + 1)) // in the same col, next or previos row
        return true;

    return false;
}

bool DotsAndBoxes::hasLine(int a, int b) const {
    if (lines_.find(sorted(a, b)) != lines_.end())
        return true;

    return false;
}

int DotsAndBoxes::totalLines() const {
    // horizontal lines: rows * (cols-1),  vertical lines: cols * (rows-1)
    return rows_ * (cols_ - 1) + cols_ * (rows_ - 1);
}

bool DotsAndBoxes::boxIsComplete(int indx) const {
    int r = indx / (cols_ - 1);
    int c = indx % (cols_ - 1);

    int topLeft = dotIndex(r, c);   // top-left dot is in the row r and col c
    int topRight = dotIndex(r, c + 1);
    int bottomLeft = dotIndex(r + 1, c);
    int bottomRight = dotIndex(r + 1, c + 1);

    if (hasLine(topLeft, topRight) && hasLine(bottomLeft, bottomRight)
        && hasLine(topLeft, bottomLeft) && hasLine(topRight, bottomRight))
        return true;

    return false;
}

pair<int, int> DotsAndBoxes::sorted(int a, int b) {
    if (a <= b)
        return make_pair(a, b);
    
    return make_pair(b, a);
}




string DotsAndBoxes::name() const {
    return "Dots and Boxes";
}

int DotsAndBoxes::currentPlayer() const {
    return current_;
}

vector<Move> DotsAndBoxes::legalMoves() const {
    vector<Move> moves;
    if (isGameOver())
        return moves;  // no moves left

    // horizontal lines
    for (int r = 0; r < rows_; ++r)
        for (int c = 0; c + 1 < cols_; ++c)    // each time, the line between (r,c) and (r,c+1)
        {
            int a = dotIndex(r, c);
            int b = dotIndex(r, c + 1);
            if (!hasLine(a, b))
                moves.push_back(Move{ a, b });
        }
    // vertical lines
    for (int r = 0; r + 1 < rows_; ++r)
        for (int c = 0; c < cols_; ++c)    // each time, the line between (r,c) and (r+1,c)
        {
            int a = dotIndex(r, c);
            int b = dotIndex(r + 1, c);
            if (!hasLine(a, b))
                moves.push_back(Move{ a, b });
        }

    return moves;
}

bool DotsAndBoxes::applyMove(const Move& move) {
    if (isGameOver())
        return false;

    pair<int, int> line = sorted(move.from, move.to);
    int a = line.first;
    int b = line.second;

    // rejecting illegal moves(we could also use legalMoves() here but it's not much enhanced)
    int dotsCount = rows_ * cols_;
    if (a < 0 || b >= dotsCount)
        return false;
    if (!areNeighbours(a, b))
        return false;
    if (hasLine(a, b))
        return false;

    lines_.insert(line);
    lineOwner_[line] = current_;

    // checking if some boxes become complete after we draw this line
    int completed = 0;
    int boxCount = (rows_ - 1) * (cols_ - 1);
    for (int box = 0; box < boxCount; ++box)
        if (boxIsComplete(box) && boxOwner_[box] == 0)
        {
            boxOwner_[box] = current_;   // this player is now the owner of this square
            score_[current_]++;
            completed++;
        }

    if (completed == 0)    // passing the turn to the other player if this one didn't complete any box
    {
        if (current_ == 1)
            current_ = 2;
        else
            current_ = 1;
    }

    return true;
}

bool DotsAndBoxes::isGameOver() const {
    // the game gets over when every possible lines has been drawn
    if ((int)lines_.size() == totalLines())
        return true; // we could also use boxIsComplete() for every boxes but not enhanced

    return false;
}

int DotsAndBoxes::winner() const {
    if (!isGameOver())
        return -1;    // not finished yet
    if (score_[1] > score_[2])
        return 1;
    else if (score_[2] > score_[1])
        return 2;
    else
        return 0;    // draw
}

int DotsAndBoxes::score(int player) const {
    if (player == 1 || player == 2) // for not making a mistake
        return score_[player];
    return 0;
}



int DotsAndBoxes::rows() const {
    return rows_;
}
int DotsAndBoxes::cols() const {
    return cols_;
}

vector<Move> DotsAndBoxes::drawnLines() const {
    vector<Move> result;

    for (auto it = lines_.begin(); it != lines_.end(); it++)
        result.push_back(Move{ it->first, it->second });
    
    return result;
}

vector<int> DotsAndBoxes::boxOwners() const {
    return boxOwner_;
}

int DotsAndBoxes::boxOwner(int r, int c) const {
    return boxOwner_[boxIndex(r, c)];
}



void DotsAndBoxes::loadState(int rows, int cols, int current,
    int score1, int score2,
    const vector<Move>& lines,
    const vector<int>& BoxOwners) {
    rows_ = rows;
    cols_ = cols;
    current_ = current;
    score_[0] = 0;
    score_[1] = score1;
    score_[2] = score2;

    lines_.clear();
    lineOwner_.clear();
    for (auto it = lines.begin(); it != lines.end(); it++){
        lines_.insert(sorted(it->from, it->to));
        lineOwner_[sorted(it->from,it->to)] = current_;
    }
    boxOwner_ = BoxOwners;
}


int DotsAndBoxes::lineOwner(int from,int to) const {
    auto p = sorted(from,to);
    auto it = lineOwner_.find(p);

    if(it == lineOwner_.end()){
        return 0;
    }
    return it->second;
}

map<pair<int,int>,int> DotsAndBoxes::lineOwners() const {
    return lineOwner_;
}

void DotsAndBoxes::forceNextPlayer() {
    if(current_ == 1){
        current_ = 2;
    }
    else{
        current_ = 1;
    }
}