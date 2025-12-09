<div align="center">

# ♟️ Chess AI Engine

A chess engine built from scratch in C++ with SFML, featuring bitboard representation and advanced search algorithms.

![Chess Demo](https://github.com/user-attachments/assets/2448123b-b52c-4738-b487-77500478518d)

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![SFML](https://img.shields.io/badge/SFML-2.6-green.svg)](https://www.sfml-dev.org/)
[![License](https://img.shields.io/badge/license-MIT-orange.svg)](LICENSE)

**Current Strength:** ~1200 Elo | **Search Depth:** 6 plies

</div>

---

## 🎯 Features

### Chess Engine
- **Bitboard representation** for efficient board state management
- **Minimax algorithm** with alpha-beta pruning
- **Move ordering** using MVV-LVA (Most Valuable Victim - Least Valuable Attacker)
- **Zobrist hashing** for transposition table
- **Legal move generation** including special moves (castling, en passant, promotion)
- **Check and checkmate detection**

### User Interface
- **Interactive GUI** built with SFML
- **Drag-and-drop** piece movement
- **Move validation** with visual feedback
- **Board highlighting** for selected pieces and legal moves

