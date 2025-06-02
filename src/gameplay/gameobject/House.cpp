//
// Created by lclcl on 25-5-22.
//

#include "House.h"
#include "ModelC.h"
#include "TransformC.h"

House::House() {
}

House::~House() {
}

void House::Init() {
    GameObject::Init();
    // ModelC コンポーネントを追加し、モデルをロードします
    AddComponent<ModelC>().Load("assets\\models\\house.obj");
    // TransformC コンポーネントを取得し、ローカル位置を設定します
    GetComponent<TransformC>().SetLocalScale(Vector3(0.015f, 0.015f, 0.015f));
}

void House::Uninit() {
    GameObject::Uninit();
}

void House::Update(float dt) {
    GameObject::Update(dt);
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

    auto &transform = GetComponent<TransformC>();
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        transform.SetLocalRotationEuler(transform.GetLocalRotationEuler() +
                                        Vector3(
                                            io.MouseDelta.y * -1.0f,
                                            io.MouseDelta.x * -1.0f,
                                            0.0f
                                        ) * dt);
    }
    transform.SetLocalPosition(transform.GetLocalPosition() + v3 * 2 * dt);

    if (ImGui::IsKeyPressed(ImGuiKey_R)) {
        halgame->GetScene()->RemoveGameObject(GetUID());
    }
}
