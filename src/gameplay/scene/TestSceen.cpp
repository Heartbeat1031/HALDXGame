//
// Created by lclcl on 25-5-22.
//

#include "TestSceen.h"
#include "Global.h"
#include "ModelObject.h"
#include "gameobject/House.h"

void TestSceen::Init()
{
    // 初始化地板
    halgame->AddModel("assets\\models\\ground_19.obj");

    AddGameObject<House>(1);
}

void TestSceen::Update()
{
    // ******************
    // 第三人称摄像机的操作
    //
    ImGuiIO &io = ImGui::GetIO();
    // 绕物体旋转
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        halgame->m_pCamera->RotateX(io.MouseDelta.y * 0.01f);
        halgame->m_pCamera->RotateY(io.MouseDelta.x * 0.01f);
    }
    halgame->m_pCamera->Approach(-io.MouseWheel * 1.0f);

    if (ImGui::Begin("メッシュ")) {
        ImGui::Text("三人称モード");
        ImGui::Text("マウスの右ボタンを押したままビューをドラッグします");
        ImGui::Text("日本語と简体と繁體中文と한국인をテストします");
    }
    ImGui::End();
    ImGui::Render();
}