//
// Created by lclcl on 25-5-30.
//

#pragma once
#include "GameObject.h"


class Miku:public GameObject {
private:
    int m_score = 0;
public:
    Miku();
    ~Miku() override;
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;

    void OnContactAdded(CollisionC &my, CollisionC &other, const JPH::ContactManifold &inManifold, JPH::ContactSettings &ioSettings) override;
};
