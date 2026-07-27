#pragma once
#include <string>
#include <vector>
#include "move.h"

using namespace std;

class GameMediator {
public:
    virtual ~GameMediator() = default;

    virtual string name() const = 0;

    //it should return the number of current player(player 1 or 2)
    virtual int currentPlayer() const = 0;

    // Every legal move in the current position
    virtual vector<Move> legalMoves() const = 0;

    //applying the move. if the move is illegal we should Return false and don't change anything
    // If it's legal, we can pass the turn to the other player(or something else)
    virtual bool applyMove(const Move& move) = 0;

    virtual bool isGameOver() const = 0;

    //returning the end situation(1=player 1 won, 2=player 2 won, 0=draw, -1=game isn't over yet)
    virtual int winner() const = 0;

    virtual int score(int player) const = 0;
};
