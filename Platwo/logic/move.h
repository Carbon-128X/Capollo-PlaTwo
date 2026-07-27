#pragma once

// A single action which a player can do in each game

//in Dots and Boxes: draw a line between two dots(from & to)
//Nine Men's Morris: put, move, remove
//Fanorona: move and capture


struct Move {
    int from = -1;  // start dot number(-1 means not used yet)
    int to = -1;  // target dot number(-1 means not used yet)

    //comparing two moves(for checking current move with legal moves)
    bool operator==(const Move& other) const {
        return from == other.from && to == other.to;
    }
};
