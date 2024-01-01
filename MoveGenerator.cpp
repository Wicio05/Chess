#include "MoveGenerator.hpp"

MoveGenerator::MoveGenerator(Position *position) : m_position(position)
{ 
    initAll();
}

MoveGenerator::~MoveGenerator()
{
}

U64 MoveGenerator::maskPawnAttacks(int side, int square)
{
    //result attacks bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set piece on board
    set_bit(bitboard, square);

    //white pawns
    if(!side)
    {
        //generate pawn attacks
        if((bitboard >> 7) & NOT_A_FILE) attacks |= (bitboard >> 7);
        if((bitboard >> 9) & NOT_H_FILE) attacks |= (bitboard >> 9);
    }
    //black pawns
    else
    {
        //generate pawn attacks
        if((bitboard << 7) & NOT_H_FILE) attacks |= (bitboard << 7);
        if((bitboard << 9) & NOT_A_FILE) attacks |= (bitboard << 9);
    }

    //resturn attack map
    return attacks;
}

U64 MoveGenerator::maskKnightAttacks(int square)
{
    //result attacks bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set piece on board
    set_bit(bitboard, square);

    //generate knight attacks
    if((bitboard >> 17) & NOT_H_FILE) attacks |= (bitboard >> 17);
    if((bitboard >> 15) & NOT_A_FILE) attacks |= (bitboard >> 15);
    if((bitboard >> 10) & NOT_HG_FILE) attacks |= (bitboard >> 10);
    if((bitboard >> 6) & NOT_AB_FILE) attacks |= (bitboard >> 6);

    if((bitboard << 17) & NOT_A_FILE) attacks |= (bitboard << 17);
    if((bitboard << 15) & NOT_H_FILE) attacks |= (bitboard << 15);
    if((bitboard << 10) & NOT_AB_FILE) attacks |= (bitboard << 10);
    if((bitboard << 6) & NOT_HG_FILE) attacks |= (bitboard << 6);

    //resturn attack map
    return attacks;
}

U64 MoveGenerator::maskKingAttacks(int square)
{
    //result attacks bitboard
    U64 attacks = 0ULL;

    //piece bitboard
    U64 bitboard = 0ULL;

    //set piece on board
    set_bit(bitboard, square);

    //generate king attacks
    if(bitboard >> 8) attacks |= (bitboard >> 8);
    if((bitboard >> 1) & NOT_H_FILE) attacks |= (bitboard >> 1);
    if((bitboard >> 9) & NOT_H_FILE) attacks |= (bitboard >> 9);
    if((bitboard >> 7) & NOT_A_FILE) attacks |= (bitboard >> 7);

    if((bitboard << 8)) attacks |= (bitboard << 8);
    if((bitboard << 1) & NOT_A_FILE) attacks |= (bitboard << 1);
    if((bitboard << 9) & NOT_A_FILE) attacks |= (bitboard << 9);
    if((bitboard << 7) & NOT_H_FILE) attacks |= (bitboard << 7);

    //resturn attack map
    return attacks;
}

U64 MoveGenerator::maskBishopAttacks(int square)
{
    //result attacks bitboard
    U64 attacks = 0ULL;

    //init ranks & files
    int rank, file;

    //inti target rank & file
    int tr = square / 8;
    int tf = square % 8;

    //mask relevant bishop occupancy bits
    for(rank = tr + 1, file = tf + 1; rank <= 6 && file <= 6; ++rank, ++file)
    {
        attacks |= (1ULL << (rank * 8 + file));
    }

    for(rank = tr - 1, file = tf + 1; rank >= 1 && file <= 6; --rank, ++file)
    {
        attacks |= (1ULL << (rank * 8 + file));
    }

    for(rank = tr + 1, file = tf - 1; rank <= 6 && file >= 1; ++rank, --file)
    {
        attacks |= (1ULL << (rank * 8 + file));
    }

    for(rank = tr - 1, file = tf - 1; rank >= 1 && file >= 1; --rank, --file)
    {
        attacks |= (1ULL << (rank * 8 + file));
    }

    //resturn attack map
    return attacks;
}

U64 MoveGenerator::bishopAttacksOnTheFly(int square, U64 blocks)
{
    //result attacks bitboard
    U64 attacks = 0ULL;

    //init ranks & files
    int rank, file;

    //inti target rank & file
    int tr = square / 8;
    int tf = square % 8;

    //mask relevant bishop occupancy bits
    for(rank = tr + 1, file = tf + 1; rank <= 7 && file <= 7; ++rank, ++file)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if(1ULL << (rank * 8 + file) & blocks) break;
    }

    for(rank = tr - 1, file = tf + 1; rank >= 0 && file <= 7; --rank, ++file)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if(1ULL << (rank * 8 + file) & blocks) break;
    }

    for(rank = tr + 1, file = tf - 1; rank <= 7 && file >= 0; ++rank, --file)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if(1ULL << (rank * 8 + file) & blocks) break;
    }

    for(rank = tr - 1, file = tf - 1; rank >= 0 && file >= 0; --rank, --file)
    {
        attacks |= (1ULL << (rank * 8 + file));
        if(1ULL << (rank * 8 + file) & blocks) break;
    }

    //resturn attack map
    return attacks;
}

U64 MoveGenerator::maskRookAttacks(int square)
{
    //result attacks bitboard
    U64 attacks = 0ULL;

    //init ranks & files
    int rank, file;

    //inti target rank & file
    int tr = square / 8;
    int tf = square % 8;

    //mask relevant rook occupancy bits
    for(rank = tr + 1; rank <= 6; ++rank)
    {
        attacks |= (1ULL << (rank * 8 + tf));
    }

    for(rank = tr - 1; rank >= 1; --rank)
    {
        attacks |= (1ULL << (rank * 8 + tf));
    }

    for(file = tf + 1; file <= 6; ++file)
    {
        attacks |= (1ULL << (tr * 8 + file));
    }

    for(file = tf - 1; file >= 1; --file)
    {
        attacks |= (1ULL << (tr * 8 + file));
    }

    //resturn attack map
    return attacks;
}

U64 MoveGenerator::rookAttacksOnTheFly(int square, U64 blocks)
{
    //result attacks bitboard
    U64 attacks = 0ULL;

    //init ranks & files
    int r, f;

    //inti target rank & file
    int tr = square / 8;
    int tf = square % 8;

    //mask relevant rook occupancy bits
    for(r = tr + 1; r <= 7; ++r)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if((1ULL << (r * 8 + tf)) & blocks) break;
    }

    for(r = tr - 1; r >= 0; --r)
    {
        attacks |= (1ULL << (r * 8 + tf));
        if((1ULL << (r * 8 + tf)) & blocks) break;
    }

    for(f = tf + 1; f <= 7; ++f)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if((1ULL << (tr * 8 + f)) & blocks) break;
    }

    for(f = tf - 1; f >= 0; --f)
    {
        attacks |= (1ULL << (tr * 8 + f));
        if((1ULL << (tr * 8 + f)) & blocks) break;
    }

    //resturn attack map
    return attacks;
}

U64 MoveGenerator::setOccupancy(int index, int bits_in_mask, U64 attack_mask)
{
    //occupancy map
    U64 occupancy = 0ULL;

    //loop over the range of bits within attack mask
    for(int i = 0; i < bits_in_mask; ++i)
    {
        //get LSB index of atack mask
        int square = get_LSB(attack_mask);

        //remove LSB in attack map
        remove_bit(attack_mask, square);

        //make sure occupancy is on board
        if(index & (1 << i))
            //populate occupancy
            occupancy |= (1ULL << square);
    }

    //return occupancy map
    return occupancy;
}

void MoveGenerator::initLeapersAttacks()
{
    //loop over the board
    for(int square = 0; square < 64; ++square)
    {
        //inti pawn attacks
        pawn_attacks[0][square] = maskPawnAttacks(0, square);
        pawn_attacks[1][square] = maskPawnAttacks(1, square);

        //init knight attacks
        knight_attacks[square] = maskKnightAttacks(square);

        //init king attacks
        king_attacks[square] = maskKingAttacks(square);
    }
}

void MoveGenerator::initSlidersAttacks(bool is_bishop)
{
    //loop ober 64 board squares
    for(int square = 0; square < 64; ++square)
    {
        //init bishop & rook masks
        bishop_masks[square] = maskBishopAttacks(square);
        rook_masks[square] = maskRookAttacks(square);

        //init current mask
        U64 attack_mask = is_bishop ? bishop_masks[square] : rook_masks[square];

        //init relevant bit count
        int relevant_bits_count = count_bits(attack_mask);

        //init occupancy indicies
        int occupacy_indicies = (1 << relevant_bits_count);

        //loop over occupany indicies
        for(int index = 0; index < occupacy_indicies; ++index)
        {
            //bishop
            if(is_bishop)
            {
                //init current occupancy bariation
                U64 occupacy = setOccupancy(index, relevant_bits_count, attack_mask);

                //init magic index
                int magic_index = (occupacy * bishop_magic_numbers[square]) >> (64 - bishop_relevant_bits[square]);
            
                //init bishop attacks
                bishop_attacks[square][magic_index] = bishopAttacksOnTheFly(square, occupacy);
            }
            //rook
            else
            {
                //init current occupancy bariation
                U64 occupacy = setOccupancy(index, relevant_bits_count, attack_mask);

                //init magic index
                int magic_index = (occupacy * rook_magic_numbers[square]) >> (64 - rook_relevant_bits[square]);
            
                //init bishop attacks
                rook_attacks[square][magic_index] = rookAttacksOnTheFly(square, occupacy);
            }
        }
    }
}

void MoveGenerator::initAll()
{
    initLeapersAttacks();

    initSlidersAttacks(bishop);

    initSlidersAttacks(rook);
}

U64 MoveGenerator::getBishopAttacks(int square, U64 occupancy)
{
    //get bishop attacks on current board occupancy
    occupancy &= bishop_masks[square];
    occupancy *= bishop_magic_numbers[square];
    occupancy >>= 64 - bishop_relevant_bits[square];

    //return bishop attacks
    return bishop_attacks[square][occupancy];
}

U64 MoveGenerator::getRookAttacks(int square, U64 occupancy)
{
    //get rook attacks on current board occupancy
    occupancy &= rook_masks[square];
    occupancy *= rook_magic_numbers[square];
    occupancy >>= 64 - rook_relevant_bits[square];

    //return rook attacks
    return rook_attacks[square][occupancy];
}

U64 MoveGenerator::getQueenAttacks(int square, U64 occupancy)
{
    //init result attacks bitboard
    U64 queen_attacks = 0ULL;

    //init bishop occupancies
    U64 bishop_occupancy = occupancy;

    //init rook occupancies
    U64 rook_occupancy = occupancy;

    //get bishop attacks on current board occupancy
    bishop_occupancy &= bishop_masks[square];
    bishop_occupancy *= bishop_magic_numbers[square];
    bishop_occupancy >>= 64 - bishop_relevant_bits[square];

    //get bishop attacks
    queen_attacks = bishop_attacks[square][bishop_occupancy];

    //get rook attacks on current board occupancy
    rook_occupancy &= rook_masks[square];
    rook_occupancy *= rook_magic_numbers[square];
    rook_occupancy >>= 64 - rook_relevant_bits[square];

    //get rook attacks
    queen_attacks |= rook_attacks[square][rook_occupancy];

    //return queen attacks
    return queen_attacks;
}

bool MoveGenerator::isSquareAttacked(int square, int side)
{
    //attacked by white pawns
    if(side == white && 
      (pawn_attacks[black][square] & m_position->getBitboard(P)))
    {
        return true;
    }

    //attacked by black pawns
    if(side == black && 
      (pawn_attacks[white][square] & m_position->getBitboard(p)))
    {
        return true;
    }

    //attacked by knights
    if(knight_attacks[square] & 
      (side == white ? m_position->getBitboard(N) : m_position->getBitboard(n)))
    {
        return true;
    }

    //attacked by kings
    if(king_attacks[square] & 
      (side == white ? m_position->getBitboard(K) : m_position->getBitboard(k)))
    {
        return true;
    }

    //attacked by bishops
    if(getBishopAttacks(square, m_position->getOccupancy(both)) & 
      (side == white ? m_position->getBitboard(B) : m_position->getBitboard(b)))
    {
        return true;
    }

    //attacked by rooks
    if(getRookAttacks(square, m_position->getOccupancy(both)) & 
      (side == white ? m_position->getBitboard(R) : m_position->getBitboard(r)))
    {
        return true;
    }

    //attacked by queen
    if(getQueenAttacks(square, m_position->getOccupancy(both)) & 
      (side == white ? m_position->getBitboard(Q) : m_position->getBitboard(q)))
    {
        return true;
    }

    //by default return false
    return false;
}

void MoveGenerator::generateAllMoves(Moves *move_list)
{
    //init source & target squares
    int src_square, trg_square;

    //init current piece's bitboard copy & it's attacks
    U64 bitboard, attacks;

    int piece, e_piece;
    ((m_position->getSide() == white) ? (piece = P, e_piece = K) : (piece = p, e_piece = k));

    // loop over all the bitboards
    for(piece; piece <= e_piece; ++piece)
    {
        //init piece bitboard copy
        bitboard = m_position->getBitboard(piece);

        switch(piece)
        {
            case P:
            {
                //loop over white pawns on the white pawn bitboard
                while(bitboard)
                {
                    //define src square
                    src_square = get_LSB(bitboard);

                    //define target square
                    trg_square = src_square - 8;

                    //generate quite pawn moves
                    if(!(trg_square < 0) && 
                       !get_bit(m_position->getOccupancy(both), trg_square))
                    {
                        //pawn promotion
                        if(RANK7 & (1ULL << src_square))
                        {
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 4), 0, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 3), 0, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 2), 0, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 1), 0, 0, 0, 0));
                        }
                        else
                        {
                            move_list->addMove(encode_move(src_square, trg_square, piece, 0, 0, 0, 0, 0));

                            //two square pawn move
                            if(RANK2 & (1ULL << src_square) && 
                               !get_bit(m_position->getOccupancy(both), (trg_square - 8)))
                            {
                                move_list->addMove(encode_move(src_square, (trg_square - 8), piece, 0, 0, 1, 0, 0));
                            }
                        }
                    }

                    //define pawn attacks
                    attacks = pawn_attacks[white][src_square] & m_position->getOccupancy(black);
                
                    //generate pawn captures
                    while(attacks)
                    {
                        //define target square
                        trg_square = get_LSB(attacks);

                        //pawn promotion
                        if(RANK7 & (1ULL << src_square))
                        {
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 4), 1, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 3), 1, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 2), 1, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 1), 1, 0, 0, 0));
                        }
                        else
                        {
                            move_list->addMove(encode_move(src_square, trg_square, piece, 0, 1, 0, 0, 0));
                        }

                        //remove lsb from bitboard copy
                        remove_bit(attacks, trg_square);
                    }

                    //check for enpassant captures
                    if(m_position->getEnpassant() != no_sq)
                    {
                        //lookup pawn attacks & bitwise & with enpassant square (bit)
                        attacks = pawn_attacks[white][src_square] & (1ULL << m_position->getEnpassant());
                    
                        //make sure enpasant capture is available
                        if(attacks)
                        {
                            //define enpassant trg_square
                            trg_square = get_LSB(attacks);

                            move_list->addMove(encode_move(src_square, trg_square, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    //remove lsb from bitboard copy
                    remove_bit(bitboard, src_square);
                }

                break;
            }

            case K:
            {
                //check king side castling right
                if(m_position->getCastling() & wk)
                {
                    //check if castling move is possible
                    if(!get_bit(m_position->getOccupancy(both), f1) && 
                       !get_bit(m_position->getOccupancy(both), g1) &&
                       !isSquareAttacked(e1, black) &&
                       !isSquareAttacked(f1, black))
                    {
                        move_list->addMove(encode_move(e1, g1, K, 0, 0, 0, 0, 1));
                    }
                }

                //check queen side castling right
                if(m_position->getCastling() & wq)
                {
                    if(!get_bit(m_position->getOccupancy(both), d1) && 
                       !get_bit(m_position->getOccupancy(both), c1) && 
                       !get_bit(m_position->getOccupancy(both), b1) && 
                       !isSquareAttacked(e1, black) && 
                       !isSquareAttacked(d1, black))
                    {
                        move_list->addMove(encode_move(e1, c1, K, 0, 0, 0, 0, 1));
                    }
                }

                break;
            }

            case p:
            {
                //loop over white pawns on the white pawn bitboard
                while(bitboard)
                {
                    //define src square
                    src_square = get_LSB(bitboard);

                    //define target square
                    trg_square = src_square + 8;

                    //generate quite pawn moves
                    if(!(trg_square > h1) && 
                       !get_bit(m_position->getOccupancy(both), trg_square))
                    {
                        //pawn promotion
                        if(RANK2 & (1ULL << src_square))
                        {
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 4), 0, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 3), 0, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 2), 0, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 1), 0, 0, 0, 0));
                        }
                        else
                        {
                            move_list->addMove(encode_move(src_square, trg_square, piece, 0, 0, 0, 0, 0));

                            if(RANK7 & (1ULL << src_square) && 
                               !get_bit(m_position->getOccupancy(both), (trg_square + 8)))
                            {
                                move_list->addMove(encode_move(src_square, (trg_square + 8), piece, 0, 0, 1, 0, 0));
                            }
                        }
                    }

                    //define pawn attacks
                    attacks = pawn_attacks[black][src_square] & m_position->getOccupancy(white);

                    //generate pawn captures
                    while(attacks)
                    {
                        //define target square
                        trg_square = get_LSB(attacks);

                        //pawn promotion
                        if(RANK2 & (1ULL << src_square))
                        {
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 4), 1, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 3), 1, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 2), 1, 0, 0, 0));
                            move_list->addMove(encode_move(src_square, trg_square, piece, (piece + 1), 1, 0, 0, 0));
                        }
                        else
                        {
                            move_list->addMove(encode_move(src_square, trg_square, piece, 0, 1, 0, 0, 0));
                        }

                        //remove lsb from bitboard copy
                        remove_bit(attacks, trg_square);
                    }

                    //check for enpassant captures
                    if(m_position->getEnpassant() != no_sq)
                    {
                        //lookup pawn attacks & bitwise & with enpassant square (bit)
                        attacks = pawn_attacks[black][src_square] & (1ULL << m_position->getEnpassant());

                        //make sure enpasant capture is available
                        if(attacks)
                        {
                            //define enpassant trg_square
                            trg_square = get_LSB(attacks);
                            // std::cout << " pawn enpassant capture: " << square_to_coord[src_square] << square_to_coord[trg_square] << "\n";
                        
                            move_list->addMove(encode_move(src_square, trg_square, piece, 0, 1, 0, 1, 0));
                        }
                    }

                    //remove lsb from bitboard copy
                    remove_bit(bitboard, src_square);
                }

                break;
            }

            case k:
            {
                //check king side castling right
                if(m_position->getCastling() & bk)
                {
                    if(!get_bit(m_position->getOccupancy(both), f8) && 
                       !get_bit(m_position->getOccupancy(both), g8) &&
                       !isSquareAttacked(e8, white) && //not sure if nessesery
                       !isSquareAttacked(f8, white))
                    {
                        move_list->addMove(encode_move(e8, g8, k, 0, 0, 0, 0, 1));
                    }
                }

                //check queen side castling right
                if(m_position->getCastling() & bq)
                {
                    if(!get_bit(m_position->getOccupancy(both), d8) && 
                       !get_bit(m_position->getOccupancy(both), c8) && 
                       !get_bit(m_position->getOccupancy(both), b8) && 
                       !isSquareAttacked(e8, white) && //not sure if nessesery
                       !isSquareAttacked(d8, white))
                    {
                        move_list->addMove(encode_move(e8, c8, k, 0, 0, 0, 0, 1));
                    }
                }

                break;
            }
        }

        int side = m_position->getSide();
    
        //generate knight moves
        if(side == white ? piece == N : piece == n)
        {
            while(bitboard)
            {
                //def src square
                src_square = get_LSB(bitboard);

                //def attacks
                attacks = knight_attacks[src_square] & ~(m_position->getOccupancy(side));

                //loop over target squares
                while(attacks)
                {
                    //def trg square
                    trg_square = get_LSB(attacks);

                    //quiet move
                    if(!get_bit((side == white ? m_position->getOccupancy(black) : m_position->getOccupancy(white)), trg_square))
                    {
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 0, 0, 0, 0));
                    }
                    //capture move
                    else
                    {
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 1, 0, 0, 0));
                    }

                    //remove bit
                    remove_bit(attacks, trg_square);
                }

                //remove LSB
                remove_bit(bitboard, src_square);
            }
        }

        //generate bishop moves
        if(side == white ? piece == B : piece == b)
        {
            while(bitboard)
            {
                //def src square
                src_square = get_LSB(bitboard);

                //def attacks
                attacks = getBishopAttacks(src_square, m_position->getOccupancy(both)) & ~(m_position->getOccupancy(side));

                //loop over target squares
                while(attacks)
                {
                    //def trg square
                    trg_square = get_LSB(attacks);

                    //quiet move
                    if(!get_bit((side == white ? m_position->getOccupancy(black) : m_position->getOccupancy(white)), trg_square))
                    {
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 0, 0, 0, 0));
                    }
                    //capture move
                    else
                    {
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 1, 0, 0, 0));
                    }

                    //remove bit
                    remove_bit(attacks, trg_square);
                }

                //remove LSB
                remove_bit(bitboard, src_square);
            }
        }

        //generate rook moves
        if(side == white ? piece == R : piece == r)
        {
            while(bitboard)
            {
                //def src square
                src_square = get_LSB(bitboard);

                //def attacks
                attacks = getRookAttacks(src_square, m_position->getOccupancy(both)) & ~(m_position->getOccupancy(side));

                //loop over target squares
                while(attacks)
                {
                    //def trg square
                    trg_square = get_LSB(attacks);

                    //quiet move
                    if(!get_bit((side == white ? m_position->getOccupancy(black) : m_position->getOccupancy(white)), trg_square))
                    {
                        // std::cout << " rook move: " << square_to_coord[src_square] << square_to_coord[trg_square] << "\n";
                    
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 0, 0, 0, 0));
                    }
                    //capture move
                    else
                    {
                        // std::cout << " rook capture: " << square_to_coord[src_square] << square_to_coord[trg_square] << "\n";
                    
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 1, 0, 0, 0));
                    }

                    //remove bit
                    remove_bit(attacks, trg_square);
                }

                //remove LSB
                remove_bit(bitboard, src_square);
            }
        }

        //generate queen moves
        if(side == white ? piece == Q : piece == q)
        {
            while(bitboard)
            {
                //def src square
                src_square = get_LSB(bitboard);

                //def attacks
                attacks = getQueenAttacks(src_square, m_position->getOccupancy(both)) & ~(m_position->getOccupancy(side));

                //loop over target squares
                while(attacks)
                {
                    //def trg square
                    trg_square = get_LSB(attacks);

                    //quiet move
                    if(!get_bit((side == white ? m_position->getOccupancy(black) : m_position->getOccupancy(white)), trg_square))
                    {
                        // std::cout << " queen move: " << square_to_coord[src_square] << square_to_coord[trg_square] << "\n";
                    
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 0, 0, 0, 0));
                    }
                    //capture move
                    else
                    {
                        // std::cout << " queen capture: " << square_to_coord[src_square] << square_to_coord[trg_square] << "\n";
                    
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 1, 0, 0, 0));
                    }

                    //remove bit
                    remove_bit(attacks, trg_square);
                }

                //remove LSB
                remove_bit(bitboard, src_square);
            }
        }

        //generate king moves
        if(side == white ? piece == K : piece == k)
        {
            while(bitboard)
            {
                //def src square
                src_square = get_LSB(bitboard);

                //def attacks
                attacks = king_attacks[src_square] & ~(m_position->getOccupancy(side));

                //loop over target squares
                while(attacks)
                {
                    //def trg square
                    trg_square = get_LSB(attacks);

                    //quiet move
                    if(!get_bit((side == white ? m_position->getOccupancy(black) : m_position->getOccupancy(white)), trg_square))
                    {
                        // std::cout << " king move: " << square_to_coord[src_square] << square_to_coord[trg_square] << "\n";
                    
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 0, 0, 0, 0));
                    }
                    //capture move
                    else
                    {
                        // std::cout << " king capture: " << square_to_coord[src_square] << square_to_coord[trg_square] << "\n";
                    
                        move_list->addMove(encode_move(src_square, trg_square, piece, 0, 1, 0, 0, 0));
                    }

                    //remove bit
                    remove_bit(attacks, trg_square);
                }

                //remove LSB
                remove_bit(bitboard, src_square);
            }
        }
    }
}

bool MoveGenerator::makeMove(int move, int capture_flag)
{
    int capture = get_capture(move);

    //quiet moves
    if(capture_flag == all)
    {
        // copy_board();
        U64 bitboards_copy[12], occupancies_copy[3];                            
        int side_copy, enpassant_copy, castling_copy;                           
        memcpy(bitboards_copy, m_position->getBitboardPtr(), 96);                                  
        memcpy(occupancies_copy, m_position->getOccupancyPtr(), 24);                              
        side_copy = m_position->getSide(), enpassant_copy = m_position->getEnpassant(), castling_copy = m_position->getCastling(); 


        int src_square = get_src(move);
        int trg_square = get_trg(move);
        int piece = get_piece(move);
        int promoted = get_promotion(move);
        int _double = get_double(move);
        int _enpassant = get_enpassant(move);
        int _castling = get_castling(move);

        //move piece
        // remove_bit(bitboards[piece], src_square);
        remove_bit(*(m_position->getBitboardPtr() + piece), src_square);
        // set_bit(bitboards[piece], trg_square);
        set_bit(*(m_position->getBitboardPtr() + piece), trg_square);

        //handle captures
        if(capture)
        {
            //get index
            int str_p, end_p;

            if(m_position->getSide() == white)
            {
                str_p = p, end_p = k;
            }
            else
            {
                str_p = P, end_p = K;
            }

            //loop over bitboards
            for(int bb_piece = str_p; bb_piece <= end_p; ++bb_piece)
            {
                //if piece on trg square
                if(get_bit(m_position->getBitboard(bb_piece), trg_square))
                {
                    //remove it
                    // remove_bit(bitboards[bb_piece], trg_square);
                    remove_bit(*(m_position->getBitboardPtr() + bb_piece), trg_square);
                    break;
                }
            }
        }

        //handle pwan promotions
        if(promoted)
        {
            //move piece
            // remove_bit(bitboards[piece], trg_square);
            // remove_bit(bitboards[(m_position->getSide() == white) ? P : p], trg_square);
            remove_bit(((m_position->getSide() == white) ? *(m_position->getBitboardPtr() + P) : *(m_position->getBitboardPtr() + p)), trg_square);
            // set_bit(bitboards[promoted], trg_square);
            set_bit(*(m_position->getBitboardPtr() + promoted), trg_square);
        }

        //handle enpassant
        if(_enpassant) 
        {
            // (m_position->getSide() == white) ? 
            //     remove_bit(bitboards[p], trg_square + 8) : 
            //     remove_bit(bitboards[P], trg_square - 8) ;
            (m_position->getSide() == white) ? 
                remove_bit(*(m_position->getBitboardPtr() + p), trg_square + 8) : 
                remove_bit(*(m_position->getBitboardPtr() + P), trg_square - 8) ;
        }

        //reset enpassant
        m_position->setEnpassant(no_sq);
 
        //handle double pawn move
        if(_double)
        {
            //set enpassant square
            (m_position->getSide() == white) ? (m_position->setEnpassant(trg_square + 8)) : (m_position->setEnpassant(trg_square - 8));
        }

        //handle castling
        if(_castling)
        {
            switch(trg_square)
            {
                case g1:
                    // remove_bit(bitboards[R], h1);
                    // set_bit(bitboards[R], f1);
                    remove_bit(*(m_position->getBitboardPtr() + R), h1);
                    set_bit(*(m_position->getBitboardPtr() + R), f1);
                    break;
                    
                case c1:
                    // remove_bit(bitboards[R], a1);
                    // set_bit(bitboards[R], d1);
                    remove_bit(*(m_position->getBitboardPtr() + R), a1);
                    set_bit(*(m_position->getBitboardPtr() + R), d1);
                    break;

                case g8:
                    // remove_bit(bitboards[r], h8);
                    // set_bit(bitboards[r], f8);
                    remove_bit(*(m_position->getBitboardPtr() + r), h8);
                    set_bit(*(m_position->getBitboardPtr() + r), f8);
                    break;
                    
                case c8:
                    // remove_bit(bitboards[r], a8);
                    // set_bit(bitboards[r], d8);
                    remove_bit(*(m_position->getBitboardPtr() + r), a8);
                    set_bit(*(m_position->getBitboardPtr() + r), d8);
                    break;
            }
        }

        //update castling rights
        // *(m_position->getCastlingPtr()) &= castling_rights[src_square];
        // *(m_position->getCastlingPtr()) &= castling_rights[trg_square];

        m_position->setCastling(m_position->getCastling() & castling_rights[src_square]);
        m_position->setCastling(m_position->getCastling() & castling_rights[trg_square]);

        //reset occupancies
        memset(m_position->getOccupancyPtr(), 0ULL, 24);

        //loop oberr white pieces bitboards
        for(int _piece = P; _piece <= K; ++_piece)
        {
            //populate white occupancy bitboard
            // occupancies[white] |= bitboards[_piece];

            m_position->setOccupancy(white, m_position->getOccupancy(white) | m_position->getBitboard(_piece));
        }

        //loop oberr black pieces bitboards
        for(int _piece = p; _piece <= k; ++_piece)
        {
            //populate white occupancy bitboard
            // occupancies[black] |= bitboards[_piece];

            m_position->setOccupancy(black, m_position->getOccupancy(black) | m_position->getBitboard(_piece));
        }

        //init all ocupancies
        // occupancies[both] |= occupancies[white];
        m_position->setOccupancy(both, m_position->getOccupancy(both) | m_position->getOccupancy(white));
        // occupancies[both] |= occupancies[black];
        m_position->setOccupancy(both, m_position->getOccupancy(both) | m_position->getOccupancy(black));

        //change side
        // *(m_position->getSidePtr()) ^= 1;
        m_position->setSide(m_position->getSide() ^ 1);

        //if king is in check
        // if(isSquareAttacked((m_position->getSide() == white) ? 
        //                     get_LSB(bitboards[k]) : 
        //                     get_LSB(bitboards[K]), 
        //                     m_position->getSide()))
        if(isSquareAttacked((m_position->getSide() == white) ? 
                            get_LSB(m_position->getBitboard(k)) : 
                            get_LSB(m_position->getBitboard(K)), 
                            m_position->getSide()))
        {
            // take_back();
            memcpy(m_position->getBitboardPtr(), bitboards_copy, 96);                                  
            memcpy(m_position->getOccupancyPtr(), occupancies_copy, 24);                              
            m_position->setSide(side_copy), m_position->setEnpassant(enpassant_copy), m_position->setCastling(castling_copy);

            return 0;
        }
        else
        {
            return 1;
        }
    }

    //captures move
    else
    {
        if(capture)
        {
            makeMove(move, all);
        }
        else return 0;
    }   
}

void MoveGenerator::perftDriver(int depth)
{
    if(depth == 0)
    {
        m_nodes++;
        return;
    }
    else
    {
        Moves *move_list = new Moves;

        generateAllMoves(move_list);

        for(int i = 0; i < move_list->getCount(); ++i)
        {
            // copy_board();
            U64 bitboards_copy[12], occupancies_copy[3];                            
            int side_copy, enpassant_copy, castling_copy;                           
            memcpy(bitboards_copy, m_position->getBitboardPtr(), 96);                                  
            memcpy(occupancies_copy, m_position->getOccupancyPtr(), 24);                              
            side_copy = m_position->getSide(), enpassant_copy = m_position->getEnpassant(), castling_copy = m_position->getCastling(); 

            if(!makeMove(move_list->getMove(i), all))
                continue;
            
            //call perft driver recursively
            perftDriver(depth - 1);

            // take_back();
            memcpy(m_position->getBitboardPtr(), bitboards_copy, 96);                                  
            memcpy(m_position->getOccupancyPtr(), occupancies_copy, 24);                              
            m_position->setSide(side_copy), m_position->setEnpassant(enpassant_copy), m_position->setCastling(castling_copy);
        }

        delete move_list;
    }
}

void MoveGenerator::perftTest(int depth)
{
    std::cout << "\n Performance test\n\n";
    Moves *move_list = new Moves;

    generateAllMoves(move_list);

    // int start = getTimeMs();
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

    for(int i = 0; i < move_list->getCount(); ++i)
    {
        // copy_board();
        U64 bitboards_copy[12], occupancies_copy[3];                            
        int side_copy, enpassant_copy, castling_copy;                           
        memcpy(bitboards_copy, m_position->getBitboardPtr(), 96);                                  
        memcpy(occupancies_copy, m_position->getOccupancyPtr(), 24);                              
        side_copy = m_position->getSide(), enpassant_copy = m_position->getEnpassant(), castling_copy = m_position->getCastling(); 

        if(!makeMove(move_list->getMove(i), all))
            continue;

        U64 cummulative_nodes = m_nodes;
        
        //call perft driver recursively
        perftDriver(depth - 1);

        U64 old_nodes = m_nodes - cummulative_nodes;

        // take_back();
        memcpy(m_position->getBitboardPtr(), bitboards_copy, 96);                                  
        memcpy(m_position->getOccupancyPtr(), occupancies_copy, 24);                              
        m_position->setSide(side_copy), m_position->setEnpassant(enpassant_copy), m_position->setCastling(castling_copy);

        // printMove(move_list.moves[i]);
        // std::cout << " " << square_to_coord[get_src(move_list.moves[i])] << square_to_coord[get_trg(move_list.moves[i])] << (get_promotion(move_list.moves[i]) ? promotion_pieces[get_promotion(move_list.moves[i])] : ' ') << "\n";
        std::cout << " " << square_to_coord[get_src(move_list->getMove(i))] << square_to_coord[get_trg(move_list->getMove(i))] << promotion_pieces[get_promotion(move_list->getMove(i))] << "\tNodes: " << old_nodes << "\n";
    }

    delete move_list;

    // int end = getTimeMs();
    std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = end - start; 

    std::cout << "\n Depth: " << depth << "\n";
    std::cout << " Nodes: " << m_nodes << "\n";
    std::cout << " Time taken to execute: " << fp_ms.count() << "ms\n";
}

void MoveGenerator::printMove(int move)
{
    int src = get_src(move);
    int trg = get_trg(move);
    int piece = get_piece(move);
    int promoted = get_promotion(move);
    // int capture = get_capture(move);
    // int double_ = get_double(move);
    // int enpassant = get_enpassant(move);
    // int castling = get_castling(move);

    std::cout << square_to_coord[src] << square_to_coord[trg] << promotion_pieces[promoted] << "";
}

U64 MoveGenerator::getNodes()
{
    return m_nodes;
}

void MoveGenerator::setNodes(U64 val)
{
    m_nodes = val;
}
