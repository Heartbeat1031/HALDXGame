//
// Created by lclcl on 25-6-24.
//

#include "CollisionC.h"

#include "GameObject.h"
#include "Global.h"
#include "TransformC.h"

using namespace DirectX::SimpleMath;

CollisionC::CollisionC(JPH::EMotionType motionType): m_bodyInterface(halgame->GetPhysicsSystem()->GetBodyInterface()),
                                                     m_MotionType(motionType) {
}

void CollisionC::Init() {
    Component::Init();
    JPH::BodyCreationSettings settings = GetBodyCreationSettings();
    m_bodyID = m_bodyInterface.CreateBody(settings)->GetID();
    m_bodyInterface.AddBody(m_bodyID, JPH::EActivation::Activate);
    halgame->GetContactListener().AddBodyToUIDMap(m_bodyID, GetUID());
}

void CollisionC::Uninit() {
    Component::Uninit();
    m_bodyInterface.RemoveBody(m_bodyID);
    halgame->GetContactListener().RemoveBodyFromUIDMap(m_bodyID);
}

void CollisionC::Update(float dt) {
    Component::Update(dt);

    JPH::RVec3 pos = m_bodyInterface.GetCenterOfMassPosition(m_bodyID);
    JPH::Quat rot = m_bodyInterface.GetRotation(m_bodyID);
    Quaternion dxRot(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());

    // 局部旋转偏移
    Quaternion offsetRot = Quaternion::CreateFromYawPitchRoll(
        m_offsetTransform.rotationOffset.y,
        m_offsetTransform.rotationOffset.x,
        m_offsetTransform.rotationOffset.z
    );

    // 注意这里换乘顺序
    Quaternion finalRot = offsetRot * dxRot;

    // 位移也基于偏移旋转后的方向
    Vector3 worldOffset = Vector3::Transform(m_offsetTransform.positionOffset, finalRot);

    auto &myTransform = m_gameObject->GetComponentRef<TransformC>();
    myTransform.SetWorldPosition(Vector3(pos.GetX(), pos.GetY(), pos.GetZ()) - worldOffset);
    myTransform.SetWorldRotation(finalRot);
}


void CollisionC::SetOffsetPosition(const DirectX::SimpleMath::Vector3 &position) {
    m_offsetTransform.positionOffset = position;
}

void CollisionC::SetOffsetRotation(const DirectX::SimpleMath::Vector3 &rotation) {
    m_offsetTransform.rotationOffset = rotation;
}

void CollisionC::SetGravityFactor(float inGravityFactor) const {
    m_bodyInterface.SetGravityFactor(m_bodyID, inGravityFactor);
}

void CollisionC::SetPosition(Vector3 position) {
    m_bodyInterface.SetPosition(
        m_bodyID, JPH::RVec3(position.x, position.y, position.z), JPH::EActivation::Activate);
}

void CollisionC::SetRotation(Quaternion rotation) {
    m_bodyInterface.SetRotation(
        m_bodyID, JPH::Quat(rotation.x, rotation.y, rotation.z, rotation.w), JPH::EActivation::Activate);
}

void CollisionC::SetRotationEuler(Vector3 eulerRadians) {
    Quaternion q = Quaternion::CreateFromYawPitchRoll(eulerRadians.y, eulerRadians.x, eulerRadians.z);
    SetRotation(q);
}

Vector3 CollisionC::GetPosition() const {
    JPH::RVec3 pos = m_bodyInterface.GetCenterOfMassPosition(m_bodyID);
    return Vector3(pos.GetX(), pos.GetY(), pos.GetZ());
}

Vector3 CollisionC::GetRotationEuler() const {
    JPH::Quat rot = m_bodyInterface.GetRotation(m_bodyID);
    Quaternion q(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());

    float pitch = std::atan2(2.0f * (q.y * q.z + q.w * q.x), 1.0f - 2.0f * (q.x * q.x + q.y * q.y));
    float yaw = std::asin(2.0f * (q.w * q.y - q.x * q.z));
    float roll = std::atan2(2.0f * (q.w * q.z + q.x * q.y), 1.0f - 2.0f * (q.y * q.y + q.z * q.z));

    return Vector3(pitch, yaw, roll);
}

void CollisionC::OnInspectorGUI() {
    Component::OnInspectorGUI();

    Vector3 pos = GetPosition();
    float posArr[3] = {pos.x, pos.y, pos.z};
    if (ImGui::DragFloat3("BoxPos", posArr, 0.1f)) {
        SetPosition(Vector3(posArr[0], posArr[1], posArr[2]));
    }

    Vector3 rot = GetRotationEuler();
    float rotArr[3] = {rot.x, rot.y, rot.z};
    if (ImGui::DragFloat3("BoxRot", rotArr, 0.1f)) {
        SetRotationEuler(Vector3(rotArr[0], rotArr[1], rotArr[2]));
    }

    float offsetArr[3] = {
        m_offsetTransform.positionOffset.x, m_offsetTransform.positionOffset.y, m_offsetTransform.positionOffset.z
    };
    if (ImGui::DragFloat3("OffsetPos", offsetArr, 0.1f)) {
        m_offsetTransform.positionOffset = Vector3(offsetArr[0], offsetArr[1], offsetArr[2]);
    }

    float rotOffsetArr[3] = {
        m_offsetTransform.rotationOffset.x, m_offsetTransform.rotationOffset.y, m_offsetTransform.rotationOffset.z
    };
    if (ImGui::DragFloat3("OffsetRot", rotOffsetArr, 0.1f)) {
        m_offsetTransform.rotationOffset = Vector3(rotOffsetArr[0], rotOffsetArr[1], rotOffsetArr[2]);
    }
}
