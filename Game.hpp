#ifndef Game_hpp
#define Game_hpp

// #include <SDL.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <thread>

#include "Position.hpp"
#include "MoveGenerator.hpp"
#include "Evaluation.hpp"
#include "Text.hpp"
#include "Piece.hpp"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

class Game
{
private:
    MoveGenerator *moveGen = nullptr;
    Position *pos = nullptr;
    Evaluation* eval = nullptr;
    Piece *clickedOn = nullptr;

    Moves* movelist = nullptr;

    Text* enemyName = nullptr;
    Text* myName = nullptr;

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Point mousePos;

    //board squares
    SDL_Rect rect[64];

    SDL_Rect moveListRect;
    std::vector<Text*> moveListText;

    bool isRunning;
    int count = 0;
    int tmpCount = INT32_MIN;

    int srcFile = -1, srcRank = -1, srcSquare = -1;
    int trgFile = -1, trgRank = -1, trgSquare = -1;

    // int lastMove = 0;

    bool inBounds;
    bool leftMouseButtonDown = false;

public:
    Game();
    ~Game();

    void start();

    void makeMove(int move);

    void startEngine(int depth);

    void handleEvents();
    void update();
    void render();
    void clean();

    void renderBoard();
    void renderMoveList();

    inline bool running() { return isRunning; }
};

#endif 