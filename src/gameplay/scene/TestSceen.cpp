//
// Created by lclcl on 25-5-22.
//

#include "TestSceen.h"

#include "BoxCollisionC.h"
#include "Global.h"
#include "ModelObject.h"
#include "House.h"
#include "Miku.h"
#include "ModelC.h"
#include "TransformC.h"

void TestSceen::Init() {
    // 地面を追加するサンプル
    auto &ground = AddGameObject<GameObject>();
    // 地面のモデルコンポーネントを追加します
    ground.AddComponent<ModelC>("assets\\models\\ground_19.obj");
    // 地面の座標を設定します
    ground.GetComponent<TransformC>().SetLocalPosition(Vector3(0, -0, 0));
    ground.AddComponent<BoxCollisionC>(
        Vector3(10, 1, 10), // サイズを設定します
        JPH::EMotionType::Static // 静的な物体
    );
    ground.GetComponent<BoxCollisionC>().SetOffset(Vector3(0, -2, 0));

    // ゲームオブジェクトを追加します
    auto &house = AddGameObject<House>();
    miku = &AddGameObject<Miku>();
}

void TestSceen::Update() {

    if (ImGui::Begin("メッシュ")) {
        ImGui::Text("三人称モード");
        ImGui::Text("マウスの右ボタンを押したままビューをドラッグします");
        ImGui::Text("WASDキーで移動します");
    }
    ImGui::End();

    // 三人称カメラの操作
    ImGuiIO &io = ImGui::GetIO();
    // カメラのターゲットをミクの位置に設定します
    halgame->m_pCamera->SetTarget(miku ->GetComponent<TransformC>().GetWorldPosition());
    // オブジェクトの周りを回転する
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        halgame->m_pCamera->RotateX(io.MouseDelta.y * 0.01f);
        halgame->m_pCamera->RotateY(io.MouseDelta.x * 0.01f);
    }
    halgame->m_pCamera->Approach(-io.MouseWheel * 1.0f);
}

void TestSceen::Uninit() {
}
