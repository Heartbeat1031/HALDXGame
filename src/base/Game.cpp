//
// Created by clcc777 on 25-5-20.
//

#include "Game.h"

Game::Game(HINSTANCE hInstance, const std::wstring &windowName, int initWidth, int initHeight): GameApp(hInstance, windowName, initWidth, initHeight) {
}

Game::~Game() = default;

bool Game::Init() {
    if (!GameApp::Init()) {
        return false;
    }

    return true;
}

void Game::UpdateScene(float dt) {
    GameApp::UpdateScene(dt);
}
