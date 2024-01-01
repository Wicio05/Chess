#include "Game.hpp"
#include "Position.hpp"
#include "MoveGenerator.hpp"
#include "Evaluation.hpp"

int main(int argc, char *argv[])
{
    const int FPS = 120;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    Game *game = new Game();

    while(game->running())
    {
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    
    game->clean();

    delete game;

	SDL_Delay(1000);

    // Position* pos = new Position();
    // pos->parseFEN("4k2r/p7/8/8/8/8/8/R3K3 w -Qk- - 0 1");
    // pos->printBoard();
    // pos->perftTest(1);
    // MoveGenerator* moveGen = new MoveGenerator(pos);

    return 0;
}