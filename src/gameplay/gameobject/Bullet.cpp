//
// Created by lclcl on 25-6-26.
//

#include "Bullet.h"

#include "CapsuleCollisionC.h"
#include "ModelC.h"
#include <DirectXMath.h>

Bullet::Bullet() {
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
    // 重力を無効にして、弾丸が重力の影響を受けないようにします
    //boxC.SetGravityFactor(0);
    boxC.SetOffsetRotation(Vector3(DirectX::XM_PI / 2, 0.0f, 0.0f));
}

void Bullet::Update(float dt) {
    GameObject::Update(dt);
    timeAlive += dt;
    // 弾丸の生存時間が一定時間を超えたら、弾丸を削除します
    if (timeAlive > 10.0f) { // 5秒後に弾丸を削除
        Destroy();
    }
}

void Bullet::OnContactAdded(CollisionC &my, CollisionC &other, const JPH::ContactManifold &inManifold,
                            JPH::ContactSettings &ioSettings) {
    GameObject::OnContactAdded(my, other, inManifold, ioSettings);
}

void Bullet::Uninit() {
    GameObject::Uninit();
}

void Bullet::Fire(DirectX::SimpleMath::Vector3 direction, float speed) {
    CapsuleCollisionC &boxC = GetComponentRef<CapsuleCollisionC>();

    // 归一化方向
    direction.Normalize();

    // 构造一个让 Y+ 朝向 direction 的旋转
    Vector3 forward = direction;
    Vector3 reference = Vector3::UnitZ; // 用 Z 轴作为参考构建正交坐标系
    Vector3 right = reference.Cross(forward);
    if (right.LengthSquared() < 1e-6f) {
        // 如果 direction 与 Z 平行，就换个参考轴
        reference = Vector3::UnitX;
        right = reference.Cross(forward);
    }
    right.Normalize();
    Vector3 up = forward.Cross(right);
    up.Normalize();

    DirectX::SimpleMath::Matrix m;
    m.Right(right);
    m.Up(forward);     // Y+ 朝向 direction
    m.Forward(up);
    m.Translation(Vector3::Zero);

    Quaternion targetRotation = Quaternion::CreateFromRotationMatrix(m);

    // 设置旋转和速度
    boxC.SetRotation(targetRotation);
    boxC.SetLinearVelocity(direction * speed);
}
