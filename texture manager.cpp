
#include "texture manager.h"

SDL_Texture* TextureManager::Load(const string& path, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) return nullptr;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void TextureManager::Destroy(SDL_Texture* texture) {
    if (texture) SDL_DestroyTexture(texture);
}
