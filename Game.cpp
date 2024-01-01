#include "Game.hpp"

Game::Game()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Subsystems Initialised!\n";

        window = SDL_CreateWindow("Chess", 100, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window)
        {
            std::cout << "Window created!\n";
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer)
        {
            std::cout << "Renderer created!\n";
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        int imgFlags = IMG_INIT_PNG;
        if( !( IMG_Init( imgFlags ) & imgFlags ) )
        {
            std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
            isRunning = false;
        }

        if(TTF_Init() != 0)
        {
            std::cout << "TTF could not initialize! SDL_image Error: " << TTF_GetError() << "\n";
            isRunning = false;
        }

        start();

        isRunning = true;
    }
    else
    {
        isRunning = false;
        std::cout << "sth wrong :(\n";
    }
}

Game::~Game()
{
    delete myName;
    delete enemyName;
    delete movelist;
    delete eval;
    delete moveGen;
    delete pos;
}

void Game::start()
{
    pos = new Position(starting_position, renderer);
    moveGen = new MoveGenerator(pos);
    eval = new Evaluation(pos, moveGen);
    movelist = new Moves();

    myName = new Text("Player 1", 20, 100, 900, renderer);
    enemyName = new Text("Engine", 20, renderer);
    enemyName->setPos(100, 100 - enemyName->getTextHight());

    pos->printBoard();
}

void Game::makeMove(int move)
{
    int src = get_src(move);
    int trg = get_trg(move);

    if(get_castling(move))
    {
        switch(get_trg(move))
        {
            case g1:
            {
                pos->setPiece(f1, pos->getPiecePtr(h1));
                pos->setPiece(h1, nullptr);
                pos->getPiecePtr(f1)->setSquare(f1);
                break;
            }

            case c1:
            {
                pos->setPiece(d1, pos->getPiecePtr(a1));
                pos->setPiece(a1, nullptr);
                pos->getPiecePtr(d1)->setSquare(d1);
                break;
            }

            case g8:
            {
                pos->setPiece(f8, pos->getPiecePtr(h8));
                pos->setPiece(h8, nullptr);
                pos->getPiecePtr(f8)->setSquare(f8);
                break;
            }

            case c8:
            {
                pos->setPiece(d8, pos->getPiecePtr(a8));
                pos->setPiece(a8, nullptr);
                pos->getPiecePtr(d8)->setSquare(d8);
                break;
            }
        }
    }

    if(get_enpassant(move))
    {
        (pos->getSide() != white ? pos->setPiece(trg + 8, nullptr) : pos->setPiece(trg - 8, nullptr));
    }

    pos->setPiece(trg, pos->getPiecePtr(src));
    pos->setPiece(src, nullptr);
    pos->getPiecePtr(trg)->setSquare(trg);
    clickedOn = nullptr;

    movelist->addMove(move);
}

void Game::startEngine(int depth)
{
    if(count - 1 == tmpCount)
    {
        int move = eval->go(depth);

        if(move == 0 || moveGen->makeMove(move, all) == 0)
        {
            std::cout << "This move is illegal\n";
            clickedOn->setSquare(srcSquare);
            return;
        }

        // pos->printBoard();

        makeMove(move);
    }
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type)
    {
        case SDL_QUIT:
            isRunning = false;
            break;

        // case SDL_MOUSEBUTTONDOWN:
        //     xStart = event.button.x / 100 - 1;
        //     yStart = event.button.y / 100 - 1;
        //     inBounds = xStart < 8 && yStart < 8 && xStart >= 0 && yStart >= 0;
        //     if(!inBounds) break;
        //     clickedOn = position->getPiece(yStart, xStart);

        //     if(xTemp == -1 || yTemp == -1 || (clickedOn != nullptr && clickedOn->getTeam() == position->getTurn()))
        //     {
        //         xTemp = yTemp = -1;
        //         if(clickedOn != nullptr)
        //         {
        //             if(clickedOn->getTeam() == position->getTurn())
        //             {
        //                 for(int i = 0; i < position->getMoveList()->count; ++i)
        //                 {
        //                     if(get_src(position->getMoveList()->moves[i] ) == (int) clickedOn->getSquare()) legalMoves.push_back(position->getMoveList()->moves[i]);
        //                 }
        //             }
        //         }
        //     }
        //     else if(xTemp != -1 && yTemp != -1)
        //     {
        //         for(int i = 0; i < legalMoves.size(); ++i)
        //         {
        //             if(get_trg(legalMoves[i]) == yStart * 8 + xStart)
        //             {
        //                 move = legalMoves[i];
        //                 i = legalMoves.size();
        //             } 
        //         }
        //     }
                
        //     if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT )
        //     {
        //         leftMouseButtonDown = true;
        //     }
        //     break;
        
        // case SDL_MOUSEBUTTONUP:
        // {
        //     xEnd = event.button.x / 100 - 1;
        //     yEnd = event.button.y / 100 - 1;
        //     inBounds = xEnd < 8 && yEnd < 8 && xEnd >= 0 && yEnd >= 0;
        //     if(!inBounds)
        //     {
        //         std::cout << "Mouse draged out of the board!\n";
        //         if(xTemp == -1 || yTemp == -1)
        //         {
        //             if(xStart < 8 && yStart < 8 && xStart >= 0 && yStart >= 0 && clickedOn != nullptr && leftMouseButtonDown) clickedOn->setSquare(yStart, xStart);
        //         }
        //         leftMouseButtonDown = false;
        //         break;
        //     }

        //     //if left btn & was down
        //     if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT)
        //     {
        //         leftMouseButtonDown = false;

        //         if(xTemp == xEnd && yTemp == yEnd) break;

        //         if(xStart == xEnd && yStart == yEnd && xTemp == -1 && yTemp == -1 && clickedOn != nullptr && clickedOn->getTeam() == position->getTurn())
        //         {
        //             clickedOn->setSquare(yStart, xStart);
        //             xTemp = xEnd; 
        //             yTemp = yEnd; 
        //             break;
        //         } 
        //         // else if(xStart == xEnd && yStart == yEnd) 
                

        //         // for(int i = 0; i < legalMoves.size(); ++i)
        //         // {
        //         //     if(get_trg(legalMoves[i]) == yEnd * 8 + xEnd)
        //         //     {
        //         //         move = legalMoves[i];
        //         //         i = legalMoves.size();
        //         //     } 
        //         // }

        //         //drag to move
        //         if((xTemp == -1 || yTemp == -1) && clickedOn != nullptr)
        //         {
        //             for(int i = 0; i < legalMoves.size(); ++i)
        //             {
        //                 if(get_trg(legalMoves[i]) == yEnd * 8 + xEnd)
        //                 {
        //                     if(!get_promotion(move))
        //                     {
        //                         move = legalMoves[i];
        //                         i = legalMoves.size();
        //                     }
        //                     else
        //                     {
        //                         //need to be changed!!!!!
        //                         move = legalMoves[i];
        //                         i = legalMoves.size();
        //                     }
        //                 } 
        //             }

        //             //move is illigal
        //             if(move == 0)
        //             {
        //                 std::cout << "Drag move is illegal\n";

        //                 //set position to where the piece came from
        //                 position->getPiece(yStart, xStart)->setSquare(yStart, xStart);
        //             }
                    
        //             //move is legal
        //             else
        //             {
        //                 if(!position->makeMove(move, false))
        //                 {
        //                     std::cout << "It's check dumbass!\n";
        //                     //set position to where the piece came from
        //                     position->getPiece(yStart, xStart)->setSquare(yStart, xStart);
        //                 }
        //                 else
        //                 {
        //                     std::cout << "Drag move is legal\n";

        //                     position->setPiece(yEnd, xEnd, position->getPiece(yStart, xStart));
        //                     position->setPiece(yStart, xStart, nullptr);
        //                     position->getPiece(yEnd, xEnd)->setSquare(yEnd, xEnd);

        //                     //handle pwan promotions
        //                     if(get_promotion(move))
        //                     {
        //                         position->getPiece(yEnd, xEnd)->setType((PieceType) get_promotion(move));
        //                     }

        //                     if(get_castling(move))
        //                     {
        //                         switch((Square) get_trg(move))
        //                         {
        //                             case Square::G1:
        //                                 position->setPiece(Square::F1, position->getPiece(Square::H1));
        //                                 position->setPiece(Square::H1, nullptr);
        //                                 position->getPiece(Square::F1)->setSquare(Square::F1);
        //                                 break;
                                        
        //                             case Square::C1:
        //                                 position->setPiece(Square::D1, position->getPiece(Square::A1));
        //                                 position->setPiece(Square::A1, nullptr);
        //                                 position->getPiece(Square::D1)->setSquare(Square::D1);
        //                                 break;

        //                             case Square::G8:
        //                                 position->setPiece(Square::F8, position->getPiece(Square::H8));
        //                                 position->setPiece(Square::H8, nullptr);
        //                                 position->getPiece(Square::F8)->setSquare(Square::F8);
        //                                 break;
                                        
        //                             case Square::C8:
        //                                 position->setPiece(Square::D8, position->getPiece(Square::A8));
        //                                 position->setPiece(Square::A8, nullptr);
        //                                 position->getPiece(Square::D8)->setSquare(Square::D8);
        //                                 break;
        //                         }
        //                     }

        //                     move = 0;

        //                     position->clearMoveList();
        //                     legalMoves.clear();
        //                     position->getMoveGen()->generateAllMoves(position->getMoveList());
        //                 }

                        
        //             }
        //         }

        //         //click to move
        //         else if(xTemp != -1 && yTemp != -1)
        //         {
        //             //move is illigal
        //             if(move == 0)
        //             {
        //                 std::cout << "Click move is illegal\n";

        //                 //set position to where the piece came from
        //                 position->getPiece(yTemp, xTemp)->setSquare(yTemp, xTemp);
        //             }

        //             else
        //             {
        //                 if(!position->makeMove(move, false))
        //                 {
        //                     std::cout << "It's check dumbass!\n";

        //                     //set position to where the piece came from
        //                     position->getPiece(yTemp, xTemp)->setSquare(yTemp, xTemp);
        //                 }
        //                 else
        //                 {
        //                     std::cout << "Click move is legal\n";

        //                     position->setPiece(yEnd, xEnd, position->getPiece(yTemp, xTemp));
        //                     position->setPiece(yTemp, xTemp, nullptr);
        //                     position->getPiece(yEnd, xEnd)->setSquare(yEnd, xEnd);

        //                     //handle pwan promotions
        //                     if(get_promotion(move))
        //                     {
        //                         position->getPiece(yEnd, xEnd)->setType((PieceType) get_promotion(move));
        //                     }

        //                     if(get_castling(move))
        //                     {
        //                         switch((Square) get_trg(move))
        //                         {
        //                             case Square::G1:
        //                                 position->setPiece(Square::F1, position->getPiece(Square::H1));
        //                                 position->setPiece(Square::H1, nullptr);
        //                                 position->getPiece(Square::F1)->setSquare(Square::F1);
        //                                 break;
                                        
        //                             case Square::C1:
        //                                 position->setPiece(Square::D1, position->getPiece(Square::A1));
        //                                 position->setPiece(Square::A1, nullptr);
        //                                 position->getPiece(Square::D1)->setSquare(Square::D1);
        //                                 break;

        //                             case Square::G8:
        //                                 position->setPiece(Square::F8, position->getPiece(Square::H8));
        //                                 position->setPiece(Square::H8, nullptr);
        //                                 position->getPiece(Square::F8)->setSquare(Square::F8);
        //                                 break;
                                        
        //                             case Square::C8:
        //                                 position->setPiece(Square::D8, position->getPiece(Square::A8));
        //                                 position->setPiece(Square::A8, nullptr);
        //                                 position->getPiece(Square::D8)->setSquare(Square::D8);
        //                                 break;
        //                         }
        //                     }

        //                     xTemp = yTemp = -1;
        //                     move = 0;

        //                     position->clearMoveList();
        //                     legalMoves.clear();
        //                     position->getMoveGen()->generateAllMoves(position->getMoveList());
        //                 }
                        
        //             }
                    
        //         }
        //     }

        //     break;
        // }

        case SDL_MOUSEBUTTONDOWN:
        {   
            leftMouseButtonDown = true;

            srcFile = event.button.x / 100 - 1;
            srcRank = event.button.y / 100 - 1;
            srcSquare = srcRank * 8 + srcFile;

            inBounds = srcFile < 8 && srcRank < 8 && srcFile >= 0 && srcRank >= 0;
            if(!inBounds) break;

            clickedOn = pos->getPiecePtr(srcSquare);

            if(clickedOn == nullptr)
            {
                std::cout << "Nic tu nie ma\n";
                break;
            }
            else
            {
                // clickedOn->sayMyName();
                // std::cout << "\n";
            }
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            leftMouseButtonDown = false;

            trgFile = event.button.x / 100 - 1;
            trgRank = event.button.y / 100 - 1;

            trgSquare = trgRank * 8 + trgFile;

            inBounds = trgFile < 8 && trgRank < 8 && trgFile >= 0 && trgRank >= 0;
            if(!inBounds) break;

            if(clickedOn == nullptr)
            {
                break;
            }

            Moves* movelist = new Moves();
            moveGen->generateAllMoves(movelist);

            int move;

            for(int i = 0; i < movelist->getCount(); ++i)
            {
                move = movelist->getMove(i);
                if(srcSquare == get_src(move) && trgSquare == get_trg(move))
                {
                    break;
                }

                move = 0;
            }

            if(move == 0 || moveGen->makeMove(move, all) == 0)
            {
                std::cout << "This move is illegal\n";
                clickedOn->setSquare(srcSquare);
                break;
            }

            // pos->printBoard();

            makeMove(move);

            movelist->addMove(move);

            tmpCount = count;

            break;
        }
        
        case SDL_MOUSEMOTION:
            mousePos = { event.motion.x, event.motion.y };

            if (leftMouseButtonDown && clickedOn != nullptr)
            {
                clickedOn->setPos(mousePos.x - 50, mousePos.y - 50);
            }
            break;



        default:
            break;
    }
}

void Game::update()
{
    startEngine(7);

    // if(tmpCount == count - 1)
    // {
    //     std::thread search(startEngine, 7);
    // }

    myName->update();
    enemyName->update();
    
    pos->update();

    count++;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    
    //Draw board
    renderBoard();

    //Draw move list
    // renderMoveList();

    myName->render();
    enemyName->render();
    
    //render pieces
    pos->render();

    if(clickedOn != nullptr)
    {
        clickedOn->render();
    }

    SDL_SetRenderDrawColor(renderer, 49, 46, 43, 255);

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned!\n";
}

void Game::renderBoard()
{
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            rect[i * 8 + j].w = rect[i * 8 + j].h = 100;
            rect[i * 8 + j].x = j * 100 + 100;
            rect[i * 8 + j].y = i * 100 + 100;

            if((i + j) % 2 == 0)
            {
                SDL_SetRenderDrawColor(renderer, 238, 238, 210, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 118, 150, 86, 255);
            }

            SDL_RenderFillRect(renderer, &rect[i * 8 + j]);
        }
    }

    //change backgrounf color of the clicked piece
    if(clickedOn != nullptr)
    {
        // SDL_SetRenderDrawColor(renderer, 255, 255, 51, 0);

        int rank = clickedOn->getSquare() / 8;
        int file = clickedOn->getSquare() % 8;

        if((rank + file) % 2 == 0)
        {
            SDL_SetRenderDrawColor(renderer, 247, 247, 131, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 187, 203, 69, 255);
        }

        SDL_RenderFillRect(renderer, &rect[rank * 8 + file]);
    }

    if(movelist->getMove(movelist->getCount()) - 1 != 0 && movelist->getCount() != 0)
    {
        int squares[2] = { get_src(movelist->getMove(movelist->getCount() - 1)), get_trg(movelist->getMove(movelist->getCount() - 1)) };

        for(int square : squares)
        {
            if(((square / 8) + (square % 8)) % 2 == 0)
            {
                SDL_SetRenderDrawColor(renderer, 247, 247, 131, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 187, 203, 69, 255);
            }

            SDL_RenderFillRect(renderer, &rect[square]);
        }
    }
}

// void Game::renderMoveList()
// {
//     SDL_SetRenderDrawColor(renderer, 38, 36, 33, 255);

//     moveListRect.w = 400;
//     moveListRect.h = 800;
//     moveListRect.x = 1000;
//     moveListRect.y = 100;

//     SDL_RenderFillRect(renderer, &moveListRect);

//     // for(int i = 1; i < (movelist->getCount() / 2) + 1; ++i)
//     // {

//     // }


//     SDL_SetRenderDrawColor(renderer, 49, 46, 43, 255);
// }
