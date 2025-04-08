#include "game.h"
#include <iostream>
#include <ctime>
using namespace std;

Game::Game() : window(nullptr), renderer(nullptr),
              player(NONE), ai(NONE),
              result(RESULT_NONE), showResult(false),
              running(true) {}

Game::~Game() { Close(); }

bool Game::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL Init failed: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Rock Paper Scissors",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window creation failed: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer creation failed: " << SDL_GetError() << endl;
        return false;
    }

    LoadTextures();
    srand(time(0));
    return true;
}

void Game::LoadTextures() {
    textures.background = TextureManager::Load("background.png", renderer);
    textures.rock = TextureManager::Load("bua.png", renderer);
    textures.paper = TextureManager::Load("bao.png", renderer);
    textures.scissors = TextureManager::Load("keo.png", renderer);
    textures.win = TextureManager::Load("win.png", renderer);
    textures.lose = TextureManager::Load("lose.png", renderer);
    textures.draw = TextureManager::Load("draw.png", renderer);

    if (!textures.background) {
        cerr << "Failed to load background image!" << endl;
    }
}

void Game::HandleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);

            if (!showResult && y >= 400 && y <= 500) {
                if (x >= 100 && x <= 200) player = ROCK;
                else if (x >= 300 && x <= 400) player = PAPER;
                else if (x >= 500 && x <= 600) player = SCISSORS;

                if (player != NONE) {
                    ai = static_cast<GameChoice>(rand() % 3 + 1);
                    result = CheckResult(player, ai);
                    showResult = true;
                }
            }
            else if (showResult) {
                showResult = false;
                player = ai = NONE;
            }
        }
    }
}

GameResult Game::CheckResult(GameChoice p, GameChoice a) const {
    if (p == a) return DRAW;
    if ((p == ROCK && a == SCISSORS) ||
        (p == PAPER && a == ROCK) ||
        (p == SCISSORS && a == PAPER)) return WIN;
    return LOSE;
}

void Game::Update() {}

void Game::Render() {

    SDL_RenderClear(renderer);


    if (textures.background) {
        SDL_RenderCopy(renderer, textures.background, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);
    }
    SDL_Rect r = {0, 400, 100, 100};
    for (int i = 0; i < 3; i++) {
        r.x = 100 + i*200;
        SDL_Texture* tex = i == 0 ? textures.scissors :
                          i == 1 ? textures.rock : textures.paper;
        SDL_RenderCopy(renderer, tex, NULL, &r);
    }
    if (showResult) {
        SDL_Texture* resTex = result == WIN ? textures.win :
                             result == LOSE ? textures.lose : textures.draw;
        SDL_Rect resRect = {300, 150, 200, 100};
        SDL_RenderCopy(renderer, resTex, NULL, &resRect);
    }

    SDL_RenderPresent(renderer);
}

void Game::Close() {
    TextureManager::Destroy(textures.background);
    TextureManager::Destroy(textures.rock);
    TextureManager::Destroy(textures.paper);
    TextureManager::Destroy(textures.scissors);
    TextureManager::Destroy(textures.win);
    TextureManager::Destroy(textures.lose);
    TextureManager::Destroy(textures.draw);

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::Run() {
    while (running) {
        HandleEvents();
        Update();
        Render();
    }
}
