//
// Created by lclcl on 25-5-29.
//
#pragma once

#include <SimpleMath.h>
#include <vector>
#include "Component.h"

class TransformC : public Component {

private:
    DirectX::SimpleMath::Vector3 m_localPosition;
    DirectX::SimpleMath::Quaternion m_localRotation;
    // 用于存储欧拉角
    DirectX::SimpleMath::Vector3 m_localEuler;
    DirectX::SimpleMath::Vector3 m_localScale;

    DirectX::SimpleMath::Matrix m_localMatrix;
    DirectX::SimpleMath::Matrix m_worldMatrix;
    bool m_dirty;

    TransformC* m_parent;
    std::vector<TransformC*> m_Childs;

    void UpdateTransform();
    void RecalculateLocalMatrix();

public:
    TransformC(GameObject* parent);
    ~TransformC() override = default;
    void Uninit() override;
    // 设置局部属性
    void SetLocalPosition(const DirectX::SimpleMath::Vector3& position);
    void SetLocalRotation(const DirectX::SimpleMath::Quaternion& rotation);
    void SetLocalRotationEuler(const DirectX::SimpleMath::Vector3& eulerRadians);
    void SetLocalScale(const DirectX::SimpleMath::Vector3& scale);

    // 获取局部属性
    DirectX::SimpleMath::Vector3 GetLocalPosition() const;
    DirectX::SimpleMath::Quaternion GetLocalRotation() const;
    DirectX::SimpleMath::Vector3 GetLocalRotationEuler() const;
    DirectX::SimpleMath::Vector3 GetLocalScale() const;

    // 获取全局属性
    DirectX::SimpleMath::Matrix GetWorldMatrix();
    DirectX::SimpleMath::Vector3 GetWorldPosition();
    DirectX::SimpleMath::Quaternion GetWorldRotation();
    DirectX::SimpleMath::Vector3 GetWorldRotationEuler();
    DirectX::SimpleMath::Vector3 GetWorldScale();

    // 父子节点管理
    void SetParent(TransformC* newParent, bool keepWorld = true);
    TransformC* GetParent() const { return m_parent; }
    void AddChild(TransformC* child, bool keepWorld = true);
    void RemoveChild(TransformC* child);
};
