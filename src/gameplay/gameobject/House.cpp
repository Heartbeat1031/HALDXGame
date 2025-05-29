//
// Created by lclcl on 25-5-22.
//

#include "House.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

House::House() {
}

House::~House() {
}

void House::Init() {
    auto &transform = AddComponent<TransformComponent>();
    transform.SetLocalPosition({0.0f, 0.0f, 0.0f});
    transform.SetLocalRotation({0.0f, 0.0f, 0.0f, 1.0f});
    transform.SetLocalScale(Vector3(0.015f, 0.015f, 0.015f));
    auto &model = AddComponent<ModelComponent>();
    model.Load("assets\\models\\house.obj");

}

void House::Uninit() {
}


void House::Update(float dt) {

    // オブジェクト移動のサンプル
    ImGuiIO &io = ImGui::GetIO();
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

    auto &transform = GetComponent<TransformComponent>();
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        transform.SetLocalRotationEuler(transform.GetLocalRotationEuler() +
            Vector3(
            io.MouseDelta.y * -1.0f,
            io.MouseDelta.x * -1.0f,
            0.0f
            ) * dt);
    }
    transform.SetLocalPosition(transform.GetLocalPosition() + v3 * 2  * dt);
}
