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

    // 加载字体
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.OversampleH = 1;
    config.OversampleV = 1;
    config.PixelSnapH = true;
    // 主字体：加载简体中文字体
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-zh_hans.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    // 合并日文
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-ja.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesJapanese());
    // 合并韩文
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-ko.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesKorean());
    // 合并拉丁字母
    io.Fonts->AddFontFromFileTTF("assets/fonts/fusion-pixel-10px-monospaced-latin.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesDefault());


    // 加载模式测试
    // 初始化地板
    AddModel("assets\\models\\ground_19.obj");
    ModelObject &house = GetModelObject(AddModel("assets\\models\\house.obj"));
    // 获取房屋包围盒
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(0.015f, 0.015f, 0.015f);
    DirectX::BoundingBox houseBox = house.GetModel()->boundingbox;
    houseBox.Transform(houseBox, S);
    // 让房屋底部紧贴地面
    Transform &houseTransform = house.GetTransform();
    houseTransform.SetScale(0.015f, 0.015f, 0.015f);
    houseTransform.SetPosition(0.0f, -(houseBox.Center.y - houseBox.Extents.y + 1.0f), 0.0f);
    return true;
}

void Game::Update(float dt) {
    GameApp::Update(dt);
    // ******************
    // 第三人称摄像机的操作
    //
    ImGuiIO &io = ImGui::GetIO();
    // 绕物体旋转
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        m_pCamera->RotateX(io.MouseDelta.y * 0.01f);
        m_pCamera->RotateY(io.MouseDelta.x * 0.01f);
    }
    m_pCamera->Approach(-io.MouseWheel * 1.0f);

    if (ImGui::Begin("メッシュ")) {
        ImGui::Text("三人称モード");
        ImGui::Text("マウスの右ボタンを押したままビューをドラッグします");
        ImGui::Text("日本語と中国语と한국인をテストします");
    }
    ImGui::End();
    ImGui::Render();
}
