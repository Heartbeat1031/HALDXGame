//
// Created by clcc777 on 25-5-20.
//

#include "Game.h"
#include "TestSceen.h"
Scene* Game::m_Scene = nullptr;
Game::Game(HINSTANCE hInstance, const std::wstring &windowName, int initWidth, int initHeight): GameApp(hInstance, windowName, initWidth, initHeight) {
}

Game::~Game() = default;

bool Game::Init() {
    if (!GameApp::Init()) {
        return false;
    }

    // ImGuiを初期化
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.OversampleH = 1;
    config.OversampleV = 1;
    config.PixelSnapH = true;
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-ja.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesJapanese());
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-zh_hans.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-zh_hant.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-ko.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesKorean());
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-latin.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesDefault());

    // テストシーンを読み込む
    SetScene<TestSceen>();
    return true;
}

void Game::Update(float dt) {
    GameApp::Update(dt);
    m_Scene->UpdateBase();
}

template<typename T>
void Game::SetScene() {
    if (m_Scene)
    {
        m_Scene->UninitBase();
        delete m_Scene;
    }
    m_Scene = new T();
    m_Scene->InitBase();
}