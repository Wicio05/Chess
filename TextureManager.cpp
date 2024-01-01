#include "TextureManager.hpp"

SDL_Texture *TextureManager::LoadImgTexture(const char* texture, SDL_Renderer* renderer)
{
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

// SDL_Texture *TextureManager::LoadFontTexture(const char* text, int size, const char* texture, SDL_Renderer* renderer)
// {
//     TTF_Font* Sans = TTF_OpenFont(texture, size);

//     if(Sans == NULL)
//     {
//         perror("Could not open font file\n");
//     }

//     SDL_Color White = {255, 255, 255};

//     SDL_Surface* tempSurface = TTF_RenderText_Solid(Sans, text, White); 
//     SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
//     SDL_FreeSurface(tempSurface);

//     return tex;
// }