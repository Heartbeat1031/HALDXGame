//
// Created by clcc777 on 25-5-20.
//

#include "Game.h"
#include "TestSceen.h"

// 游戏实例
// ゲームインスタンス
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
    //日本語文字体を追加
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-ja.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesJapanese());
    config.MergeMode = true;
    // 中国語簡体字、繁体字、韓国語、ラテン文字のフォントを追加
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-zh_hans.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-zh_hant.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-ko.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesKorean());
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-latin.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesDefault());

    // Physicsシステムを初期化
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();
    mTempAllocator = new JPH::TempAllocatorImpl(32 * 1024 * 1024);
    mJobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
    // テストシーンを読み込む
    SetScene<TestSceen>();
    return true;
}

// ゲームの実行
void Game::Update(float dt) {
    GameApp::Update(dt);
    // シーンの更新
    m_Scene->UpdateBase(dt);
}