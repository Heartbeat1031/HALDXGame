//
// Created by clcc777 on 25-5-20.
//
#pragma once
#include "GameApp.h"

class Game : public GameApp {
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game() override;

    bool Init() override;
    void UpdateScene(float dt) override;
};
