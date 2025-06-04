//
// Created by lclcl on 25-5-22.
//

#include "TestSceen.h"

#include "AnimatorC.h"
#include "BoxCollisionC.h"
#include "Global.h"
#include "ModelObject.h"
#include "House.h"
#include "Miku.h"
#include "ModelC.h"
#include "TransformC.h"

void TestSceen::Init() {
    // 地面を追加するサンプル
    GameObject &ground = AddGameObject<GameObject>();
    // 地面のモデルコンポーネントを追加します
    ground.AddComponent<ModelC>("assets\\models\\ground_19.obj");
    // 地面の座標を設定します
    ground.GetComponent<TransformC>().SetLocalPosition(Vector3(0, -0, 0));
    ground.AddComponent<BoxCollisionC>(
        Vector3(10, 1, 10), // サイズを設定します
        JPH::EMotionType::Static // 静的な物体
    );
    ground.GetComponent<BoxCollisionC>().SetOffset(Vector3(0, -2, 0));

    // House ゲームオブジェクトを追加します
    House &house = AddGameObject<House>();

    Miku &miku = AddGameObject<Miku>();
    miku.AddComponent<ModelC>("assets/models/Praying.fbx");
    miku.AddComponent<AnimatorC>("Idle");
    miku.GetComponent<TransformC>().SetLocalPosition(Vector3(5, 10, 0));
     miku.GetComponent<TransformC>().SetLocalScale(Vector3(0.03f, 0.03f, 0.03f));
    miku.AddComponent<BoxCollisionC>(Vector3(2, 2, 2), JPH::EMotionType::Dynamic).SetOffset(Vector3(0, 2, 0));
    //miku.GetComponent<TransformC>().SetLocalScale(Vector3(0.5, 0.5, 0.5));
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
}

void TestSceen::Uninit() {

}
