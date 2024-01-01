#ifndef TextureManager_hpp
#define TextureManager_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdexcept>

class TextureManager
{
public:
    static SDL_Texture* LoadImgTexture(const char* texture, SDL_Renderer* renderer);

    // static SDL_Texture* LoadFontTexture(const char* text, int size, const char* texture, SDL_Renderer* renderer);
};

#endif 