//
// Created by lclcl on 25-5-30.
//

#include "Miku.h"

#include "AnimatorC.h"
#include "BoxCollisionC.h"
#include "MixamorigBoneC.h"
#include "ModelC.h"
#include "TransformC.h"

Miku::Miku() {
}

Miku::~Miku() {
}

void Miku::Init() {
    GameObject::Init();
    // モデルコンポーネントを追加し、モデルをロードします
    AddComponent<ModelC>("assets/models/player/man.fbx");
    AddComponent<MixamorigBoneC>();

    // アニメーションコンポーネントを追加し、初期アニメーションを設定します
    AddComponent<AnimatorC>("Idle");
    GetComponentRef<TransformC>().SetLocalScale(Vector3(0.025f, 0.025f, 0.025f));
     BoxCollisionC &boxCollision = AddComponent<BoxCollisionC>(Vector3(1, 2, 1), JPH::EMotionType::Dynamic);
     boxCollision.SetOffset(Vector3(0, 2, 0));
    //AddComponent<MixamoRagdollC>();
}

void Miku::Update(float dt) {
    GameObject::Update(dt);

    // カメラの前方と右方向を取得
    Vector3 cameraVertical(0, 0, 1);
    Vector3 cameraHorizontal(1, 0, 0);
    if (halgame && halgame->m_pCamera) {
        auto f = halgame->m_pCamera->GetLookAxis();
        auto r = halgame->m_pCamera->GetRightAxis();
        cameraVertical = Vector3(f.x, 0, f.z);
        cameraHorizontal = Vector3(r.x, 0, r.z);
        if (cameraVertical.LengthSquared() > 0.0001f) cameraVertical.Normalize();
        if (cameraHorizontal.LengthSquared() > 0.0001f) cameraHorizontal.Normalize();
    }

    Vector3 moveDir = Vector3::Zero;
    // キー入力による移動方向を決定
    if (ImGui::IsKeyDown(ImGuiKey_W)) {
        moveDir += cameraVertical;
    }
    if (ImGui::IsKeyDown(ImGuiKey_S)) {
        moveDir -= cameraVertical;
    }
    if (ImGui::IsKeyDown(ImGuiKey_A)) {
        moveDir -= cameraHorizontal;
    }
    if (ImGui::IsKeyDown(ImGuiKey_D)) {
        moveDir += cameraHorizontal;
    }

    // moveDir がゼロでない場合、移動方向を正規化し、位置を更新
    if (moveDir.LengthSquared() > 0.0001f) {
        moveDir.Normalize();
        //物理オブジェクトの移動は BoxCollisionC コンポーネントを通じて行います
        if (HasComponent<BoxCollisionC>()) {
            auto &boxCollision = GetComponentRef<BoxCollisionC>();
            Vector3 targetPos = boxCollision.GetPosition() + moveDir * 5 * dt;
            boxCollision.SetPosition(targetPos);
            boxCollision.SetRotation(
                Quaternion::CreateFromYawPitchRoll(
                    std::atan2(-moveDir.x, -moveDir.z),
                    0.0f,
                    0.0f
                )
            );
        }else {
            auto &transform = GetComponentRef<TransformC>();
            Vector3 targetPos = transform.GetWorldPosition() + moveDir * 5 * dt;
            transform.SetWorldPosition(targetPos);
            transform.SetWorldRotation(
                Quaternion::CreateFromYawPitchRoll(
                    std::atan2(-moveDir.x, -moveDir.z),
                    0.0f,
                    0.0f
                )
            );
        }

        // 移動時のアニメーション
        if (HasComponent<AnimatorC>()) {
            GetComponentRef<AnimatorC>().Play("Walking");
        }
    } else {
        // 停止時のアニメーション
        if (HasComponent<AnimatorC>()) {
            GetComponentRef<AnimatorC>().Play("Idle");
        }
    }

    // 落下したら位置をリセット
    if (HasComponent<BoxCollisionC>()) {
        if (GetComponentRef<TransformC>().GetWorldPosition().y < -10.0f) {
            GetComponentRef<BoxCollisionC>().SetPosition(Vector3(0, 5, 0));
        }
    }

}

void Miku::Uninit() {
    GameObject::Uninit();
}
