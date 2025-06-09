//
// Created by clcc777 on 25-6-10.
//

#pragma once
#include "Component.h"

class MixamorigBoneC: public Component {
public:
    MixamorigBoneC();
    ~MixamorigBoneC() override;

    void Init() override;
    void Uninit() override;
    void Update(float dt) override;
};
