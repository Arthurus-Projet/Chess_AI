#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <random>
#include <array>
#include <cstdint>

struct Move;

constexpr int NUM_PIECES = 12;
constexpr int NUM_SQUARES = 64;
constexpr int NUM_CASTLING_RIGHTS = 16; // 2**4 = 16
constexpr int NUM_EN_PASSANT = 8;

class ZobristHashing {
    public:
        std::array<std::array<uint64_t, NUM_SQUARES>, NUM_PIECES> pieceSquare;
        std::array<uint64_t, NUM_CASTLING_RIGHTS> castlingRights;
        std::array<uint64_t, NUM_EN_PASSANT> enPassantNum;
        uint64_t sideToMove;
        ZobristHashing(uint64_t seed);
        uint64_t updateHash(uint64_t& hash, Move& move);

        int givePositionForCastlingRights(Move& move);
        int givePositionForCastlingRightsBefore(Move& move);

};



#endif
