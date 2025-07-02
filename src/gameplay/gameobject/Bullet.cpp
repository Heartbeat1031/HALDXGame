//
// Created by lclcl on 25-6-26.
//

#include "Bullet.h"

#include "CapsuleCollisionC.h"
#include "ModelC.h"
#include <DirectXMath.h>

Bullet::Bullet(DirectX::SimpleMath::Vector3 direction, float speed) {
    m_direction = direction;
    m_speed = speed;
}

Bullet::~Bullet() {
}

void Bullet::Init() {
    GameObject::Init();
    // ModelC コンポーネントを追加し、モデルをロードします
    AddComponent<ModelC>("assets/models/Bullet.fbx");
    TransformC &transform = GetComponentRef<TransformC>();
    transform.SetLocalScale(Vector3(0.2f, 0.2f, 0.2f)); // 弾丸のサイズを調整
    CapsuleCollisionC &boxC = AddComponent<CapsuleCollisionC>(JPH::EMotionType::Dynamic, 0.08f, 0.1f);
    boxC.SetGravityFactor(0);
    boxC.SetOffsetRotation(Vector3(DirectX::XM_PI / 2, 0.0f, 0.0f));
}

void Bullet::Update(float dt) {
    GameObject::Update(dt);
}

void Bullet::OnContactAdded(CollisionC &my, CollisionC &other, const JPH::ContactManifold &inManifold,
                            JPH::ContactSettings &ioSettings) {
    GameObject::OnContactAdded(my, other, inManifold, ioSettings);
}

void Bullet::Uninit() {
    GameObject::Uninit();
}
