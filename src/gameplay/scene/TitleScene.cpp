#include "TitleScene.h"

#include "GameObject.h"
#include "GamePlayScene.h"
#include "ImageC.h"

void TitleScene::Init() {
    GameObject &bg = AddGameObject<GameObject>();
    bg.SetName("Background");
    ImageC &bgImage = bg.AddComponent<ImageC>("assets/Texture/title.png");
    bgImage.SetSize({1280, 720});
    startFlagIndex = 10;
    isStart = false;

    // GameObject &server = AddGameObject<GameObject>();
    // server.SetName("Server");
    // ServerC &server_c = server.AddComponent<ServerC>();
    // server_c.Port = 12345;
    //server_c.Init();

}

void TitleScene::Update(float dt) {
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
