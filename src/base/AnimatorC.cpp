//
// Created by clcc777 on 25-6-5.
//

#include "AnimatorC.h"

#include <iostream>

#include "AnimationManager.h"
#include "Global.h"
#include "ModelC.h"


DirectX::XMMATRIX CalcInterpolatedBoneMatrix(
    const std::string& boneName,
    const AnimationClip& clip,
    double time
) {
    auto it = clip.boneAnimations.find(boneName);
    if (it == clip.boneAnimations.end() || it->second.keyframes.empty())
        return DirectX::XMMatrixIdentity();

    // 简化：找两个相邻的关键帧做线性插值
    const auto& keyframes = it->second.keyframes;
    const Keyframe* prev = &keyframes.front();
    const Keyframe* next = &keyframes.back();
    for (size_t i = 1; i < keyframes.size(); ++i) {
        if (time < keyframes[i].time) {
            next = &keyframes[i];
            prev = &keyframes[i - 1];
            break;
        }
    }
    double t = (next->time - prev->time) > 0 ? (time - prev->time) / (next->time - prev->time) : 0.0;

    // 线性插值
    auto lerp3 = [](const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, float t) {
        return DirectX::XMFLOAT3(
            a.x + (b.x - a.x) * t,
            a.y + (b.y - a.y) * t,
            a.z + (b.z - a.z) * t
        );
    };
    auto slerp4 = [](const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b, float t) {
        DirectX::XMVECTOR va = DirectX::XMLoadFloat4(&a);
        DirectX::XMVECTOR vb = DirectX::XMLoadFloat4(&b);
        DirectX::XMVECTOR vout = DirectX::XMQuaternionSlerp(va, vb, t);
        DirectX::XMFLOAT4 out;
        DirectX::XMStoreFloat4(&out, vout);
        return out;
    };
    DirectX::XMFLOAT3 trans = lerp3(prev->translation, next->translation, (float)t);
    DirectX::XMFLOAT3 scale = lerp3(prev->scaling, next->scaling, (float)t);
    DirectX::XMFLOAT4 rot = slerp4(prev->rotation, next->rotation, (float)t);

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
    m_modelHandle = m_gameObject->GetComponent<ModelC>().handle;
    if (m_modelHandle == -1) {
        throw std::runtime_error("ゲームオブジェクトにモデルコンポーネントがありません");
    }
    m_modelObject = &halgame->GetModelObject(m_modelHandle);
    m_model = m_modelObject->GetModel();
}

void AnimatorC::Update(float dt) {
    Component::Update(dt);
    // 获取当前动画clip
    auto& clip = halgame->animationManager.GetAnimation(m_animatorName);
    m_currentTime += dt;
    // 循环动画
    if (m_currentTime > clip.duration) m_currentTime = fmod(m_currentTime, clip.duration);

    // 骨骼变换缓存resize
    size_t boneCount = m_model->bones.size();
    if (m_finalBoneMatrices.size() != boneCount) {
        DirectX::XMFLOAT4X4 identity;
        DirectX::XMStoreFloat4x4(&identity, DirectX::XMMatrixIdentity());
        m_finalBoneMatrices.resize(boneCount, identity);
    }

    // 递归刷新骨骼变换（插值动画，父子递归）
    std::function<void(int, const DirectX::XMMATRIX&)> updateBone;
    std::vector<bool> visited(boneCount, false);
    updateBone = [&](int boneIdx, const DirectX::XMMATRIX& parentMat) {
        if (boneIdx < 0 || boneIdx >= (int)boneCount)
            return;
        if (visited[boneIdx])
            return; // 防止骨骼层级出现环导致死循环
        visited[boneIdx] = true;
        auto& boneInfo = m_model->bones[boneIdx];
        DirectX::XMMATRIX localMat = XMLoadFloat4x4(&boneInfo.nodeTransform);
        if (clip.boneAnimations.count(boneInfo.name))
            localMat = CalcInterpolatedBoneMatrix(boneInfo.name, clip, m_currentTime);
        DirectX::XMMATRIX globalMat = localMat * parentMat;
        DirectX::XMMATRIX offset = XMLoadFloat4x4(&boneInfo.offsetMatrix);
        DirectX::XMMATRIX finalMat = offset * globalMat; // 注意乘法顺序！
        DirectX::XMStoreFloat4x4(&m_finalBoneMatrices[boneIdx], finalMat);
        for (int childIdx : boneInfo.children)
            updateBone(childIdx, globalMat);
    };

    //遍历所有root骨骼
    for (int i = 0; i < boneCount; ++i) {
        if (m_model->bones[i].parentIndex == -1) {
            updateBone(i, DirectX::XMMatrixIdentity());
        }
    }

    // 打印调试信息
    std::cout << "AnimatorC: " << m_animatorName << " at time: " << m_currentTime << "\n";
    for (int i = 0; i < (int)m_finalBoneMatrices.size(); ++i) {
        DirectX::XMFLOAT4X4& mat = m_finalBoneMatrices[i];
        std::cout << "Bone " << i << ": "
                  << mat._11 << ", " << mat._12 << ", " << mat._13 << ", " << mat._14 << "\n"
                  << mat._21 << ", " << mat._22 << ", " << mat._23 << ", " << mat._24 << "\n"
                  << mat._31 << ", " << mat._32 << ", " << mat._33 << ", " << mat._34 << "\n"
                  << mat._41 << ", " << mat._42 << ", " << mat._43 << ", " << mat._44 << "\n";
    }
    std::cout << "Total bones: " << boneCount << "\n";

    // 设置模型对象的骨骼矩阵
    if (m_modelObject) {
        m_modelObject->SetBoneMatrices(&m_finalBoneMatrices);
    }
}

void AnimatorC::Uninit() {
    Component::Uninit();
    if (m_modelObject)
        m_modelObject->SetBoneMatrices(nullptr);
}

void AnimatorC::Play(const std::string& animName) {
    m_animatorName = animName;
    m_currentTime = 0.0;
}

const std::vector<DirectX::XMFLOAT4X4> & AnimatorC::GetFinalBoneMatrices() const {
    return  m_finalBoneMatrices;
}

