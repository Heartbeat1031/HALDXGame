#include "TitleScene.h"

#include "ClientManagerC.h"
#include "GameObject.h"
#include "GamePlayScene.h"
#include "ImageC.h"
#include "PacketIO.h"
#include "ServerManagerC.h"

void TitleScene::Init() {
    GameObject &bg = AddGameObject<GameObject>();
    bg.SetName("Background");
    ImageC &bgImage = bg.AddComponent<ImageC>("assets/Texture/title.png");
    bgImage.SetSize({1280, 720});
    startFlagIndex = 10;
    isStart = false;

     ServerManagerC &server_c = root->AddComponent<ServerManagerC>();
     server_c.Port = 8123;
    server_c.Start();

    ClientManagerC &client_c = root->AddComponent<ClientManagerC>();
    client_c.Hostname = "127.0.0.1";
    client_c.Port = 8123;
    client_c.Start();


    PacketWriter pw;
    pw.writeString("Hello, Server!");
    root->GetComponent<ClientManagerC>()->Send(pw);
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
