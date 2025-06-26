//
// Created by lclcl on 25-6-26.
//

#include "Bullet.h"

#include "BoxCollisionC.h"
#include "ModelC.h"

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
   //AddComponent<BoxCollisionC>(JPH::EMotionType::Dynamic, Vector3(1, 1, 1));
    TransformC &transform = GetComponentRef<TransformC>();
    transform.SetLocalScale(Vector3(1.0f, 1.0f, 1.0f)); // 弾丸のサイズを調整
}

void Bullet::Update(float dt) {
    GameObject::Update(dt);
    // 弾丸の位置を更新するロジックをここに追加できます
    // 例えば、弾丸の速度に基づいて位置を更新するなど
    TransformC &transform = GetComponentRef<TransformC>();
    Vector3 position = transform.GetWorldPosition();
    position += m_direction * m_speed * dt; // 弾丸の移動
    transform.SetWorldPosition(position);
    // 弾丸の向きを更新
    float yaw = std::atan2(-m_direction.x, -m_direction.z);
    transform.SetWorldRotation(
        Quaternion::CreateFromYawPitchRoll(
            yaw,
            0.0f,
            0.0f
        )
    );
}

void Bullet::OnContactAdded(CollisionC &my, CollisionC &other, const JPH::ContactManifold &inManifold,
                            JPH::ContactSettings &ioSettings) {
    GameObject::OnContactAdded(my, other, inManifold, ioSettings);

}

void Bullet::Uninit() {
    GameObject::Uninit();
}
