//
// Created by clcc777 on 25-6-5.
//

#pragma once
#include <DirectXMath.h>
#include <string>

#include "Component.h"
#include "ModelManager.h"
#include "ModelObject.h"


class AnimatorC: public Component {
private:
    std::string m_animatorName;
    // 动画播放时间
    double m_currentTime = 0.0;
    const Model *m_model = nullptr;
    std::vector<DirectX::XMFLOAT4X4> m_finalBoneMatrices;
    std::vector<DirectX::XMMATRIX> m_cachedOffset;
    std::vector<DirectX::XMMATRIX> m_cachedNode;
    std::vector<int> m_rootBones;
    UID m_modelHandle = -1;
    ModelObject* m_modelObject = nullptr;
public:
    explicit AnimatorC(const std::string &defaultAnim = "Idle");
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;

    void Play(const std::string& animName);
    std::vector<DirectX::XMFLOAT4X4>& GetFinalBoneMatrices();
};
