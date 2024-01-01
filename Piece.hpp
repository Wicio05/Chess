#ifndef Piece_hpp
#define Piece_hpp

#include <SDL.h>
#include <vector>
#include <iostream>

#include "TextureManager.hpp"
// #include "Types.hpp"
#include "Consts.hpp"

class Piece
{
public:
    // Piece(Team _team, PieceType _type, Square _square, SDL_Renderer *_renderer);

    // Piece(Team _team, PieceType _type, SDL_Renderer *_renderer);

    Piece(int type, SDL_Renderer *renderer);

    Piece(int type, int square, SDL_Renderer *renderer);

    ~Piece();

    //get|set
    // Team getTeam() { return team; }
    // PieceType getType() { return type; }
    inline int getSquare() { return m_square; }

    inline void setSquare(int square) { m_square = square, xPos = (( m_square % 8) * 100) + 100, yPos = ((m_square / 8) * 100) + 100; }
    inline void setSquare(int rank, int file) { m_square = rank * 8 + file, xPos = file * 100 + 100, yPos = rank * 100 + 100; }
    inline void setPos(int x, int y) { xPos = x, yPos = y; }
    void setType(int type);

    inline void sayMyName() { std::cout << ascii_pieces[m_type]; }

    void update();
    void render();

    // Uint64 setOccupancy(int index, int bits_in_mask, Uint64 attack_mask);

private:
    int m_type;
    int m_side;
    int m_square;

    SDL_Texture *pieceTex = nullptr;
    SDL_Renderer *m_renderer;

    int xPos;
    int yPos;
    SDL_Rect srcRect, destRect;
};

#endif