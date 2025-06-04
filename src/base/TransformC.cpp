﻿//
// Created by lclcl on 25-5-29.
//

#include "TransformC.h"

#include "Global.h"
#include "Scene.h"

TransformC::TransformC()
    : m_localPosition(Vector3::Zero),
      m_localRotation(Quaternion::Identity),
      m_localScale(Vector3::One),
      m_localMatrix(DirectX::SimpleMath::Matrix::Identity),
      m_worldMatrix(DirectX::SimpleMath::Matrix::Identity),
      m_dirty(true),
      m_parent(nullptr) {
}

void TransformC::Uninit() {
    Component::Uninit();
    if (m_parent) {
        m_parent->RemoveChild(this); // 親ノードから自分自身を取り除く
        m_parent = nullptr; // 親子関係を解除する
    }
    // 子ノードをクリアする
    for (TransformC *child : m_Childs) {
        child->SetParent(nullptr); // 親子関係を解除する
        UID childgameuid = child->m_gameObject->GetUID();
        halgame->GetScene()->RemoveGameObject(childgameuid);
    }
    m_Childs.clear();
}

void TransformC::SetLocalPosition(const Vector3 &position) {
    m_localPosition = position;
    m_dirty = true;
}

void TransformC::SetLocalRotation(const Quaternion &rotation) {
    m_localRotation = rotation;
    m_dirty = true;
    // オイラー角キャッシュを更新（Yaw-Pitch-Roll の順で計算）
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

void TransformC::SetLocalRotationEuler(const Vector3& eulerRadians) {
    m_localEuler = eulerRadians;
    m_localRotation = Quaternion::CreateFromYawPitchRoll(eulerRadians.y, eulerRadians.x, eulerRadians.z);
    m_dirty = true;
}


void TransformC::SetLocalScale(const Vector3 &scale) {
    m_localScale = scale;
    m_dirty = true;
}

void TransformC::SetWorldPosition(const DirectX::SimpleMath::Vector3 &position) {
    UpdateTransform(); // 确保 world matrix 是最新的
    // 构造新的 world matrix 的 translation
    DirectX::SimpleMath::Matrix newWorldMatrix = m_worldMatrix;
    newWorldMatrix.Translation(position);

    if (m_parent) {
        DirectX::SimpleMath::Matrix parentWorldInv = m_parent->GetWorldMatrix().Invert();
        m_localMatrix = newWorldMatrix * parentWorldInv;
    } else {
        m_localMatrix = newWorldMatrix;
    }

    // 分解本地矩阵，提取新的 localPosition/Rotation/Scale
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    m_localMatrix.Decompose(scale, rotation, translation);

    m_localScale = scale;
    m_localRotation = rotation;
    SetLocalPosition(translation);
    SetLocalRotation(rotation); // 更新欧拉角缓存
    m_dirty = true;
}

void TransformC::SetWorldRotation(const DirectX::SimpleMath::Quaternion &rotation) {
    UpdateTransform(); // 确保 world matrix 是最新的

    // 获取当前世界的位置和缩放
    Vector3 position = GetWorldPosition();
    Vector3 scale = GetWorldScale();

    DirectX::SimpleMath::Matrix rotMatrix = DirectX::SimpleMath::Matrix::CreateFromQuaternion(rotation);
    DirectX::SimpleMath::Matrix scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale);
    DirectX::SimpleMath::Matrix transMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(position);

    DirectX::SimpleMath::Matrix newWorldMatrix = scaleMatrix * rotMatrix * transMatrix;

    if (m_parent) {
        DirectX::SimpleMath::Matrix parentWorldInv = m_parent->GetWorldMatrix().Invert();
        m_localMatrix = newWorldMatrix * parentWorldInv;
    } else {
        m_localMatrix = newWorldMatrix;
    }

    // 分解本地矩阵，提取新的 localPosition/Rotation/Scale
    Vector3 outScale;
    Quaternion outRotation;
    Vector3 outTranslation;
    m_localMatrix.Decompose(outScale, outRotation, outTranslation);

    m_localScale = outScale;
    m_localRotation = outRotation;
    SetLocalPosition(outTranslation);
    SetLocalRotation(outRotation); // 更新欧拉角缓存
    m_dirty = true;
}

Vector3 TransformC::GetLocalPosition() const {
    return m_localPosition;
}

Quaternion TransformC::GetLocalRotation() const {
    return m_localRotation;
}

Vector3 TransformC::GetLocalRotationEuler() const {
    return m_localEuler;
}

Vector3 TransformC::GetLocalScale() const {
    return m_localScale;
}

void TransformC::RecalculateLocalMatrix() {
    m_localMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_localScale) *
                    DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_localRotation) *
                    DirectX::SimpleMath::Matrix::CreateTranslation(m_localPosition);
}

void TransformC::UpdateTransform() {
    if (m_dirty) {
        // 必须先将脏位置为false，因为子类更新会回调到此函数, 防止死循环(暂时没有太好的解决方案)
        //サ ブクラスの更新処理がこの関数を再び呼び出す可能性があるため、最初にダーティーフラグを false に設定しておく必要があります。
        //無限ループを防ぐためです。（今のところ、あまり良い解決策はありません）
        m_dirty = false;
        RecalculateLocalMatrix();
        if (m_parent) {
            m_worldMatrix = m_localMatrix * m_parent->GetWorldMatrix();
        } else {
            m_worldMatrix = m_localMatrix;
        }
        // 子TransformCのワールド変換行列を更新
        for (TransformC * transformChild : m_Childs) {
            transformChild->m_dirty = true;
            transformChild->UpdateTransform();
        }
    }
}

DirectX::SimpleMath::Matrix TransformC::GetWorldMatrix() {
    UpdateTransform();
    return m_worldMatrix;
}

Vector3 TransformC::GetWorldPosition() {
    UpdateTransform();
    return m_worldMatrix.Translation();
}

Quaternion TransformC::GetWorldRotation() {
    UpdateTransform();
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    m_worldMatrix.Decompose(scale, rotation, translation);
    return rotation;
}

Vector3 TransformC::GetWorldRotationEuler() {
    UpdateTransform();
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    m_worldMatrix.Decompose(scale, rotation, translation);

    // クォータニオンを使ってオイラー角に変換する
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

Vector3 TransformC::GetWorldScale() {
    UpdateTransform();
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    m_worldMatrix.Decompose(scale, rotation, translation);
    return scale;
}

void TransformC::SetParent(TransformC *newParent, bool keepWorld) {
    if (keepWorld) {
        // 現在のワールド変換行列を取得
        DirectX::SimpleMath::Matrix oldWorld = GetWorldMatrix();
        // 新しい親ノードを設定
        m_parent = newParent;

        // ローカル行列を再計算：親の逆行列 × ワールド行列
        DirectX::SimpleMath::Matrix parentWorldInv = DirectX::SimpleMath::Matrix::Identity;
        if (m_parent)
            parentWorldInv = m_parent->GetWorldMatrix().Invert();

        m_localMatrix = oldWorld * parentWorldInv;

        // 行列を position / rotation / scale に分解
        Vector3 scale;
        Quaternion rotation;
        Vector3 translation;
        m_localMatrix.Decompose(scale, rotation, translation);
        SetLocalPosition(translation);
        SetLocalRotation(rotation);
        SetLocalScale(scale);
    } else {
        // ワールド座標を保持しない場合、親ノードを直接設定
        m_parent = newParent;
    }
    m_dirty = true;
}

void TransformC::AddChild(TransformC *child, bool keepWorld) {
    child->SetParent(this, keepWorld);
    m_Childs.push_back(child);
}

void TransformC::RemoveChild(TransformC *child) {
    m_Childs.erase(
        std::remove(m_Childs.begin(), m_Childs.end(), child),
        m_Childs.end()
    );
}
