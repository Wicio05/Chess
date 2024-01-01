#include "Text.hpp"

Text::Text(SDL_Renderer *renderer) : _renderer(renderer)
{

}

Text::Text(std::string text, SDL_Renderer *renderer) : _text(text), _renderer(renderer)
{ 
    LoadTexture();
}

Text::Text(std::string text, int size, SDL_Renderer *renderer) : _text(text), _size(size), _renderer(renderer)
{
    LoadTexture();
}

Text::Text(std::string text, int size, int x, int y, SDL_Renderer *renderer) : _text(text), _size(size), _x(x), _y(y), _renderer(renderer)
{
    LoadTexture();
}

Text::~Text()
{
    SDL_DestroyTexture(fontTex);
}

void Text::LoadTexture()
{
    TTF_Font* font = TTF_OpenFont("./assets/sans_r.ttf", _size);

    if(font == NULL)
    {
        perror("Could not open font file\n");
    }

    SDL_Color White = {255, 255, 255};

    SDL_Surface* tempSurface = TTF_RenderText_Solid(font, _text.c_str(), White);
    _w = tempSurface->w;
    _h = tempSurface->h;

    fontTex = SDL_CreateTextureFromSurface(_renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    TTF_CloseFont(font);

    // fontTex = TextureManager::LoadFontTexture(_text.c_str(), _size, "./assets/sans_r.ttf", _renderer);
}

void Text::update()
{
    destRect.x = _x;
    destRect.y = _y;
    destRect.w = _w; 
    destRect.h = _h; 
}

void Text::render()
{
    SDL_RenderCopy(_renderer, fontTex, nullptr, &destRect);
}

std::string Text::getText()
{
    return _text;
}

int Text::getTextWidth()
{
    return _w;
}

int Text::getTextHight()
{
    return _h;
}

void Text::setText(std::string text)
{
    _text = text;
}

void Text::setPos(int x, int y)
{
    _x = x, _y = y;
}
