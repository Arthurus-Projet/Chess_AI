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
/*
uint64_t ZobristHashing::updateHash(uint64_t& hash, Move& move) {

    hash ^= pieceSquare[move.piece][move.from];
    hash ^= pieceSquare[move.piece][move.to];

    if(move.capturedType != NONE)
        hash ^= pieceSquare[move.capturedType][move.to];

    int pos = givePositionForCastlingRightsBefore(move);
    hash ^= castlingRights[pos];

    pos = givePositionForCastlingRights(move);
    hash ^= castlingRights[pos];

    if (move.enPassantSquareBefore != -1) 
        hash ^= enPassantNum[(move.enPassantSquareBefore % 8)];

    if (move.enPassantSquareAfter != -1)
        hash ^= enPassantNum[(move.enPassantSquareAfter % 8)];

    hash ^= sideToMove;

    return hash;
}
*/

uint64_t ZobristHashing::updateHash(uint64_t& hash, Move& move) {
    
    // ════════════════════════════════════════════════════════
    // PROMOTION
    // ════════════════════════════════════════════════════════
    if (((move.piece == WHITE_PAWN) || (move.piece == BLACK_PAWN)) && 
        (((move.to >> 3) == 7) || ((move.to >> 3) == 0))) {
        
        // Remove pawn
        hash ^= pieceSquare[move.piece][move.from];
        
        // add Queen
        PieceType queen = (move.piece == WHITE_PAWN) ? WHITE_QUEEN : BLACK_QUEEN;
        hash ^= pieceSquare[queen][move.to];
        
        if (move.capturedType != NONE)
            hash ^= pieceSquare[move.capturedType][move.to];
        
        // Castling rights
        int posBefore = givePositionForCastlingRightsBefore(move);
        hash ^= castlingRights[posBefore];
        int posAfter = givePositionForCastlingRights(move);
        hash ^= castlingRights[posAfter];
        
        // En passant
        if (move.enPassantSquareBefore != -1) 
            hash ^= enPassantNum[move.enPassantSquareBefore % 8];
        if (move.enPassantSquareAfter != -1)
            hash ^= enPassantNum[move.enPassantSquareAfter % 8];
        
        // Side to move
        hash ^= sideToMove;
        

        return hash;
    }
    
    // ════════════════════════════════════════════════════════
    // CASTLING
    // ════════════════════════════════════════════════════════
    if (move.moveType == CASTLING) {
        bool isWhite = move.piece < 6;
        
        hash ^= pieceSquare[move.piece][move.from];
        hash ^= pieceSquare[move.piece][move.to];
        
        PieceType rook = isWhite ? WHITE_ROOK : BLACK_ROOK;
        
        if (isWhite) {
            if (move.castlingType == KINGSIDE) {
                hash ^= pieceSquare[rook][7];  
                hash ^= pieceSquare[rook][5];  
            } else {
                hash ^= pieceSquare[rook][0];  
                hash ^= pieceSquare[rook][3];  
            }
        } else {
            if (move.castlingType == KINGSIDE) {
                hash ^= pieceSquare[rook][63]; 
                hash ^= pieceSquare[rook][61];
            } else {
                hash ^= pieceSquare[rook][56]; 
                hash ^= pieceSquare[rook][59]; 
            }
        }
        
        // Castling rights
        int posBefore = givePositionForCastlingRightsBefore(move);
        hash ^= castlingRights[posBefore];
        int posAfter = givePositionForCastlingRights(move);
        hash ^= castlingRights[posAfter];
        
        // En passant
        if (move.enPassantSquareBefore != -1) 
            hash ^= enPassantNum[move.enPassantSquareBefore % 8];
        if (move.enPassantSquareAfter != -1)
            hash ^= enPassantNum[move.enPassantSquareAfter % 8];
        
        // Side to move
        hash ^= sideToMove;
        
        return hash;
    }
    
    // ════════════════════════════════════════════════════════
    // EN PASSANT
    // ════════════════════════════════════════════════════════
    if (move.moveType == EN_PASSANT) {
        bool isWhite = move.piece < 6;
        

        hash ^= pieceSquare[move.piece][move.from];
        hash ^= pieceSquare[move.piece][move.to];
        
        int capturedSquare = isWhite ? (move.to - 8) : (move.to + 8);
        hash ^= pieceSquare[move.capturedType][capturedSquare];
        
        // Castling rights
        int posBefore = givePositionForCastlingRightsBefore(move);
        hash ^= castlingRights[posBefore];
        int posAfter = givePositionForCastlingRights(move);
        hash ^= castlingRights[posAfter];
        
        // En passant
        if (move.enPassantSquareBefore != -1) 
            hash ^= enPassantNum[move.enPassantSquareBefore % 8];
        if (move.enPassantSquareAfter != -1)
            hash ^= enPassantNum[move.enPassantSquareAfter % 8];
        
        // Side to move
        hash ^= sideToMove;
        
        return hash;
    }
    
    // ════════════════════════════════════════════════════════
    // NORMAL
    // ════════════════════════════════════════════════════════
    
    hash ^= pieceSquare[move.piece][move.from];
    hash ^= pieceSquare[move.piece][move.to];
    
    if (move.capturedType != NONE)
        hash ^= pieceSquare[move.capturedType][move.to];
    
    int posBefore = givePositionForCastlingRightsBefore(move);
    hash ^= castlingRights[posBefore];
    int posAfter = givePositionForCastlingRights(move);
    hash ^= castlingRights[posAfter];
    
    // En passant
    if (move.enPassantSquareBefore != -1) 
        hash ^= enPassantNum[move.enPassantSquareBefore % 8];
    if (move.enPassantSquareAfter != -1)
        hash ^= enPassantNum[move.enPassantSquareAfter % 8];
    
    // Side to move
    hash ^= sideToMove;
    
    return hash;
}
