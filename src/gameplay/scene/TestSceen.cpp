//
// Created by lclcl on 25-5-22.
//

#include "TestSceen.h"
#include "Global.h"
#include "ModelObject.h"
#include "House.h"

void TestSceen::Init() {
    // ground モジュール　
    UID groundHandle = halgame->AddModel("assets\\models\\ground_19.obj");
    ModelObject &ground = halgame->GetModelObject(groundHandle);
    Transform &groundTransform = ground.GetTransform();
    groundTransform.SetPosition(0, 0, 0);

    UID hand = halgame->AddModel("assets\\models\\GothicMiku\\Gothic Miku (Apearance Ver.)5.pmx");
    ModelObject &praying = halgame->GetModelObject(hand);
    Transform &transform = praying.GetTransform();
    transform.SetPosition(5.0f, 0.0f, 0.0f);
    transform.SetRotation(0.0f, 0.0f, 0.0f);
    //transform.SetScale(1.0f, 1.0f, 1.0f);
    transform.SetScale(0.5f, 0.5f, 0.5f);
    //transform.SetScale(0.01f, 0.01f, 0.01f);
    // house ゲームオブジェクト
    House house = AddGameObject<House>();
    House house1 = GetGameObject<House>(house.GetUID());
}

void TestSceen::Update() {
    // ******************
    // 三人称カメラの操作
    //
    ImGuiIO &io = ImGui::GetIO();
    // オブジェクトの周りを回転する
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

void TestSceen::Uninit() {

}
