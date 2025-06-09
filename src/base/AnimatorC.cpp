//
// Created by clcc777 on 25-6-5.
//

#include "AnimatorC.h"

#include <iostream>

#include "AnimationManager.h"
#include "Global.h"
#include "ModelC.h"

DirectX::XMMATRIX CalcInterpolatedBoneMatrix(
    const std::string &boneName,
    const AnimationClip &clip,
    double time
) {
    auto it = clip.boneAnimations.find(boneName);
    if (it == clip.boneAnimations.end() || it->second.keyframes.empty())
        return DirectX::XMMatrixIdentity();

    // 简化：找两个相邻的关键帧做线性插值
    const auto &keyframes = it->second.keyframes;
    const Keyframe *prev = &keyframes.front();
    const Keyframe *next = &keyframes.back();
    for (size_t i = 1; i < keyframes.size(); ++i) {
        if (time < keyframes[i].time) {
            next = &keyframes[i];
            prev = &keyframes[i - 1];
            break;
        }
    }
    double t = (next->time - prev->time) > 0 ? (time - prev->time) / (next->time - prev->time) : 0.0;

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
    DirectX::XMFLOAT3 trans = lerp3(prev->translation, next->translation, (float) t);
    DirectX::XMFLOAT3 scale = lerp3(prev->scaling, next->scaling, (float) t);
    DirectX::XMFLOAT4 rot = slerp4(prev->rotation, next->rotation, (float) t);

    // 组装为变换矩阵
    return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) *
           DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rot)) *
           DirectX::XMMatrixTranslation(trans.x, trans.y, trans.z);
}

AnimatorC::AnimatorC(std::string defaultAnim) {
    m_animatorName = defaultAnim;
}

void AnimatorC::Init() {
    Component::Init();
    // 获取模型组件
    m_modelHandle = m_gameObject->GetComponent<ModelC>().GetHandle();
    if (m_modelHandle == -1) {
        throw std::runtime_error("ゲームオブジェクトにモデルコンポーネントがありません");
    }
    m_modelObject = &halgame->GetModelObject(m_modelHandle);
    m_model = m_modelObject->GetModel();

    size_t boneCount = m_model->bones.size();
    m_cachedOffset.resize(boneCount);
    m_cachedNode.resize(boneCount);
    m_rootBones.clear();
    for (size_t i = 0; i < boneCount; ++i) {
        m_cachedOffset[i] = DirectX::XMLoadFloat4x4(&m_model->bones[i].offsetMatrix);
        m_cachedNode[i] = DirectX::XMLoadFloat4x4(&m_model->bones[i].nodeTransform);
        if (m_model->bones[i].parentIndex == -1)
            m_rootBones.push_back(static_cast<int>(i));
    }
    DirectX::XMFLOAT4X4 identity;
    DirectX::XMStoreFloat4x4(&identity, DirectX::XMMatrixIdentity());
    m_finalBoneMatrices.assign(boneCount, identity);
}

void AnimatorC::Update(float dt) {
    Component::Update(dt);
    // 获取当前动画clip
    auto &clip = halgame->animationManager.GetAnimation(m_animatorName);
    m_currentTime += dt;
    // 循环动画
    if (m_currentTime > clip.duration) {
        m_currentTime = fmod(m_currentTime, clip.duration);
    }

    struct NodeState { int idx; DirectX::XMMATRIX parentMat; };
    std::vector<NodeState> stack;
    for (int i : m_rootBones) {
        stack.push_back({i, DirectX::XMMatrixIdentity()});
    }

    while (!stack.empty()) {
        NodeState cur = stack.back();
        stack.pop_back();
        int idx = cur.idx;
        const auto& boneInfo = m_model->bones[idx];

        DirectX::XMMATRIX local = m_cachedNode[idx];
        if (clip.boneAnimations.count(boneInfo.name)) {
            local = CalcInterpolatedBoneMatrix(boneInfo.name, clip, m_currentTime);
        }
        DirectX::XMMATRIX global = local * cur.parentMat;
        DirectX::XMMATRIX finalMat = m_cachedOffset[idx] * global;
        DirectX::XMStoreFloat4x4(&m_finalBoneMatrices[idx], finalMat);

        for (int child : boneInfo.children) {
            stack.push_back({child, global});
        }
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

std::vector<DirectX::XMFLOAT4X4> &AnimatorC::GetFinalBoneMatrices() {
    return m_finalBoneMatrices;
}
