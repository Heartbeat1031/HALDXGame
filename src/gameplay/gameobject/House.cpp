//
// Created by lclcl on 25-5-22.
//

#include "House.h"

#include "BoxCollisionC.h"
#include "ModelC.h"
#include "TransformC.h"

House::House() {
}

House::~House() {
}

void House::Init() {
    GameObject::Init();
    // ModelC コンポーネントを追加し、モデルをロードします
    AddComponent<ModelC>("assets\\models\\house.obj");
    // TransformC コンポーネントを取得し、ローカル位置を設定します
    GetComponent<TransformC>().SetLocalScale(Vector3(0.015f, 0.015f, 0.015f));
    AddComponent<BoxCollisionC>(Vector3(2, 2, 2), JPH::EMotionType::Dynamic).SetOffset(Vector3(0, 2, 0));
}

void House::Uninit() {
    GameObject::Uninit();
}

void House::Update(float dt) {
    GameObject::Update(dt);
    ImGuiIO &io = ImGui::GetIO();
    auto &boxCollision = GetComponent<BoxCollisionC>();
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        boxCollision.SetRotationEuler(
            boxCollision.GetRotationEuler() +
            Vector3(
                io.MouseDelta.y * -1.0f,
                io.MouseDelta.x * -1.0f,
                0.0f
            ) * dt
        );
    }

    if (GetComponent<TransformC>().GetWorldPosition().y < -10.0f) {
        GetComponent<BoxCollisionC>().SetPosition(Vector3(0, 5, 0));
    }
}

