//
// Created by lclcl on 25-5-29.
//

#include "TransformComponent.h"

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

void TransformComponent::SetLocalPosition(const Vector3 &position) {
    m_localPosition = position;
    m_dirty = true;
}

void TransformComponent::SetLocalRotation(const Quaternion &rotation) {
    m_localRotation = rotation;
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
        // for (auto &child: m_children) {
        //     child->m_dirty = true;
        //     child->UpdateTransform();
        // }
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

void TransformComponent::AddChild(std::shared_ptr<TransformComponent> child) {
    child->SetParent(this);
    //m_children.push_back(child);
}

void TransformComponent::RemoveChild(TransformComponent *child) {
    // m_children.erase(std::remove_if(m_children.begin(), m_children.end(),
    //                                 [child](const std::shared_ptr<TransformComponent> &c) {
    //                                     return c.get() == child;
    //                                 }), m_children.end());
}
