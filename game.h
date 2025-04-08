
#pragma once
#include <SDL.h>
#include "constants.h"
#include "texture manager.h"
using namespace std;

class Game {
public:
    Game();
    ~Game();

    bool Init();
    void Run();
    void Close();

private:
    void HandleEvents();
    void Update();
    void Render();
    void LoadTextures();
    GameResult CheckResult(GameChoice player, GameChoice ai) const;

    SDL_Window* window;
    SDL_Renderer* renderer;

   struct {
    SDL_Texture *background, *rock, *paper, *scissors, *win, *lose, *draw;
} textures;

    GameChoice player, ai;
    GameResult result;
    bool showResult, running;
};
