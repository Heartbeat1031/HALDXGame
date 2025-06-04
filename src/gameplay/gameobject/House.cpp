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
    GetComponent<TransformC>().SetLocalPosition(Vector3(0, 10, 0));
    AddComponent<BoxCollisionC>(Vector3(2, 2, 2), JPH::EMotionType::Dynamic).SetOffset(Vector3(0, 2, 0));
}

void House::Uninit() {
    GameObject::Uninit();
}

void House::Update(float dt) {
    GameObject::Update(dt);

    ImGuiIO &io = ImGui::GetIO();
    Vector3 moveDir = Vector3::Zero;
    int keyFlag = 0;
    auto &transform = GetComponent<TransformC>();

    // 1. 获取相机朝向
    Vector3 cameraForward(0,0,1), cameraRight(1,0,0);
    if (halgame && halgame->m_pCamera) {
        auto f = halgame->m_pCamera->GetLookAxis();
        auto r = halgame->m_pCamera->GetRightAxis();
        cameraForward = Vector3(f.x, 0, f.z); // 只取水平分量
        cameraRight   = Vector3(r.x, 0, r.z);
        if (cameraForward.LengthSquared() > 0.0001f) cameraForward.Normalize();
        if (cameraRight.LengthSquared() > 0.0001f) cameraRight.Normalize();
    }

    // 2. 组合移动向量
    if (ImGui::IsKeyDown(ImGuiKey_W)) {
        moveDir += cameraForward;
        keyFlag++;
    }
    if (ImGui::IsKeyDown(ImGuiKey_S)) {
        moveDir -= cameraForward;
        keyFlag++;
    }
    if (ImGui::IsKeyDown(ImGuiKey_A)) {
        moveDir -= cameraRight;
        keyFlag++;
    }
    if (ImGui::IsKeyDown(ImGuiKey_D)) {
        moveDir += cameraRight;
        keyFlag++;
    }
    if (moveDir.LengthSquared() > 0.0001f) moveDir.Normalize();

    auto &boxCollision = GetComponent<BoxCollisionC>();
    if (keyFlag > 0) {
        boxCollision.SetPosition(boxCollision.GetPosition() + moveDir * 5 * dt);
    }

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
}

