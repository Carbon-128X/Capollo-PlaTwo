#pragma once
#include <set>
#include <utility>
#include <vector>
#include <string>
#include "gameMediator.h"
#include <map>
using namespace std;

class DotsAndBoxes : public GameMediator {
private:
    int rows_;
    int cols_;
    int current_;   //player's turn(1 or 2)
    int score_[3];  // player1->score_[1]    and    player2->score_[2]     (index 0 is unused)

    //a set of pair for eaach line that has drawn(small dot, big dot).
    set<pair<int, int>> lines_;
    map<pair<int,int>, int> lineOwner_;
    // (rows-1) * (cols-1) boxes, numbered like the dots.
    vector<int> boxOwner_;

    // ----------------------------------------- helper functions -----------------------------------------
    int dotIndex(int r, int c) const;
    int boxIndex(int r, int c) const;
    bool areNeighbours(int a, int b) const; //returns true if there could be line between two dots
    bool hasLine(int a, int b) const;   //returns true if there is a drawn line between two dots
    int totalLines() const;   //number of total possible lines(for gameOver)
    bool boxIsComplete(int indx) const;   //returns true if all side lines of a box were drawn
    static pair<int, int> sorted(int a, int b);   //sort the line pair (smaller dot, bigger dot)
public:
    //constructor
    DotsAndBoxes(int rows, int cols);

    //inherited functions of GameMediator
    string name() const override;
    int currentPlayer() const override;
    vector<Move> legalMoves() const override;
    bool applyMove(const Move& move) override;
    bool isGameOver() const override;
    int winner() const override;
    int score(int player) const override;

    // ---------------------------------- read access to the state ----------------------------------
    //for UI to draw the board and runnig the game and also saving
    int rows() const;
    int cols() const;
    vector<Move> drawnLines() const;
    vector<int> boxOwners() const;    // owner of every squares in order
    int boxOwner(int r, int c) const;    // owner of each square(0=no one,1=player 1, 2=player 2)

    // Rebuilding a game from a saved state
    void loadState(int rows, int cols, int current,
        int score1, int score2,
        const vector<Move>& lines,
        const vector<int>& boxOwners);


    int lineOwner(int from,int to) const;
    map<pair<int,int>,int> lineOwners() const;
    void forceNextPlayer();
};
