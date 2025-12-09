#include "Headers/ZobristHashing.h"
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
        enPassant[i] = dist(rng);
    }

    sideToMove = dist(rng);


}