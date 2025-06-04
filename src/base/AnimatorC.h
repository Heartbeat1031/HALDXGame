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
public:
    explicit AnimatorC(std::string defaultAnim = "Idle");
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;

    void Play(const std::string& animName);
    const std::vector<DirectX::XMFLOAT4X4>& GetFinalBoneMatrices() const;
};
