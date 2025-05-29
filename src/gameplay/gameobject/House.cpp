//
// Created by lclcl on 25-5-22.
//

#include "House.h"
#include "ModelComponent.h"

House::House() {
}

House::~House() {
}

void House::Init() {
    AddComponent<ModelComponent>()->Load("assets\\models\\house.obj");
    // SetScale(Vector3(0.015f, 0.015f, 0.015f));
    // SetPosition(Vector3(0, -1, 0));

}

void House::Uninit() {
}


void House::Update(float dt) {

    // オブジェクト移動のサンプル
    Vector3 v3;
    if (ImGui::IsKeyDown(ImGuiKey_W)) {
        v3 += {0, 0, 1};
    }
    if (ImGui::IsKeyDown(ImGuiKey_S)) {
        v3 += {0, 0, -1};
    }
    if (ImGui::IsKeyDown(ImGuiKey_A)) {
        v3 += {-1, 0, 0};
    }
    if (ImGui::IsKeyDown(ImGuiKey_D)) {
        v3 += {1, 0, 0};
    }
    //SetPosition(GetPosition() + v3 * 2  * dt);
}
