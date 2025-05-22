//
// Created by clcc777 on 25-5-20.
//
#pragma once
#include "GameApp.h"
#include "Scene.h"

class Game : public GameApp {
private:
    static Scene* m_Scene;
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game() override;

    bool Init() override;
    void Update(float dt) override;

    template <typename T>
    static void SetScene();
};
