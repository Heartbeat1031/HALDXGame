//
// Created by lclcl on 25-6-26.
//

#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    // 弾丸の速度
    float m_speed = 10.0f; // m/s
    // 弾丸の方向
    DirectX::SimpleMath::Vector3 m_direction = DirectX::SimpleMath::Vector3(0, 0, 1); // 前方方向
public:
    Bullet(DirectX::SimpleMath::Vector3 direction, float speed = 10.0f);

    ~Bullet() override;

    void Init() override;

    void Update(float dt) override;

    void OnContactAdded(CollisionC &my, CollisionC &other, const JPH::ContactManifold &inManifold,
                        JPH::ContactSettings &ioSettings) override;

    void Uninit() override;
};
