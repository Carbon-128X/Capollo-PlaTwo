#pragma once
#include <vector>
#include <string>
#include "gameMediator.h"

using namespace std;

// for putting or removing a piece -> {from = -1, to = position}

class NineMensMorris : public GameMediator {
private:
    int board_[24];          // 0 means empty    |    1 means player1    |    2 means player2
    int current_;            // player's turn(1 or 2)
    int toPlace_[3];         // left pieces to place in the board for each player (index 1->player1 , 2->player2)    (index 0 is unused)
    bool awaitingRemoval_;   // a flag that becomes true after current player made a mill and should remove one of other player's piece
    int movesSinceRemoval_;  // counter used by the draw rule

    // ----------------------------------------- helper funcs -----------------------------------------
    int otherPlayer(int player) const;
    int countPieces(int player) const;      // placed pieces of player
    bool inPlacingPhase(int player) const;      // returns true if player still has pieces to place
    bool canFly(int player) const;      // returns true if player only have 3 pieces
    bool areNeighbours(int a, int b) const;
    bool formsMill(int pos, int player) const;      // returns true if pos and its neighbors are a mill
    bool isRemovable(int pos, int opponent) const;
    vector<Move> movement(int player) const;

public:
    //constructor
    NineMensMorris();

    //inherited functions of GameMediator
    string name() const override;
    int currentPlayer() const override;
    vector<Move> legalMoves() const override;
    bool applyMove(const Move& move) override;
    bool isGameOver() const override;
    int winner() const override;
    int score(int player) const override;

    // ---------------------------------- read access to the state ----------------------------------
    // for UI to draw the board and runnig the game and also saving
    int posStatus(int pos) const;
    int notPlaced(int player) const;      // pieces the player hasn't placed yet
    bool mustRemove() const;      // returns true if current player made a mill but didn't remove a piece

    // Rebuilding a game from a saved state
    void loadState(const vector<int>& board, int current, int toPlace1, int toPlace2,
        bool awaitingRemoval, int movesSinceRemoval);

    void forceNextPlayer();
};