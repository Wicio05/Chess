#ifndef MOVEGENERATOR_HPP
#define MOVEGENERATOR_HPP

#include "Position.hpp"
#include "Macros.hpp"

class Moves //can use vector<int>
{
public:
    Moves()
    {
        memset(moves, 0, sizeof(moves));
    }

    void addMove(int move)
    {
        try
        {
            //store move
            moves[count] = move;

            //increment count
            count++;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    int getMove(int index)
    {
        return moves[index];
    }

    int& getMoveRef(int index)
    {
        return moves[index];
    }

    int getCount()
    {
        return count;
    }

private:
    //Moves arr
    int moves[256];

    //move count
    int count = 0;
};

class MoveGenerator
{
public:
    MoveGenerator(Position* position);

    ~MoveGenerator();

    //generate pawn attacks
    U64 maskPawnAttacks(int side, int square);

    //generate knight attacks
    U64 maskKnightAttacks(int square);

    //generate king attacks
    U64 maskKingAttacks(int square);

    //generate bishop attacks
    U64 maskBishopAttacks(int square);

    //generate bishop attacks on the fly
    U64 bishopAttacksOnTheFly(int square, U64 blocks);

    //generate rook attacks
    U64 maskRookAttacks(int square);

    //generate rook attacks on the fly
    U64 rookAttacksOnTheFly(int square, U64 blocks);

    //set occupancies
    U64 setOccupancy(int index, int bits_in_mask, U64 attack_mask);

    //init leaper pieces attcks
    void initLeapersAttacks();

    //init slider pieces' attack tables
    void initSlidersAttacks(bool is_bishop);

    //init all
    void initAll();

    //get bishop attacks
    U64 getBishopAttacks(int square, U64 occupancy);

    //get rook attacks
    U64 getRookAttacks(int square, U64 occupancy);

    //get queen attacks
    U64 getQueenAttacks(int square, U64 occupancy);

    //is square attacked func
    bool isSquareAttacked(int square, int side);

    // generate all moves
    void generateAllMoves(Moves *move_list);

    //make move on the board
    bool makeMove(int move, int capture_flag);

    //perft driver 
    void perftDriver(int depth);

    //perft test
    void perftTest(int depth);

    //print move
    void printMove(int move);
    

    //getters
    U64 getNodes();


    //setters
    void setNodes(U64 val);

private:
    Position* m_position = nullptr;

    // std::vector<int> m_move_list;

    U64 m_nodes = 0;

    //rook magic numbers
    U64 rook_magic_numbers[64] = 
    {
        0x8a80104000800020ULL,
        0x140002000100040ULL,
        0x2801880a0017001ULL,
        0x100081001000420ULL,
        0x200020010080420ULL,
        0x3001c0002010008ULL,
        0x8480008002000100ULL,
        0x2080088004402900ULL,
        0x800098204000ULL,
        0x2024401000200040ULL,
        0x100802000801000ULL,
        0x120800800801000ULL,
        0x208808088000400ULL,
        0x2802200800400ULL,
        0x2200800100020080ULL,
        0x801000060821100ULL,
        0x80044006422000ULL,
        0x100808020004000ULL,
        0x12108a0010204200ULL,
        0x140848010000802ULL,
        0x481828014002800ULL,
        0x8094004002004100ULL,
        0x4010040010010802ULL,
        0x20008806104ULL,
        0x100400080208000ULL,
        0x2040002120081000ULL,
        0x21200680100081ULL,
        0x20100080080080ULL,
        0x2000a00200410ULL,
        0x20080800400ULL,
        0x80088400100102ULL,
        0x80004600042881ULL,
        0x4040008040800020ULL,
        0x440003000200801ULL,
        0x4200011004500ULL,
        0x188020010100100ULL,
        0x14800401802800ULL,
        0x2080040080800200ULL,
        0x124080204001001ULL,
        0x200046502000484ULL,
        0x480400080088020ULL,
        0x1000422010034000ULL,
        0x30200100110040ULL,
        0x100021010009ULL,
        0x2002080100110004ULL,
        0x202008004008002ULL,
        0x20020004010100ULL,
        0x2048440040820001ULL,
        0x101002200408200ULL,
        0x40802000401080ULL,
        0x4008142004410100ULL,
        0x2060820c0120200ULL,
        0x1001004080100ULL,
        0x20c020080040080ULL,
        0x2935610830022400ULL,
        0x44440041009200ULL,
        0x280001040802101ULL,
        0x2100190040002085ULL,
        0x80c0084100102001ULL,
        0x4024081001000421ULL,
        0x20030a0244872ULL,
        0x12001008414402ULL,
        0x2006104900a0804ULL,
        0x1004081002402ULL,
    };

    //bishop magic numbers
    U64 bishop_magic_numbers[64] = 
    {
        0x40040844404084ULL,
        0x2004208a004208ULL,
        0x10190041080202ULL,
        0x108060845042010ULL,
        0x581104180800210ULL,
        0x2112080446200010ULL,
        0x1080820820060210ULL,
        0x3c0808410220200ULL,
        0x4050404440404ULL,
        0x21001420088ULL,
        0x24d0080801082102ULL,
        0x1020a0a020400ULL,
        0x40308200402ULL,
        0x4011002100800ULL,
        0x401484104104005ULL,
        0x801010402020200ULL,
        0x400210c3880100ULL,
        0x404022024108200ULL,
        0x810018200204102ULL,
        0x4002801a02003ULL,
        0x85040820080400ULL,
        0x810102c808880400ULL,
        0xe900410884800ULL,
        0x8002020480840102ULL,
        0x220200865090201ULL,
        0x2010100a02021202ULL,
        0x152048408022401ULL,
        0x20080002081110ULL,
        0x4001001021004000ULL,
        0x800040400a011002ULL,
        0xe4004081011002ULL,
        0x1c004001012080ULL,
        0x8004200962a00220ULL,
        0x8422100208500202ULL,
        0x2000402200300c08ULL,
        0x8646020080080080ULL,
        0x80020a0200100808ULL,
        0x2010004880111000ULL,
        0x623000a080011400ULL,
        0x42008c0340209202ULL,
        0x209188240001000ULL,
        0x400408a884001800ULL,
        0x110400a6080400ULL,
        0x1840060a44020800ULL,
        0x90080104000041ULL,
        0x201011000808101ULL,
        0x1a2208080504f080ULL,
        0x8012020600211212ULL,
        0x500861011240000ULL,
        0x180806108200800ULL,
        0x4000020e01040044ULL,
        0x300000261044000aULL,
        0x802241102020002ULL,
        0x20906061210001ULL,
        0x5a84841004010310ULL,
        0x4010801011c04ULL,
        0xa010109502200ULL,
        0x4a02012000ULL,
        0x500201010098b028ULL,
        0x8040002811040900ULL,
        0x28000010020204ULL,
        0x6000020202d0240ULL,
        0x8918844842082200ULL,
        0x4010011029020020ULL,
    };

    //pseudo random number state
    unsigned int random_state = 1804289383;

    //pawn attacks table [side][square]
    U64 pawn_attacks[2][64];

    //knight attacks table
    U64 knight_attacks[64];

    //bishop attack masks
    U64 bishop_masks[64];

    //rook attack masks
    U64 rook_masks[64];

    //bishop attacks table [square][occupancy]
    U64 bishop_attacks[64][512];

    //rook attacks table [square][occupancy]
    U64 rook_attacks[64][4096];

    //king attacks table
    U64 king_attacks[64];

};

#endif // !MOVEGENERATOR_HPP
