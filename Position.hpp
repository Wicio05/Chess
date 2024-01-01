#ifndef POSITION_HPP
#define POSITION_HPP

#include "Macros.hpp"
#include <SDL.h>
#include <vector>
#include <string>
#include "Piece.hpp"

class Position
{
public:
    Position(SDL_Renderer* renderer);
    Position(const std::string fen, SDL_Renderer* renderer);
    ~Position();

    //print bitboard function
    void printBitboard(U64 bitboard);

    //print board
    void printBoard(); 

    //parse FEN string
    void parseFEN(const std::string fenStr);

    //getters
    //get bitboard for the given piece
    U64 getBitboard(int piece);

    //get occupancy for the given side
    U64 getOccupancy(int side);

    //get bitboard pointer
    U64* getBitboardPtr();

    //get occupancy pointer
    U64* getOccupancyPtr();

    //get side
    int getSide();

    //get castling rights
    int getCastling();

    //get enpassant square
    int getEnpassant();

    //get piece from char
    int getPiece(char c);

    //get piece pointer
    Piece* getPiecePtr(int square);

    Piece* getPiecePtr(int rank, int file);

    //setters
    //set bitboard for the given piece
    void setBitboard(int piece, U64 bitboard);

    //set occupancy for the given side
    void setOccupancy(int side, U64 occupancy);

    //set side 
    void setSide(int side);

    //set castling rights
    void setCastling(int castling);

    //set enpassant
    void setEnpassant(int square);

    //set piece
    void setPiece(int square, Piece * piece);


    void update();

    void render();


private:
    SDL_Renderer* m_renderer;

    Piece* pieces[64];

    // Moves* movelist = nullptr;

    //piece bitboards
    U64 m_bitboards[12];

    //occupancy bitboards
    U64 m_occupancies[3];

    //side to move
    int m_side = white;

    //enpassant square
    int m_enpassant = no_sq;

    //castling rights
    int m_castling;

    std::unordered_map<char, int> char_pieces = 
    {
        {'B', B},
        {'K', K},
        {'N', N},
        {'P', P},
        {'Q', Q},
        {'R', R},
        {'b', b},
        {'k', k},
        {'n', n},
        {'p', p},
        {'q', q},
        {'r', r}
    };  
};

#endif // !POSITION_HPP
