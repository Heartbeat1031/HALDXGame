//
// Created by lclcl on 25-5-22.
//
#pragma once
#include "GameObject.h"


class House:public GameObject {
public:
    House();
    ~House() override;
    void Init() override;
    void Update(float dt) override;
};
