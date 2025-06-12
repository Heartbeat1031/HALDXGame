//
// Created by clcc777 on 25-6-4.
//
#include <Jolt/Jolt.h>
#include "BoxCollisionC.h"

#include "GameObject.h"
#include "Global.h"
#include "TransformC.h"
#include "Jolt/Physics/Body/BodyCreationSettings.h"
#include "Jolt/Physics/Collision/Shape/BoxShape.h"

BoxCollisionC::BoxCollisionC(DirectX::SimpleMath::Vector3 size, JPH::EMotionType motionType) {
    m_MotionType = motionType;
    m_size = size;
    m_offset = DirectX::SimpleMath::Vector3::Zero;
    m_rotation = DirectX::SimpleMath::Vector3::Zero; // 初期回転はゼロ
}

void BoxCollisionC::SetOffset(DirectX::SimpleMath::Vector3 offset) {
    m_offset = offset;
}

void BoxCollisionC::Init() {
    Component::Init();

    JPH::BodyInterface &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();
    // 形状を作成
    RefConst<JPH::Shape> shape = new JPH::BoxShape(Vec3(m_size.x, m_size.y, m_size.z));
    auto &myTransform = m_gameObject->GetComponentRef<TransformC>();
    Vector3 myPos = myTransform.GetWorldPosition();

    // 初期位置を設定
    JPH::BodyCreationSettings settings(
        shape,
        RVec3(myPos.x + m_offset.x, myPos.y + m_offset.y, myPos.z + m_offset.z),
        Quat::sIdentity(),
        m_MotionType,
        Layers::MOVING
    );

    JPH::Body *body = bodyInterface.CreateBody(settings);
    m_bodyID = body->GetID(); // 保存刚体ID
    bodyInterface.AddBody(m_bodyID, JPH::EActivation::Activate);
}

void BoxCollisionC::Uninit() {
    Component::Uninit();
    JPH::BodyInterface &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();
    bodyInterface.RemoveBody(m_bodyID);
}

void BoxCollisionC::SetPosition(DirectX::SimpleMath::Vector3 position) {
    auto &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();
    bodyInterface.SetPosition(
        m_bodyID,
        RVec3(position.x, position.y, position.z),
        JPH::EActivation::Activate
    );
}

void BoxCollisionC::SetRotation(DirectX::SimpleMath::Quaternion rotation) {
    auto &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();
    // 将 DirectX::SimpleMath::Quaternion 转换为 Jolt 的 Quat
    Quat joltQuat(rotation.x, rotation.y, rotation.z, rotation.w);
    bodyInterface.SetRotation(m_bodyID, joltQuat, JPH::EActivation::Activate);
}

void BoxCollisionC::SetRotationEuler(DirectX::SimpleMath::Vector3 eulerRadians) {
    auto &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();
    Quaternion newRotation = Quaternion::CreateFromYawPitchRoll(eulerRadians.y, eulerRadians.x, eulerRadians.z);
    Quat joltQuat(newRotation.x, newRotation.y, newRotation.z, newRotation.w);
    bodyInterface.SetRotation(m_bodyID, joltQuat, JPH::EActivation::Activate);
}

DirectX::SimpleMath::Vector3 BoxCollisionC::GetPosition() const {
    auto &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();
    RVec3 pos = bodyInterface.GetCenterOfMassPosition(m_bodyID);
    return Vector3(pos.GetX(), pos.GetY(), pos.GetZ());
}

DirectX::SimpleMath::Vector3 BoxCollisionC::GetRotationEuler() const {
    auto &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();
    Quat rot = bodyInterface.GetRotation(m_bodyID);

    // Jolt の Quat を DirectX::SimpleMath::Quaternion に変換
    Quaternion dxRot(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());

    // Quaternion からオイラー角を取得
    float pitch = std::atan2(2.0f * (dxRot.y * dxRot.z + dxRot.w * dxRot.x),
                             1.0f - 2.0f * (dxRot.x * dxRot.x + dxRot.y * dxRot.y));
    float yaw = std::asin(2.0f * (dxRot.w * dxRot.y - dxRot.x * dxRot.z));
    float roll = std::atan2(2.0f * (dxRot.w * dxRot.z + dxRot.x * dxRot.y),
                            1.0f - 2.0f * (dxRot.y * dxRot.y + dxRot.z * dxRot.z));

    return DirectX::SimpleMath::Vector3(pitch, yaw, roll);
}

void BoxCollisionC::OnInspectorGUI() {
    Component::OnInspectorGUI();
    DirectX::SimpleMath::Vector3 postion = GetPosition();
    float posArr[3] = {postion.x, postion.y, postion.z};
    if (ImGui::DragFloat3("BoxPos", posArr, 0.1f)) {
        SetPosition(DirectX::SimpleMath::Vector3(posArr[0], posArr[1], posArr[2]));
    }

    DirectX::SimpleMath::Vector3 rotation = GetRotationEuler();
    float rotArr[3] = {rotation.x, rotation.y, rotation.z};
    if (ImGui::DragFloat3("BoxRot", rotArr, 0.1f)) {
        SetRotationEuler(DirectX::SimpleMath::Vector3(rotArr[0], rotArr[1], rotArr[2]));
    }

    float offsetArr[3] = {m_offset.x, m_offset.y, m_offset.z};
    if (ImGui::DragFloat3("Offset", offsetArr, 0.1f)) {
        SetOffset(DirectX::SimpleMath::Vector3(offsetArr[0], offsetArr[1], offsetArr[2]));
    }
}

void BoxCollisionC::Update(float dt) {
    Component::Update(dt);
    auto &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();

    // 物理システムから位置と回転を取得
    RVec3 newPos = bodyInterface.GetCenterOfMassPosition(m_bodyID);
    Quat newRotation = bodyInterface.GetRotation(m_bodyID);

    // DirectX::SimpleMath::Quaternion に変換
    Quaternion dxRot(newRotation.GetX(), newRotation.GetY(), newRotation.GetZ(), newRotation.GetW());

    // オフセットを回転
    Vector3 worldOffset = Vector3::Transform(m_offset, dxRot);

    // TransformC コンポーネントを正しく設定
    auto &myTransform = m_gameObject->GetComponentRef<TransformC>();
    myTransform.SetWorldPosition(Vector3(newPos.GetX(), newPos.GetY(), newPos.GetZ()) - worldOffset);
    myTransform.SetWorldRotation(dxRot);
}
