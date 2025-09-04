#include "TitleScene.h"

#include "GameObject.h"
#include "ModelC.h"
#include "TestScene.h"

void TitleScene::Init() {
    auto& screen = AddGameObject<GameObject>();
    screen.AddComponent<ModelC>("assets/models/bg.fbx");
    screen.GetComponentRef<TransformC>().SetWorldPosition(Vector3(0, 0, 0));
    screen.GetComponentRef<TransformC>().SetLocalScale(Vector3(3.8f, 3.8f, 1));

    halgame->m_pCamera->SetTarget(Vector3(0, 0, 0));
    halgame->m_pCamera->Approach(-10);
    halgame->m_pCamera->SetRotationX(-1);
}

void TitleScene::Update() {
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
        halgame->SetScene<TestScene>();
    }
}

void TitleScene::Uninit() {

}