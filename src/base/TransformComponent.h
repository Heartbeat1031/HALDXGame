//
// Created by lclcl on 25-5-29.
//
#pragma once

#include <SimpleMath.h>
#include <vector>
#include <memory>
#include "Component.h"

class TransformComponent : public Component {

private:
    DirectX::SimpleMath::Vector3 m_localPosition;
    DirectX::SimpleMath::Quaternion m_localRotation;
    DirectX::SimpleMath::Vector3 m_localScale;

    DirectX::SimpleMath::Matrix m_localMatrix;
    DirectX::SimpleMath::Matrix m_worldMatrix;
    bool m_dirty;

    TransformComponent* m_parent;
    std::vector<std::shared_ptr<TransformComponent>> m_children;

    void UpdateTransform();
    void RecalculateLocalMatrix();

public:
    TransformComponent(GameObject* parent);
    ~TransformComponent() override = default;

    // 设置局部属性
    void SetLocalPosition(const DirectX::SimpleMath::Vector3& position);
    void SetLocalRotation(const DirectX::SimpleMath::Quaternion& rotation);
    void SetLocalScale(const DirectX::SimpleMath::Vector3& scale);

    // 获取局部属性
    DirectX::SimpleMath::Vector3 GetLocalPosition() const;
    DirectX::SimpleMath::Quaternion GetLocalRotation() const;
    DirectX::SimpleMath::Vector3 GetLocalScale() const;

    // 获取全局属性
    DirectX::SimpleMath::Matrix GetWorldMatrix();
    DirectX::SimpleMath::Vector3 GetWorldPosition();
    DirectX::SimpleMath::Quaternion GetWorldRotation();
    DirectX::SimpleMath::Vector3 GetWorldScale();

    // 父子节点管理
    void SetParent(TransformComponent* newParent);
    void AddChild(std::shared_ptr<TransformComponent> child);
    void RemoveChild(TransformComponent* child);
};
