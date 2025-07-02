//
// Created by lclcl on 25-6-26.
//

#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    float timeAlive = 0.0f; // 弾丸の生存時間
public:
    Bullet();

    ~Bullet() override;

    void Init() override;

    void Update(float dt) override;

    void OnContactAdded(CollisionC &my, CollisionC &other, const JPH::ContactManifold &inManifold,
                        JPH::ContactSettings &ioSettings) override;

    void Uninit() override;

    void Fire(DirectX::SimpleMath::Vector3 direction, float speed = 10.0f);
};
