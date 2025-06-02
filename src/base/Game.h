//
// Created by clcc777 on 25-5-20.
//
#pragma once
#include "GameApp.h"
#include "Scene.h"
class Scene;


//---------------------------------------------------------------//
// 定义游戏程序, 包含了游戏的窗口, 游戏循环, 渲染, 输入处理等功能
// ゲームプログラムを定義します。ゲームのウィンドウ、ゲームループ、レンダリング、入力処理などの機能を含みます。
//---------------------------------------------------------------//

class Game : public GameApp {
private:
    // 現在のシーン
    Scene* m_Scene = nullptr;
public:
    Game(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~Game() override;
    // ゲームの初期化
    bool Init() override;
    // ゲームの更新 dtはデルタタイム
    void Update(float dt) override;
    // シーンを設定する
    template <typename T>
    void SetScene();
    // シーンを取得する
    Scene* GetScene() const { return m_Scene; }
};

// シーンを設定する
template<typename T>
void Game::SetScene() {
    // TはSceneのサブクラスである必要があります
    static_assert(std::is_base_of<Scene, T>::value, "TはSceneのサブクラスでなければなりません");
    // 既存のシーンを解放
    if (m_Scene != nullptr)
    {
        m_Scene->UninitBase();
        delete m_Scene;
    }
    // 新しいシーンを作成
    m_Scene = new T();
    // シーンを初期化
    m_Scene->InitBase();
}