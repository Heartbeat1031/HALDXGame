#include "GameoverScene.h"
#include "WinScene.h"

#include "GameObject.h"
#include "ModelC.h"
#include "TitleScene.h"

void GameoverScene::Init() {
    auto& screen = AddGameObject<GameObject>();
    screen.AddComponent<ModelC>("assets/models/gameover.fbx");
    screen.GetComponentRef<TransformC>().SetWorldPosition(Vector3(0, 0, 0));
    screen.GetComponentRef<TransformC>().SetLocalScale(Vector3(3.8f, 3.8f, 1));

    halgame->m_pCamera->SetTarget(Vector3(0, 0, 0));
    halgame->m_pCamera->Approach(-10);
    halgame->m_pCamera->SetRotationX(-1);
}

void GameoverScene::Update() {
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        halgame->SetScene<TitleScene>();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_Space)) {
        halgame->SetScene<TitleScene>();
    }
}

void GameoverScene::Uninit() {

}