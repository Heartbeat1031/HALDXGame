//
// Created by lclcl on 25-5-22.
//

#include "TestSceen.h"
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
    ground.AddComponent<ModelC>().Load("assets\\models\\ground_19.obj");
    // 地面の座標を設定します
    ground.GetComponent<TransformC>().SetLocalPosition(Vector3(0, -0, 0));

    // House ゲームオブジェクトを追加します
    House &house = AddGameObject<House>();
    // Miku ゲームオブジェクトを追加します
    Miku &miku = AddGameObject<Miku>();
    // HouseのTransformCを取得し、MikuのTransformCを取得して位置を設定します
    TransformC &houseTC = house.GetComponent<TransformC>();
    TransformC &mikuTC = miku.GetComponent<TransformC>();
    mikuTC.SetLocalPosition(Vector3(5, 0, 0));
    // MikuのTransformCをHouseの子に設定します
     houseTC.AddChild(&mikuTC);

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
