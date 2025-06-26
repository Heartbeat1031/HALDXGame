//
// Created by clcc777 on 25-6-5.
//

#include "AnimatorC.h"

#include "AnimationManager.h"
#include "GameObject.h"
#include "Global.h"
#include "MixamorigBoneC.h"
#include "ModelC.h"

AnimatorC::AnimatorC(const std::string &defaultAnim) {
    m_animatorName = defaultAnim;
}

void AnimatorC::Init() {
    Component::Init();
}

void AnimatorC::Update(float dt) {
    Component::Update(dt);
    if (!HasComponent<MixamorigBoneC>()) {
        return;
    }
    // 获取当前动画clip
    const AnimationClip &clip = halgame->animationManager.GetAnimation(m_animatorName);
    m_currentTime += dt;
    // 循环动画
    if (m_currentTime > clip.duration) {
        m_currentTime = fmod(m_currentTime, clip.duration);
    }
    MixamorigBoneC &boneC = GetComponentRef<MixamorigBoneC>();
    auto boneNames = boneC.GetBoneNames();
    for (const auto &boneName : *boneNames) {
        auto BoneObj = boneC.GetBone(boneName);
        if (BoneObj == nullptr) {
            continue; // 骨骼不存在，跳过
        }
        TransformC &boneTransform = BoneObj->GetComponentRef<TransformC>();
        auto it = clip.boneAnimations.find(boneName);
        if (it == clip.boneAnimations.end() || it->second.keyframes.empty()) {
            continue;
        }
        const auto &keyframes = it->second.keyframes;
        const Keyframe *prev = &keyframes.front();
        const Keyframe *next = &keyframes.back();
        for (size_t i = 1; i < keyframes.size(); ++i) {
            if (m_currentTime < keyframes[i].time) {
                next = &keyframes[i];
                prev = &keyframes[i - 1];
                break;
            }
        }
        double t = (next->time - prev->time) > 0 ? (m_currentTime - prev->time) / (next->time - prev->time) : 0.0;

        // 线性插值
        auto lerp3 = [](const DirectX::XMFLOAT3 &a, const DirectX::XMFLOAT3 &b, float t) {
            return DirectX::XMFLOAT3(
                a.x + (b.x - a.x) * t,
                a.y + (b.y - a.y) * t,
                a.z + (b.z - a.z) * t
            );
        };
        auto slerp4 = [](const DirectX::XMFLOAT4 &a, const DirectX::XMFLOAT4 &b, float t) {
            DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
            DirectX::XMVECTOR vb = DirectX::XMLoadFloat4(&b);
            DirectX::XMVECTOR vout = DirectX::XMQuaternionSlerp(va, vb, t);
            DirectX::XMFLOAT4 out;
            DirectX::XMStoreFloat4(&out, vout);
            return out;
        };
        DirectX::XMFLOAT3 trans = lerp3(prev->translation, next->translation, static_cast<float>(t));
        DirectX::XMFLOAT3 scale = lerp3(prev->scaling, next->scaling, static_cast<float>(t));
        DirectX::XMFLOAT4 rot = slerp4(prev->rotation, next->rotation, static_cast<float>(t));

        boneTransform.SetLocalPosition(trans);
        boneTransform.SetLocalRotation(rot);
        boneTransform.SetLocalScale(scale);
    }
}

void AnimatorC::Uninit() {
    Component::Uninit();
}

void AnimatorC::Play(const std::string &animName) {
    if (m_animatorName == animName && m_currentTime > 0.0) {
        return;
    }
    m_animatorName = animName;
    m_currentTime = 0.0;
}
