//
// Created by lclcl on 25-5-29.
//

#include "TransformComponent.h"

#include "Global.h"
#include "Scene.h"

using namespace DirectX::SimpleMath;

TransformComponent::TransformComponent(GameObject *parent)
    : Component(parent),
      m_localPosition(Vector3::Zero),
      m_localRotation(Quaternion::Identity),
      m_localScale(Vector3::One),
      m_localMatrix(Matrix::Identity),
      m_worldMatrix(Matrix::Identity),
      m_dirty(true),
      m_parent(nullptr) {
}

void TransformComponent::Uninit() {
    Component::Uninit();
    if (m_parent) {
        m_parent->RemoveChild(this); // 从父节点中移除自己
        m_parent = nullptr; // 解除父子关系
    }
    // 清理子节点
    for (TransformComponent *child : m_Childs) {
        child->SetParent(nullptr); // 解除父子关系
        UID childgameuid = child->m_gameobject->GetUID();
        halgame->GetScene()->RemoveGameObject(childgameuid);
    }
    m_Childs.clear(); // 清空子节点列表
}

void TransformComponent::SetLocalPosition(const Vector3 &position) {
    m_localPosition = position;
    m_dirty = true;
}

void TransformComponent::SetLocalRotation(const Quaternion &rotation) {
    m_localRotation = rotation;
    m_dirty = true;
    // 更新欧拉角缓存（以 Yaw-Pitch-Roll 解法）
    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;
    float w = rotation.w;

    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    float pitch = std::atan2(sinr_cosp, cosr_cosp);

    float sinp = 2.0f * (w * y - z * x);
    float yaw;
    if (std::abs(sinp) >= 1.0f)
        yaw = std::copysign(DirectX::XM_PIDIV2, sinp);
    else
        yaw = std::asin(sinp);

    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    float roll = std::atan2(siny_cosp, cosy_cosp);

    m_localEuler = Vector3(pitch, yaw, roll);
}

void TransformComponent::SetLocalRotationEuler(const Vector3& eulerRadians) {
    m_localEuler = eulerRadians;
    m_localRotation = Quaternion::CreateFromYawPitchRoll(eulerRadians.y, eulerRadians.x, eulerRadians.z);
    m_dirty = true;
}


void TransformComponent::SetLocalScale(const Vector3 &scale) {
    m_localScale = scale;
    m_dirty = true;
}

Vector3 TransformComponent::GetLocalPosition() const {
    return m_localPosition;
}

Quaternion TransformComponent::GetLocalRotation() const {
    return m_localRotation;
}

Vector3 TransformComponent::GetLocalRotationEuler() const {
    return m_localEuler;
}

Vector3 TransformComponent::GetLocalScale() const {
    return m_localScale;
}

void TransformComponent::RecalculateLocalMatrix() {
    m_localMatrix = Matrix::CreateScale(m_localScale) *
                    Matrix::CreateFromQuaternion(m_localRotation) *
                    Matrix::CreateTranslation(m_localPosition);
}

void TransformComponent::UpdateTransform() {
    if (m_dirty) {
        RecalculateLocalMatrix();
        if (m_parent) {
            m_worldMatrix = m_localMatrix * m_parent->GetWorldMatrix();
        } else {
            m_worldMatrix = m_localMatrix;
        }

        for (TransformComponent * transformChild : m_Childs) {
            transformChild->m_dirty = true;
            transformChild->UpdateTransform();
        }

        m_dirty = false;
    }
}

Matrix TransformComponent::GetWorldMatrix() {
    UpdateTransform();
    return m_worldMatrix;
}

Vector3 TransformComponent::GetWorldPosition() {
    UpdateTransform();
    return m_worldMatrix.Translation();
}

Quaternion TransformComponent::GetWorldRotation() {
    UpdateTransform();
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    m_worldMatrix.Decompose(scale, rotation, translation);
    return rotation;
}

Vector3 TransformComponent::GetWorldRotationEuler() {
    UpdateTransform();
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    m_worldMatrix.Decompose(scale, rotation, translation);

    // 使用四元数转换为欧拉角
    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;
    float w = rotation.w;

    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    float pitch = std::atan2(sinr_cosp, cosr_cosp);

    float sinp = 2.0f * (w * y - z * x);
    float yaw;
    if (std::abs(sinp) >= 1.0f)
        yaw = std::copysign(DirectX::XM_PIDIV2, sinp);
    else
        yaw = std::asin(sinp);

    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    float roll = std::atan2(siny_cosp, cosy_cosp);

    return Vector3(pitch, yaw, roll);
}

Vector3 TransformComponent::GetWorldScale() {
    UpdateTransform();
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    m_worldMatrix.Decompose(scale, rotation, translation);
    return scale;
}

void TransformComponent::SetParent(TransformComponent *newParent) {
    m_parent = newParent;
    m_dirty = true;
}

void TransformComponent::AddChild(TransformComponent *child) {
    child->SetParent(this);
    m_Childs.push_back(child);
}

void TransformComponent::RemoveChild(TransformComponent *child) {
    m_Childs.erase(
        std::remove(m_Childs.begin(), m_Childs.end(), child),
        m_Childs.end()
    );
}
