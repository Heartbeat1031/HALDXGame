#include "TitleScene.h"

#include "BoxCollisionC.h"
#include "Global.h"
#include "ModelC.h"
#include "TransformC.h"
#include <DirectXMath.h>
#include "GameObject.h"

void TitleScene::Init() {
    auto& screen = AddGameObject<GameObject>();
    screen.AddComponent<ModelC>("assets\\models\\title.obj");
    screen.GetComponentRef<TransformC>().SetWorldPosition(Vector3(0, 0, 0));
    screen.AddComponent<BoxCollisionC>(
        JPH::EMotionType::Static, // 静的な物体
        Vector3(10, 1, 10) // サイズを設定します
    );
}

void TitleScene::Update() {
    // 三人称カメラの操作
    ImGuiIO& io = ImGui::GetIO();
    // カメラのターゲットをミクの位置に設定します
    halgame->m_pCamera->SetTarget(Vector3(0, 0, 0));
    // オブジェクトの周りを回転する
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        halgame->m_pCamera->RotateX(io.MouseDelta.y * 0.01f);
        halgame->m_pCamera->RotateY(io.MouseDelta.x * 0.01f);
    }
    halgame->m_pCamera->Approach(-io.MouseWheel * 1.0f);
}

void TitleScene::Uninit() {

}