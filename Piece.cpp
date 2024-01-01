#include "Piece.hpp"

// Piece::Piece(Team _team, PieceType _type, Square _square, SDL_Renderer *_renderer)
//     : team(_team), type(_type), square(_square), renderer(_renderer)
// {
//     setSquare(square);
//     // set_bit(bitboard, _square);
//     switch(type)
//     {
//         case PieceType::PAWN:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wp.png", renderer) : TextureManager::LoadTexture("./assets/bp.png", renderer));
//             break;
//         }

//         case PieceType::KNIGHT:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wn.png", renderer) : TextureManager::LoadTexture("./assets/bn.png", renderer));
//             break;
//         }

//         case PieceType::BISHOP:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wb.png", renderer) : TextureManager::LoadTexture("./assets/bb.png", renderer));
//             break;
//         }

//         case PieceType::ROOK:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wr.png", renderer) : TextureManager::LoadTexture("./assets/br.png", renderer));
//             break;
//         }

//         case PieceType::QUEEN:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wq.png", renderer) : TextureManager::LoadTexture("./assets/bq.png", renderer));
//             break;
//         }

//         case PieceType::KING:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wk.png", renderer) : TextureManager::LoadTexture("./assets/bk.png", renderer));
//             break;
//         }

//         default:
//         {
//             pieceTex = nullptr;
//         }
//     }

// }

// Piece::Piece(Team _team, PieceType _type, SDL_Renderer *_renderer)
//     : team(_team), type(_type), renderer(_renderer)
// {
//     // set_bit(bitboard, _square);
//     switch(type)
//     {
//         case PieceType::PAWN:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wp.png", renderer) : TextureManager::LoadTexture("./assets/bp.png", renderer));
//             break;
//         }

//         case PieceType::KNIGHT:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wn.png", renderer) : TextureManager::LoadTexture("./assets/bn.png", renderer));
//             break;
//         }

//         case PieceType::BISHOP:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wb.png", renderer) : TextureManager::LoadTexture("./assets/bb.png", renderer));
//             break;
//         }

//         case PieceType::ROOK:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wr.png", renderer) : TextureManager::LoadTexture("./assets/br.png", renderer));
//             break;
//         }

//         case PieceType::QUEEN:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wq.png", renderer) : TextureManager::LoadTexture("./assets/bq.png", renderer));
//             break;
//         }

//         case PieceType::KING:
//         {
//             pieceTex = (team == Team::WHITE ? TextureManager::LoadTexture("./assets/wk.png", renderer) : TextureManager::LoadTexture("./assets/bk.png", renderer));
//             break;
//         }

//         default:
//         {
//             pieceTex = nullptr;
//         }
//     }

// }

Piece::Piece(int type, SDL_Renderer *renderer) : m_type(type), m_renderer(renderer)
{
    setType(type);
}

Piece::Piece(int type, int square, SDL_Renderer *renderer) : m_type(type), m_square(square), m_renderer(renderer)
{
    setType(type);
    setSquare(square);
}

Piece::~Piece()
{
    SDL_DestroyTexture(pieceTex);
}

void Piece::update()
{
    srcRect.h = 150;
    srcRect.w = 150;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xPos;
    destRect.y = yPos;
    destRect.h = 100;
    destRect.w = 100;
}

void Piece::render()
{
    SDL_RenderCopy(m_renderer, pieceTex, &srcRect, &destRect);
}

void Piece::setType(int type)
{
    m_type = type;

    if(type >= P && type <= K)
    {
        m_side = white;
        std::string path = "./assets/w";
        path += pieces_lower[type];
        path += ".png";
        pieceTex = TextureManager::LoadImgTexture(path.c_str(), m_renderer);
    }
    else if(type >= p && type <= k)
    {
        m_side = black;
        std::string path = "./assets/b";
        path += pieces_lower[type];
        path += ".png";
        pieceTex = TextureManager::LoadImgTexture(path.c_str(), m_renderer);
    }
}

// Uint64 Piece::setOccupancy(int index, int bits_in_mask, Uint64 attack_mask)
// {
//     //occupancy map
//     Uint64 occupancy = 0ULL;

//     //loop over the range of bits within attack mask
//     for(int i = 0; i < bits_in_mask; ++i)
//     {
//         //get LSB index of atack mask
//         Square sq = (Square) get_LSB(attack_mask);

//         //remove LSB in attack map
//         remove_bit(attack_mask, sq);

//         //make sure occupancy is on board
//         if(index & (1 << i))
//             //populate occupancy
//             occupancy |= (1ULL << sq);
//     }

//     //return occupancy map
//     return occupancy;
// }