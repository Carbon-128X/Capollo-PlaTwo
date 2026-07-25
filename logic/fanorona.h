#pragma once
#include <vector>
#include <string>
#include "gameMediator.h"

using namespace std;

// tip: Move{-1,-1} means "finishing the turn".

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

    // ----------------------------------------- helper functions -----------------------------------------
    int oppositeDir(int dir) const;
    int otherPlayer(int player) const;
    int rowOf(int pos) const;
    int colOf(int pos) const;
    int posOf(int row, int col) const;
    bool inBoard(int row, int col) const;
    bool isStrong(int pos) const;     // returns true if this point have diagonal lines
    int step(int pos, int dir) const;      // neighbour position in a direction (-1 if invalid)
    bool connected(int pos, int dir) const;      // returns true if there is a real line to the direction
    int dirBetween(int from, int to) const;      // direction from a point to a neighbour
    int countPieces(int player) const;
    vector<int> approachTargets(int destination, int dir, int enemy) const;
    vector<int> withdrawTargets(int from, int dir, int enemy) const;
    bool canCapture(int from, int to) const;      // returns true if this step can capture
    vector<Move> allSteps(int player) const;
    vector<Move> capturingSteps(int player) const;      // the steps that capture
    vector<Move> chainMoves() const;
    void doCapture(int from, int to);
    void endTurn();

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
