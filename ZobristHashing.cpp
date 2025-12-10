#include "Headers/ZobristHashing.h"
#include "Headers/chessboard.h"
#include <iostream>




ZobristHashing::ZobristHashing(uint64_t seed) {

    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<uint64_t> dist;

    for (int piece = 0; piece < NUM_PIECES; ++piece) {
        for (int square = 0; square < NUM_SQUARES; ++square) {
            pieceSquare[piece][square] = dist(rng);
        }
    }

    for (int i = 0; i < NUM_CASTLING_RIGHTS; ++i) {
        castlingRights[i] = dist(rng);
    }

    for (int i = 0; i < NUM_EN_PASSANT; ++i) {
        enPassantNum[i] = dist(rng);
    }

    sideToMove = dist(rng);


}


int ZobristHashing::givePositionForCastlingRights(Move& move) {
    return move.whiteKingSideCastlingAfter |
           (move.whiteQueenSideCastlingAfter << 1) |
           (move.blackKingSideCastlingAfter  << 2) |
           (move.blackQueenSideCastlingAfter << 3);
}

int ZobristHashing::givePositionForCastlingRightsBefore(Move& move) {
    return move.whiteKingSideCastlingBefore |
           (move.whiteQueenSideCastlingBefore << 1) |
           (move.blackKingSideCastlingBefore  << 2) |
           (move.blackQueenSideCastlingBefore << 3);
}




uint64_t ZobristHashing::updateHash(uint64_t& hash, Move& move) {

    

    return hash;

}

