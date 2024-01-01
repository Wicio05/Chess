#ifndef MACROS_HPP
#define MACROS_HPP

//system headers
#include <bits/stdc++.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <chrono>
#include <cstring>
#include <algorithm>

#include "Consts.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

//define bitboard data type
#define U64 unsigned long long

//set|get|remove macros
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define remove_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))

//counts 0 bits until a 1 bit appears ex. 0000 0000 0100 -- return 9
#define get_LSB(bitboard) (__builtin_ctzll(bitboard))

inline int pop_LSB(U64 &b) 
{
    int i = get_LSB(b);
    b &= b - 1;
    return i;
}

#define get_MSB(bitboard) (__builtin_clzll(bitboard))

//couts  bits in the num ex. 0010 0110 0111 0000 -- return 6
#define count_bits(bitboard) (__builtin_popcountll(bitboard))

//encode move
#define encode_move(src, trg, piece, promoted, capture, double, enpassant, castling)    \
(src) |                                                                                 \
(trg << 6) |                                                                            \
(piece << 12) |                                                                         \
(promoted << 16) |                                                                      \
(capture << 20) |                                                                       \
(double << 21) |                                                                        \
(enpassant << 22) |                                                                     \
(castling << 23)                                                                        \

//decode src square
#define get_src(move) (move & 0x3f)

//decode trg square
#define get_trg(move) ((move & 0xfc0) >> 6)

//decode piece
#define get_piece(move) ((move & 0xf000) >> 12)

//decode promoted piece
#define get_promotion(move) ((move & 0xf0000) >> 16)

//decode capture flag
#define get_capture(move) ((move & 0x100000) >> 20)

//decode double pawn move flag
#define get_double(move) ((move & 0x200000) >> 21)

//decode enpassant flag
#define get_enpassant(move) ((move & 0x400000) >> 22)

//decode castling flag
#define get_castling(move) ((move & 0x800000) >> 23)


const U64 RANK2 = 0xFF000000000000ULL;

const U64 RANK7 = 0xFF00ULL;

//not 'A' file const
/*
  8  0 1 1 1 1 1 1 1
  7  0 1 1 1 1 1 1 1
  6  0 1 1 1 1 1 1 1
  5  0 1 1 1 1 1 1 1
  4  0 1 1 1 1 1 1 1
  3  0 1 1 1 1 1 1 1
  2  0 1 1 1 1 1 1 1
  1  0 1 1 1 1 1 1 1

     a b c d e f g h
*/
const U64 NOT_A_FILE = 18374403900871474942ULL;

//not 'H' file const
/*
  8  1 1 1 1 1 1 1 0
  7  1 1 1 1 1 1 1 0
  6  1 1 1 1 1 1 1 0
  5  1 1 1 1 1 1 1 0
  4  1 1 1 1 1 1 1 0
  3  1 1 1 1 1 1 1 0
  2  1 1 1 1 1 1 1 0
  1  1 1 1 1 1 1 1 0

     a b c d e f g h
*/
const U64 NOT_H_FILE = 9187201950435737471ULL;

//not 'HG' files const
/*
  8  1 1 1 1 1 1 0 0
  7  1 1 1 1 1 1 0 0
  6  1 1 1 1 1 1 0 0
  5  1 1 1 1 1 1 0 0
  4  1 1 1 1 1 1 0 0
  3  1 1 1 1 1 1 0 0
  2  1 1 1 1 1 1 0 0
  1  1 1 1 1 1 1 0 0

     a b c d e f g h
*/
const U64 NOT_HG_FILE = 4557430888798830399ULL;

//not 'AB' file const
/*
  8  0 0 1 1 1 1 1 1
  7  0 0 1 1 1 1 1 1
  6  0 0 1 1 1 1 1 1
  5  0 0 1 1 1 1 1 1
  4  0 0 1 1 1 1 1 1
  3  0 0 1 1 1 1 1 1
  2  0 0 1 1 1 1 1 1
  1  0 0 1 1 1 1 1 1

     a b c d e f g h
*/
const U64 NOT_AB_FILE = 18229723555195321596ULL;

#endif // !MACROS_HPP
