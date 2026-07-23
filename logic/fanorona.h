#pragma once
#include <vector>
#include <string>
#include "gameMediator.h"

using namespace std;

class Fanorona : public GameMediator {
private:
    int board_[45];      // 0 = empty, 1 = player 1, 2 = player 2
    int current_;      //player's turn(1 or 2)

    // capture chain state
    bool chaining_;
    int activePos_;    // position of the piece that is chaining
    vector<int> visited_;      // positions that piece already used in this turn
    int lastDir_;      // last direction the piece moved (can't be repeated)
    int movesSinceCapture_;      // counter for the draw rule

public:
    // constructor
    Fanorona();

    // inherited functions of GameMediator
    string name() const override;
    int currentPlayer() const override;
    vector<Move> legalMoves() const override;
    bool applyMove(const Move& move) override;
    bool isGameOver() const override;
    int winner() const override;
    int score(int player) const override;

    void forceNextPlayer();

    // ---------------------------------- read access to the state ----------------------------------
    int rows() const;
    int cols() const;
    int posStatus(int pos) const;
    bool isStrongPoint(int pos) const;      // for drawing diagonal lines
    bool inCaptureChain() const;      // for chaining options (or buttons)

    // Rebuilding the game from a saved state
    void loadState(const vector<int>& board, int current, int movesSinceCapture);
};
