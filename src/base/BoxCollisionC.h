//
// Created by clcc777 on 25-6-4.
//

#pragma once
#include "Component.h"

class BoxCollisionC: public Component {
public:
    BoxCollisionC(GameObject* parent) : Component(parent) {}
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;
};
