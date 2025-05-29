//
// Created by lclcl on 25-5-30.
//

#pragma once
#include "GameObject.h"


class Miku:public GameObject {
public:
    Miku();
    ~Miku() override;
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;
};
