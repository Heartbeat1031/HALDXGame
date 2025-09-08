//
// Created by lclcl on 25-5-22.
//

#include "Idol.h"

#include <iostream>

#include "AnimatorC.h"
#include "BoxCollisionC.h"
#include "GamePlayScene.h"
#include "MixamorigBoneC.h"
#include "ModelC.h"
#include "TransformC.h"

Idol::Idol() {
}

Idol::~Idol() {
}

void Idol::Init() {
    GameObject::Init();
    // ModelC コンポーネントを追加し、モデルをロードします
    AddComponent<ModelC>("assets/models/player/Idol.fbx");
    AddComponent<MixamorigBoneC>();
    // TransformC コンポーネントを取得し、ローカル位置を設定します
    AddComponent<AnimatorC>("Dance");
    GetComponentRef<TransformC>().SetLocalScale(Vector3(0.025f, 0.025f, 0.025f));
    BoxCollisionC &boxCollision = AddComponent<BoxCollisionC>( JPH::EMotionType::Dynamic, Vector3(1, 2, 1));
    boxCollision.SetOffsetTransform(OffsetTransform{
        DirectX::SimpleMath::Vector3(0, 2, 0),
        DirectX::SimpleMath::Vector3(0, 0, 0)
    });
}

void Idol::Uninit() {
    GameObject::Uninit();
}

void Idol::BeAttacked(int damage) {
    HP -= damage;
    std::cout << "Idolは" << damage << "のダメージを受けました。残りHP: " << HP << std::endl;
}

void Idol::Update(float dt) {
    GameObject::Update(dt);
    if (GetComponentRef<TransformC>().GetWorldPosition().y < -10.0f) {
        GetComponentRef<BoxCollisionC>().SetPosition(Vector3(0, 5, 0));
    }
}