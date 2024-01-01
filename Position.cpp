#include "Position.hpp"

Position::Position(SDL_Renderer* renderer) : m_renderer(renderer)
{
    for(int i = 0; i < 64; ++i)
    {
        pieces[i] = nullptr;
    }
}

Position::Position(const std::string fenStr, SDL_Renderer* renderer) : m_renderer(renderer)
{
    for(int i = 0; i < 64; ++i)
    {
        pieces[i] = nullptr;
    }
    
    parseFEN(fenStr);

}

Position::~Position()
{
    for(int i = 0; i < 64; ++i)
    {
        // if(pieces[i] != nullptr)
        // {
            delete pieces[i];
        // }
    }

    // delete pieces;
}

void Position::printBitboard(U64 bitboard)
{
    std::cout << "\n";

    //loop over board ranks
    for(int rank = 0; rank < 8; ++rank)
    {
        //loop over bard files
        for(int file = 0; file < 8; ++file)
        {
            //convert file & rank into square index
            int square = rank * 8 + file;

            //print rank number
            if(!file) std::cout << "  " << 8 - rank << " ";

            //print bit
            std::cout << " " << (get_bit(bitboard, square) ? 1 : 0);
        }
        std::cout << "\n";
    }

    //print file number
    std::cout << "\n     a b c d e f g h\n";

    //print bitboard as decimal num
    std::cout << "     Bitboard: " << bitboard << "\n";
}

void Position::printBoard()
{
    std::cout << "\n";

    //loop over board ranks
    for(int rank = 0; rank < 8; ++rank)
    {
        //loop over board files 
        for(int file = 0; file < 8; ++file)
        {
            //init square
            int square = rank * 8 + file;

            //define piece
            int piece = -1;

            //print ranks
            if(!file)
            {
                std::cout << " " << 8 - rank << " ";
            }

            //loop over all piece bitboards
            for(int bb_piece = P; bb_piece <= k; ++bb_piece)
            {
                if(get_bit(m_bitboards[bb_piece], square))
                {
                    piece = bb_piece;
                }
            }
            
            #ifdef _WIN32
                std::cout << " " << (piece == -1 ? '.' : ascii_pieces[piece]);
            #else
                std::cout << " " << (piece == -1 ? '.' : ascii_pieces[piece]);
            #endif
        }

        std::cout << "\n";
    }

    //print board files
    std::cout << "\n    a b c d e f g h\n\n";

    //print side to move
    std::cout << " Side: " << (m_side && (m_side != -1) ? "black\n" : "white\n");

    //priont enpassant square
    std::cout << " Enpassant: " << (m_enpassant != no_sq ? square_to_coord[m_enpassant] : "null") << "\n";

    //print castling rights
    std::cout << " Castling: " << (m_castling & wk ? "K" : "-") << (m_castling & wq ? "Q" : "-") << (m_castling & bk ? "k" : "-") << (m_castling & bq ? "q" : "-") << "\n";
}

void Position::parseFEN(const std::string fenStr)
{
    //reset board position
    memset(m_bitboards, 0ULL, sizeof(m_bitboards));

    //reset occupancies (bitboards)
    memset(m_occupancies, 0ULL, sizeof(m_occupancies));

    auto fen = fenStr.begin();

    //reset game state variables
    m_side = 0;
    m_castling = 0;
    m_enpassant = no_sq;

    //loop over board ranks
    for(int rank = 0; rank < 8; ++rank)
    {
        //loop over board files
        for(int file = 0; file < 8; ++file)
        {
            //init current square
            int square = rank * 8 + file;

            //match ascii pieces within FEN string
            if((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
            {
                //init piece type
                int piece = char_pieces[*fen];

                //set piece on corresponding square
                set_bit(m_bitboards[piece], square);

                pieces[square] = new Piece(piece, square, m_renderer);

                //increment pointer to FEN string
                fen++;
            }
            //match empty square nums
            else if(*fen >= '0' && *fen <= '9')
            {
                //init offset
                int offset = *fen - '0' - 1;

                //adjust file counter
                file += offset;

                //increment pointer to FEN string
                fen++;
            }
            //match rank separator
            else if(*fen == '/')
            {
                //increment pointer to FEN string
                fen++;

                file--;
            }
        }
    }

    //go to parsing side to move
    fen++;

    //parse side to move
    *fen == 'w' ? m_side = white : m_side = black;

    //go to parsing castlim_ng rights
    fen += 2;

    //parse castling rights
    while(*fen != ' ')
    {   
        switch(*fen)
        {
            case 'K': m_castling |= wk; break;
            
            case 'Q': m_castling |= wq; break;

            case 'k': m_castling |= bk; break;

            case 'q': m_castling |= bq; break;
            
            case '-': break;
        }

        //increment pointer to FEN string
        fen++;
    }

    //go to parsing enpassant square
    fen++;

    //parse enpassant sqaure
    if(*fen != '-')
    {
        int file = fen[0] - 'a';
        int rank = 8 - (fen[1] - '0');

        //init enapssant square
        m_enpassant = rank * 8 + file;
    }
    else
    {
        m_enpassant = no_sq;
    }

    //loop oberr white pieces bitboards
    for(int piece = P; piece <= K; ++piece)
    {
        //populate white occupancy bitboard
        m_occupancies[white] |= m_bitboards[piece];
    }

    //loop oberr black pieces bitboards
    for(int piece = p; piece <= k; ++piece)
    {
        //populate white occupancy bitboard
        m_occupancies[black] |= m_bitboards[piece];
    }

    //init all ocupancies
    m_occupancies[both] |= m_occupancies[white];
    m_occupancies[both] |= m_occupancies[black];
}

U64 Position::getBitboard(int piece)
{
    return m_bitboards[piece];
}

U64 Position::getOccupancy(int side)
{
    return m_occupancies[side];
}

U64 *Position::getBitboardPtr()
{
    return m_bitboards;
}

U64 *Position::getOccupancyPtr()
{
    return m_occupancies;
}

int Position::getSide()
{
    return m_side;
}

int Position::getCastling()
{
    return m_castling;
}

int Position::getEnpassant()
{
    return m_enpassant;
}

int Position::getPiece(char c)
{
    return char_pieces[c];
}

Piece *Position::getPiecePtr(int square)
{
    if(square >= 0 && square < 64)
    {
        return pieces[square];
    }
    else
    {
        throw std::domain_error("Could not get piece. Square is out of bounds!\n");
    }   
}

Piece *Position::getPiecePtr(int rank, int file)
{
    int sqaure = rank * 8 + file;
    return getPiecePtr(sqaure);
}

void Position::setBitboard(int piece, U64 bitboard)
{
    m_bitboards[piece] = bitboard;
}

void Position::setOccupancy(int side, U64 occupancy)
{
    m_occupancies[side] = occupancy;
}

void Position::setSide(int side)
{
    m_side = side;
}

void Position::setCastling(int castling)
{
    m_castling = castling;
}

void Position::setEnpassant(int square)
{
    m_enpassant = square;
}

void Position::setPiece(int square, Piece * piece)
{
    pieces[square] = piece;
}

void Position::update()
{
    // U64 bitboard;

    // for(int piece = P; piece <= k; ++piece)
    // {
    //     bitboard = m_bitboards[piece];

    //     while(bitboard)
    //     {
    //         //get piece's position
    //         int square = get_LSB(bitboard);

    //         //remove bit
    //         remove_bit(bitboard, square);
    //     }
    // }

    for(int i = 0; i < 64; ++i)
    {
        if(pieces[i] != nullptr)
        {
            pieces[i]->update();
        }
    }
}

void Position::render()
{
    for(int i = 0; i < 64; ++i)
    {
        if(pieces[i] != nullptr)
        {
            pieces[i]->render();
        }
    }
}
