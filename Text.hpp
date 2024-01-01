#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

#include "TextureManager.hpp"

class Text
{
public:
    Text(SDL_Renderer* renderer);
    Text(std::string text, SDL_Renderer* renderer);
    Text(std::string text, int size, SDL_Renderer* renderer);
    Text(std::string text, int size, int x, int y, SDL_Renderer* renderer);
    ~Text();

    void LoadTexture();

    void update();
    void render();

    //getters
    std::string getText();
    int getTextWidth();
    int getTextHight();

    //setters
    void setText(std::string text);
    void setPos(int x, int y);

private:
    std::string _text;
    int _x = 0, _y = 0;
    int _size = 24;
    int _w, _h;

    SDL_Texture* fontTex;
    SDL_Renderer* _renderer;

    SDL_Rect destRect;
};