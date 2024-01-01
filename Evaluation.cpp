#include "Evaluation.hpp"

Evaluation::Evaluation(Position* position, MoveGenerator* moveGenerator) : m_position(position), m_moveGen(moveGenerator)
{
}

Evaluation::~Evaluation()
{
}

int Evaluation::parseMove(std::string moveStr)
{
    //create movelist instance
    Moves *move_list = new Moves;

    //generate moves
    m_moveGen->generateAllMoves(move_list);

    //parse source square
    int src_square = moveStr[0] - 'a' + 8 * (8 - moveStr[1] + '0');

    //parse target square
    int trg_square = moveStr[2] - 'a' + 8 * (8 - moveStr[3] + '0');

    // init promoted piece
    int promoted_piece = ((moveStr.length() > 4 || moveStr[4] != ' ') ? m_position->getPiece(moveStr[4]) : 0);

    // std::cout << "src: " << square_to_coord[src_square] << "\n";

    // std::cout << "trg: " << square_to_coord[trg_square] << "\n";

    // std::cout << "prom piece: " << ascii_pieces[promoted_piece] << "\n";

    //loop over the moves within a move list
    for(int move_count = 0; move_count < move_list->getCount(); ++move_count)
    {
        //init move
        int move = move_list->getMove(move_count);

        int check_prom = get_promotion(move);

        //check if src & trg squares are in the generated moves list & check if promoted piece match (for both sides)
        if(src_square == get_src(move) && trg_square == get_trg(move) && (promoted_piece == check_prom || promoted_piece == check_prom + 6))
        {
            //legal move
            delete move_list;
            return move;
        }
    }

    //return illegal move
    delete move_list;
    return 0;
}

void Evaluation::parsePosition(std::string pos)
{
    //shift the string for the commands
    pos.assign(pos.begin() + 9, pos.end());

    // std::cout << pos << "\n";

    if(pos.find("startpos") != std::string::npos)
    {
        m_position->parseFEN(starting_position);
    }
    else if(pos.find("fen") != std::string::npos)
    {
        //shift the string for the FEN string
        pos.assign(pos.begin() + 4, pos.end());

        m_position->parseFEN(pos.c_str());
    }

    int find_moves = pos.find("moves");

    if(find_moves != std::string::npos)
    {
        pos.assign(pos.begin() + find_moves, pos.end());
            
        //stop condition
        bool stop_con = (pos != "");

        while(stop_con)
        {
            int idx = pos.find(' ');

            //shift the string for the moves
            if(idx != std::string::npos)
            {
                pos.assign(pos.begin() + idx + 1, pos.end());
            }
            else
            {
                stop_con = false;
            }

            int move = parseMove(pos);

            if(!move)
            {
                break;
            }

            m_moveGen->makeMove(move, all);
        }
    }

    // m_position->printBoard();
}

void Evaluation::parseGo(std::string command)
{
    //init depth
    int depth = -1;

    //find the indedx of the beginning of the keyword depth
    int depth_idx = command.find("depth");

    // check if the keyword depth exists in command
    if(depth_idx != std::string::npos)
    {
        //get the depth
        command.assign(command.begin() + depth_idx + 6, command.end());
        depth = std::stoi(command);
    }
    //different time cotrols (blitz games etr.)
    else
    {
        depth = 6;
    }

    //search position
    searchPosition(depth);
}

void Evaluation::uciLoop()
{
    //reset STDIN & STDOUT buffers
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    //define user / GUI input buffer
    std::string input = "";
    // char* input[2000];

    //print engine ifo
    std::cout << "id name Witek\n";
    std::cout << "uciok\n";

    //UCI loop
    while(true)
    {
        // memset(input, 0, sizeof(input));
        fflush(stdout);

        // if(!fgets(input, 2000, stdin))
        // {
        //     continue;
        // }
        
        // std::cin >> input;
        getline(std::cin, input);

        if(input[0] == '\n')
        {
            continue;
        }

        //parse UCI "isready" command
        if(input.find("isready") != std::string::npos)
        {
            std::cout << "readyok\n";
        }
        //parse UCI "position" command
        else if(input.find("position") != std::string::npos)
        {
            parsePosition(input);
        }
        else if(input.find("ucinewgame") != std::string::npos)
        {
            parsePosition("position startpos");
        }
        else if(input.find("go") != std::string::npos)
        {
            parseGo(input);
        }
        else if(input.find("quit") != std::string::npos)
        {
            break;
        }
        else if(input.find("uci") != std::string::npos)
        {
            std::cout << "id name Witek\n";
            std::cout << "uciok\n";
        }
    }
}

int Evaluation::go(int depth)
{
    //init score
    int score = 0;

    //reset data
    nodes = 0;
    fol_pv = 0;
    score_pv = 0;

    memset(killer_moves, 0, sizeof(killer_moves));
    memset(history_moves, 0, sizeof(history_moves));
    memset(pv_table, 0, sizeof(pv_table));
    memset(pv_length, 0, sizeof(pv_length));

    //inerative deepening
    for(int curr_depth = 1; curr_depth <= depth; ++curr_depth)
    {
        fol_pv = 1;

        //find best move on given position
        score = negamax(-50000, 50000, curr_depth);
        std::cout << "info score cp " << score << " depth " << curr_depth << " nodes " << nodes << " ";

        //loop over moves in the PV line
        for(int i = 0; i < pv_length[0]; ++i)
        {
            m_moveGen->printMove(pv_table[0][i]);
        }

        std::cout << "\n";
    }

    std::cout << "bestmove ";
    m_moveGen->printMove(pv_table[0][0]);
    std::cout << "\n";

    return pv_table[0][0];
}

void Evaluation::searchPosition(int depth)
{
    //init score
    int score = 0;

    //reset data
    nodes = 0;
    fol_pv = 0;
    score_pv = 0;

    memset(killer_moves, 0, sizeof(killer_moves));
    memset(history_moves, 0, sizeof(history_moves));
    memset(pv_table, 0, sizeof(pv_table));
    memset(pv_length, 0, sizeof(pv_length));

    //inerative deepening
    for(int curr_depth = 1; curr_depth <= depth; ++curr_depth)
    {
        fol_pv = 1;

        //find best move on given position
        score = negamax(-50000, 50000, curr_depth);
        std::cout << "info score cp " << score << " depth " << curr_depth << " nodes " << nodes << " ";

        //loop over moves in the PV line
        for(int i = 0; i < pv_length[0]; ++i)
        {
            m_moveGen->printMove(pv_table[0][i]);
        }

        std::cout << "\n";
    }

    std::cout << "bestmove ";
    m_moveGen->printMove(pv_table[0][0]);
    std::cout << "\n";
}

int Evaluation::evaluate()
{
    //init score
    int score = 0;

    //current board state
    U64 bitboard;

    //init piece & square
    int piece, square;

    //loop over piece bitboards
    for(int bb_piece = P; bb_piece <= k; ++bb_piece)
    {
        //init piece bitboard
        bitboard = m_position->getBitboard(bb_piece);

        while(bitboard)
        {
            piece = bb_piece;

            square = get_LSB(bitboard);

            //material score
            score += material_score[piece];

            //positional score
            switch(piece)
            {
                case P: score += pawn_score[square];    break;
                case N: score += knight_score[square];  break;
                case B: score += bishop_score[square];  break;
                case R: score += rook_score[square];    break;
                // case Q: score += queen_score[square];   break;
                case K: score += king_score[square];    break;

                case p: score -= pawn_score[mirror_score[square]];    break;
                case n: score -= knight_score[mirror_score[square]];  break;
                case b: score -= bishop_score[mirror_score[square]];  break;
                case r: score -= rook_score[mirror_score[square]];    break;
                // case q: score -= queen_score[mirror_score[square]];   break;
                case k: score -= king_score[mirror_score[square]];    break;
            }

            // remove_bit(bitboard, square);
            pop_LSB(bitboard);
        }
    }

    //return final eval based on side
    return (m_position->getSide() == white ? score : -score);
}

int Evaluation::negamax(int alpha, int beta, int depth)
{
    //init PV length
    pv_length[ply] = ply;

    //escape condition
    if(depth == 0)
    {
        //run quiescence search
        return quiescence(alpha, beta);
    }

    //escape codeition (max ply reached)
    if(ply >= max_ply)
    {
        return evaluate();
    }   

    //increment node count
    ++nodes;

    //is king in check
    bool in_check = m_moveGen->isSquareAttacked(
        (m_position->getSide() == white) ? get_LSB(m_position->getBitboard(K)) : get_LSB(m_position->getBitboard(k)), (m_position->getSide() ^ 1));

    //increase search depth if king is in check
    if(in_check)
    {
        ++depth;
    }

    //legal move counter
    int legal_moves = 0;

    //null move pruning
    if(depth >= 3 && in_check == 0 && ply)
    {
        //copy board state
        // copy_board();
        U64 bitboards_copy[12], occupancies_copy[3];                            
        int side_copy, enpassant_copy, castling_copy;                           
        memcpy(bitboards_copy, m_position->getBitboardPtr(), 96);                                  
        memcpy(occupancies_copy, m_position->getOccupancyPtr(), 24);                              
        side_copy = m_position->getSide(), enpassant_copy = m_position->getEnpassant(), castling_copy = m_position->getCastling(); 

        //switch side (give an opponent an extra move)
        m_position->setSide(m_position->getSide() ^ 1);

        //reset enpassant square
        m_position->setEnpassant(no_sq);

        //search moves with reduced depth to find beta cutoffs
        int score = -negamax(-beta, -beta + 1, depth - 3);

        //restore the boared state
        // take_back();
        memcpy(m_position->getBitboardPtr(), bitboards_copy, 96);                                  
        memcpy(m_position->getOccupancyPtr(), occupancies_copy, 24);                              
        m_position->setSide(side_copy), m_position->setEnpassant(enpassant_copy), m_position->setCastling(castling_copy);

        //beta cutoff fails-hard
        if(score >= beta)
        {
            return beta;
        }
    }

    //create move list
    Moves *movelist = new Moves;

    //generate moves
    m_moveGen->generateAllMoves(movelist);

    //following PV line
    if(fol_pv)
    {
        pvScoring(movelist);
    }

    //sort moves
    sortMoves(movelist);

    // number of moves searched in a move list
    int movesSearched = 0;

    //loop over the moves
    for(int i = 0; i < movelist->getCount(); ++i)
    {
        //copy board state
        // copy_board();
        U64 bitboards_copy[12], occupancies_copy[3];                            
        int side_copy, enpassant_copy, castling_copy;                           
        memcpy(bitboards_copy, m_position->getBitboardPtr(), 96);                                  
        memcpy(occupancies_copy, m_position->getOccupancyPtr(), 24);                              
        side_copy = m_position->getSide(), enpassant_copy = m_position->getEnpassant(), castling_copy = m_position->getCastling(); 

        //increment ply
        ++ply;

        //check if legal move
        if(m_moveGen->makeMove(movelist->getMove(i), all) == 0)
        {
            //decrement ply
            --ply;

            //skip
            continue;
        }

        //increment legal moves
        ++legal_moves;

        int score;

        // full depth search
        if (movesSearched == 0)
        {
            // do normal alpha beta search
            score = -negamax(-beta, -alpha, depth - 1);
        }
        // late move reduction (LMR)
        else
        {
            // condition to consider LMR
            if(
                movesSearched >= full_depth_moves &&
                depth >= reduction_limit &&
                in_check == 0 && 
                get_capture(movelist->getMove(i)) == 0 &&
                get_promotion(movelist->getMove(i)) == 0
                )
                // search current move with reduced depth:
                score = -negamax(-alpha - 1, -alpha, depth - 2);
            
            // hack to ensure that full-depth search is done
            else score = alpha + 1;
            
            // if found a better move during LMR
            if(score > alpha)
            {
                // re-search at full depth but with narrowed score bandwith
                score = -negamax(-alpha - 1, -alpha, depth-1);
            
                // if LMR fails re-search at full depth and full score bandwith
                if((score > alpha) && (score < beta))
                    score = -negamax(-beta, -alpha, depth-1);
            }
        }

        //decrement ply
        --ply;

        //restore the boared state
        // take_back();
        memcpy(m_position->getBitboardPtr(), bitboards_copy, 96);                                  
        memcpy(m_position->getOccupancyPtr(), occupancies_copy, 24);                              
        m_position->setSide(side_copy), m_position->setEnpassant(enpassant_copy), m_position->setCastling(castling_copy);

        //fail-hard beta cutoff  -  alpha < score <= beta
        if(score >= beta)
        {
            if(get_capture(movelist->getMove(i)) == 0)
            {
                //save killer moves
                killer_moves[1][ply] = killer_moves[0][ply];
                killer_moves[0][ply] = movelist->getMove(i);
            }
            
            // node (move) fails hard
            return beta;
        }

        //better move
        if(score > alpha)
        {
            if(get_capture(movelist->getMove(i)) == 0)
            {
                //save history moves
                history_moves[get_piece(movelist->getMove(i))][get_trg(movelist->getMove(i))] += depth;
            }

            //PV node (move)
            alpha = score;

            //write PV move
            pv_table[ply][ply] = movelist->getMove(i);

            //loop over ply
            for(int next_ply = ply + 1; next_ply < pv_length[ply + 1]; ++next_ply)
            {
                //copy move from deeper ply to the current ply
                pv_table[ply][next_ply] = pv_table[ply + 1][next_ply];
            }

            //update pv length
            pv_length[ply] = pv_length[ply + 1];
        }
    }

    // no legal moves in current position
    if(legal_moves == 0)
    {
        // if king is in check
        if(in_check)
        {
            //return mating score
            // the bigger ply = the faster the mate = the bigger the score (if its you who is gatting mated)
            return -49000 + ply;
        }
        else
        {
            // retrun stalemate score
            return 0;
        }
    }

    delete movelist;

    // node (move) fails low
    return alpha;
}

int Evaluation::quiescence(int alpha, int beta)
{
    ++nodes;

    // Evaluation position
    int eval = evaluate();

    //fail-hard beta cutoff  -  alpha < score <= beta
    if(eval >= beta)
    {
        // node (move) fails hard
        return beta;
    }

    //better move
    if(eval > alpha)
    {
        //PV node (move)
        alpha = eval;
    }

    //create move list
    Moves *movelist = new Moves;

    //generate moves
    m_moveGen->generateAllMoves(movelist);

    //sort moves
    sortMoves(movelist);

    //loop over the moves
    for(int i = 0; i < movelist->getCount(); ++i)
    {
        //copy board state
        // copy_board();
        U64 bitboards_copy[12], occupancies_copy[3];                            
        int side_copy, enpassant_copy, castling_copy;                           
        memcpy(bitboards_copy, m_position->getBitboardPtr(), 96);                                  
        memcpy(occupancies_copy, m_position->getOccupancyPtr(), 24);                              
        side_copy = m_position->getSide(), enpassant_copy = m_position->getEnpassant(), castling_copy = m_position->getCastling(); 

        //increment ply
        ++ply;

        //check if legal move
        if(m_moveGen->makeMove(movelist->getMove(i), captrures) == 0)
        {
            //decrement ply
            --ply;

            //skip
            continue;
        }

        //score current move
        int score = -quiescence(-beta, -alpha);

        //decrement ply
        --ply;

        //restore the boared state
        // take_back();
        memcpy(m_position->getBitboardPtr(), bitboards_copy, 96);                                  
        memcpy(m_position->getOccupancyPtr(), occupancies_copy, 24);                              
        m_position->setSide(side_copy), m_position->setEnpassant(enpassant_copy), m_position->setCastling(castling_copy);

        //fail-hard beta cutoff  -  alpha < score <= beta
        if(score >= beta)
        {
            // node (move) fails hard
            return beta;
        }

        //better move
        if(score > alpha)
        {
            //PV node (move)
            alpha = score;
        }
    }

    delete movelist;

    //node (move) fails low
    return alpha;
}

int Evaluation::scoreMove(int move)
{
    //check if scoring pv move
    if(score_pv)
    {
        //check if pv move
        if(pv_table[0][ply] == move)
        {
            //disable scoring pv flag
            score_pv = 0;

            return 20000;
        }
    }

    //score move capture moves
    if(get_capture(move))
    {
        //init trg piece
        int trg_piece = P;

        // pick up bitboard piece index ranges depending on side
        int start_piece, end_piece;
        
        // pick up side to move
        if (m_position->getSide() == white)
        {
            start_piece = p; end_piece = k; 
        }
        else 
        {
            start_piece = P; end_piece = K; 
        }
        
        // loop over bitboards opposite to the current side to move
        for (int bb_piece = start_piece; bb_piece <= end_piece; bb_piece++)
        {
            // if there's a piece on the target square
            if (get_bit(m_position->getBitboard(bb_piece), get_trg(move)))
            {
                // remove it from corresponding bitboard
                trg_piece = bb_piece;
                break;
            }
        }

        //score move by MVV LVA lookup table [src_piece][trg_piece]
        return mvv_lva[get_piece(move)][trg_piece] + 10000;
    }
    //score quiet move
    else
    {
        //score 1st killer move
        if(killer_moves[0][ply] == move)
        {
            return 9000;
        }
        //score 2nd killer move
        else if(killer_moves[1][ply] == move)
        {
            return 8000;
        }
        //history moves
        else
        {
            return history_moves[get_piece(move)][get_trg(move)];
        }
    }

    return 0;
}

void Evaluation::sortMoves(Moves *move_list)
{
    int move_scores[move_list->getCount()];

    //score all moves
    for(int i = 0; i < move_list->getCount(); ++i)
    {
        move_scores[i] = scoreMove(move_list->getMove(i));
    }

    //sorting
    for(int current = 0; current < move_list->getCount() - 1; ++current)
    {
        for(int next = current + 1; next < move_list->getCount(); ++next)
        {
            if(move_scores[current] < move_scores[next])
            {
                //swap scores
                std::swap(move_scores[current], move_scores[next]);

                //swap moves
                std::swap(move_list->getMoveRef(current), move_list->getMoveRef(next));
            }
        }
    }

    // return 0;
}

void Evaluation::pvScoring(Moves *move_list)
{
    //disable following pv flag
    fol_pv = 0;

    for(int i = 0; i < move_list->getCount(); ++i)
    {
        //
        if(pv_table[0][ply] == move_list->getMove(i))
        {
            //enable move scoring flag
            score_pv = 1;

            //enable following pv flag
            fol_pv = 1;
        }
    } 
}
