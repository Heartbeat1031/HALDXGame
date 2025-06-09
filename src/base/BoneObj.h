//
// Created by clcc777 on 25-6-10.
//

#pragma once
#include "GameObject.h"

class BoneObj :public GameObject{
public:
    BoneObj();
    ~BoneObj() override;
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;
};
