#include "TitleScene.h"

#include "GameObject.h"
#include "ModelC.h"
#include "GamePlayScene.h"

void TitleScene::Init() {
    auto &screen = AddGameObject<GameObject>();
    screen.AddComponent<ModelC>("assets/models/bg.fbx");
    screen.GetComponentRef<TransformC>().SetWorldPosition(Vector3(0, 0, 0));
    screen.GetComponentRef<TransformC>().SetLocalScale(Vector3(3.8f, 3.8f, 1));

    halgame->m_pCamera->SetTarget(Vector3(0, 0, 0));
    halgame->m_pCamera->Approach(-10);
    halgame->m_pCamera->SetRotationX(-1);
    halgame->m_pCamera->SetRotationY(0);
    startFlagIndex = 10;
    isStart = false;
}

void TitleScene::Update() {
    if (ImGui::IsKeyReleased(ImGuiKey_Enter)) {
        isStart = true;
        startFlagIndex = 10;
    }
    if (isStart) {
        startFlagIndex--;
        if (startFlagIndex <= 0) {
            halgame->SetScene<GamePlayScene>();
        }
    }

    ImGui::Begin("TitleUI",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoBackground);

    if (!isStart) {
        ImGui::Text("Enterキーを押してゲームを開始");
    } else {
        ImGui::Text("ゲームを読み込み中 ...");
    }
    ImGui::End();
}

void TitleScene::Uninit() {
}
