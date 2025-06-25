//
// Created by lclcl on 25-5-22.
//

#include "House.h"

#include "AnimatorC.h"
#include "BoxCollisionC.h"
#include "MixamorigBoneC.h"
#include "ModelC.h"
#include "TransformC.h"

House::House() {
}

House::~House() {
}

void House::Init() {
    GameObject::Init();
    // ModelC コンポーネントを追加し、モデルをロードします
    AddComponent<ModelC>("assets/models/player/Idol.fbx");
    AddComponent<MixamorigBoneC>();
    // TransformC コンポーネントを取得し、ローカル位置を設定します
    AddComponent<AnimatorC>("Singing");
    GetComponentRef<TransformC>().SetLocalScale(Vector3(0.025f, 0.025f, 0.025f));
    BoxCollisionC &boxCollision = AddComponent<BoxCollisionC>( JPH::EMotionType::Dynamic, Vector3(1, 2, 1));
    boxCollision.SetOffsetTransform(OffsetTransform{
        DirectX::SimpleMath::Vector3(0, 2, 0),
        DirectX::SimpleMath::Vector3(0, 0, 0)
    });
}

void House::Uninit() {
    GameObject::Uninit();
}

void House::Update(float dt) {
    GameObject::Update(dt);
    if (GetComponentRef<TransformC>().GetWorldPosition().y < -10.0f) {
        GetComponentRef<BoxCollisionC>().SetPosition(Vector3(0, 5, 0));
    }
}
