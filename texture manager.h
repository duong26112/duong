
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

class TextureManager {
public:
    static SDL_Texture* Load(const string& path, SDL_Renderer* renderer);
    static void Destroy(SDL_Texture* texture);
};
