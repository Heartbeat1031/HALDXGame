//
// Created by lclcl on 25-5-22.
//
#pragma once
#include "GameObject.h"
//----------------------------------------------------------------//
// House类是一个示例游戏对象，代表一个房子
// Houseクラスは、ゲームオブジェクトの一例であり、家を表します
//----------------------------------------------------------------//

class House:public GameObject {
public:
    House();
    ~House() override;
    void Init() override;
    void Update(float dt) override;

    void OnContactAdded(CollisionC &my, CollisionC &other, const JPH::ContactManifold &inManifold,
                        JPH::ContactSettings &ioSettings) override;

    void Uninit() override;
};
