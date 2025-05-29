//
// Created by clcc777 on 25-5-20.
//
#pragma once
#include "GameApp.h"
class Scene;
class Game : public GameApp {
private:
    Scene* m_Scene = nullptr;
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game() override;

    bool Init() override;
    void Update(float dt) override;

    template <typename T>
    void SetScene();
    Scene* GetScene() const { return m_Scene; }
};
