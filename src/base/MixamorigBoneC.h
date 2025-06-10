//
// Created by clcc777 on 25-6-10.
//

#pragma once
#include <DirectXMath.h>

#include "BoneObj.h"
#include "Component.h"

class MixamorigBoneC: public Component {
private:
    std::vector<DirectX::XMFLOAT4X4> m_finalBoneMatrices;
    BoneObj *m_hipsBone = nullptr;
public:
    MixamorigBoneC();
    ~MixamorigBoneC() override;

    void Init() override;
    void Uninit() override;
    void Update(float dt) override;


    std::vector<DirectX::XMFLOAT4X4>& GetFinalBoneMatrices();
};
