#ifndef EVALUATE_HPP
#define EVALUATE_HPP

#include "Position.hpp"
#include "MoveGenerator.hpp"
#include "Macros.hpp"

class Evaluation
{
public:
    Evaluation(Position* position, MoveGenerator* moveGenerator);
    ~Evaluation();

    //uci handling
    //parse move
    int parseMove(std::string moveStr);

    //parse position
    void parsePosition(std::string pos);

    //parse go
    void parseGo(std::string command);

    //main uci loop
    void uciLoop();

    int go(int depth);

    //evaluation
    //search for the best move
    void searchPosition(int depth);

    //evaluate func
    int evaluate();

    //negamax alpha beta search
    int negamax(int alpha, int beta, int depth);

    //quiescence search
    int quiescence(int alpha, int beta);

    //score move
    int scoreMove(int move);

    //sort moves from best to worst based on score
    void sortMoves(Moves *move_list);

    //enable pv scoring
    void pvScoring(Moves *move_list);

private:
    MoveGenerator* m_moveGen = nullptr;
    Position* m_position = nullptr;

    //nodes
    U64 nodes = 0;

    //half moves
    int ply = 0;

    //following pv flag
    bool fol_pv;
    
    // score pv move flag
    bool score_pv;

    //killer moves [id][ply]
    int killer_moves[2][64];

    //history moves [piece][sqaure]
    int history_moves[12][64];

    /*
    ================================
        Triangular PV(Principle Variation) table
    --------------------------------
    PV line: e2e4 e7e5 g1f3 b8c6
    ================================

        0    1    2    3    4    5
    
        0    m1   m2   m3   m4   m5   m6
        
        1    0    m2   m3   m4   m5   m6 
        
        2    0    0    m3   m4   m5   m6
        
        3    0    0    0    m4   m5   m6
        
        4    0    0    0    0    m5   m6
        
        5    0    0    0    0    0    m6
    */

    // PV length [ply]
    int pv_length[max_ply];

    // PV table [ply][ply]
    int pv_table[max_ply][max_ply];

    // MVV LVA [attacker][victim]
    const int mvv_lva[12][12] = 
    {
        105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
        104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
        103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
        102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
        101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
        100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600,

        105, 205, 305, 405, 505, 605,  105, 205, 305, 405, 505, 605,
        104, 204, 304, 404, 504, 604,  104, 204, 304, 404, 504, 604,
        103, 203, 303, 403, 503, 603,  103, 203, 303, 403, 503, 603,
        102, 202, 302, 402, 502, 602,  102, 202, 302, 402, 502, 602,
        101, 201, 301, 401, 501, 601,  101, 201, 301, 401, 501, 601,
        100, 200, 300, 400, 500, 600,  100, 200, 300, 400, 500, 600
    };
};

#endif // !EVALUATE_HPP
#define EVALUATE_HPP
