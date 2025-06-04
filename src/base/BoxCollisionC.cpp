//
// Created by clcc777 on 25-6-4.
//
#include <Jolt/Jolt.h>
#include "BoxCollisionC.h"
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
    // 创建形状
    RefConst<JPH::Shape> shape = new JPH::BoxShape(Vec3(m_size.x, m_size.y, m_size.z));
    auto &myTransform = m_gameObject->GetComponent<TransformC>();
    Vector3 myPos = myTransform.GetWorldPosition();
    // 设置刚体创建信息
    JPH::BodyCreationSettings settings(
        shape,
        RVec3(myPos.x + m_offset.x, myPos.y + m_offset.y, myPos.z + m_offset.z), // 初期位置
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

    // 获取当前旋转
    Quat newRotation = bodyInterface.GetRotation(m_bodyID);
    Quaternion dxRot(newRotation.GetX(), newRotation.GetY(), newRotation.GetZ(), newRotation.GetW());

    // 把 offset 旋转到世界空间
    Vector3 worldOffset = Vector3::Transform(m_offset, dxRot);

    // 设置刚体中心的新位置
    Vector3 bodyPos = position + worldOffset;
    bodyInterface.SetPosition(
        m_bodyID,
        RVec3(bodyPos.x, bodyPos.y, bodyPos.z),
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

    // 获取当前旋转
    Quat rot = bodyInterface.GetRotation(m_bodyID);
    Quaternion dxRot(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());

    // 把 offset 变换到世界空间
    Vector3 worldOffset = Vector3::Transform(m_offset, dxRot);

    // 返回修正后的位置
    return Vector3(pos.GetX(), pos.GetY(), pos.GetZ()) - worldOffset;
}

DirectX::SimpleMath::Vector3 BoxCollisionC::GetRotationEuler() const {
    auto &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();
    Quat rot = bodyInterface.GetRotation(m_bodyID);

    // 将 Jolt 的 Quat 转换为 DirectX::SimpleMath::Quaternion
    Quaternion dxRot(rot.GetX(), rot.GetY(), rot.GetZ(), rot.GetW());

    // 从 Quaternion 获取欧拉角
    float pitch = std::atan2(2.0f * (dxRot.y * dxRot.z + dxRot.w * dxRot.x),
                             1.0f - 2.0f * (dxRot.x * dxRot.x + dxRot.y * dxRot.y));
    float yaw = std::asin(2.0f * (dxRot.w * dxRot.y - dxRot.x * dxRot.z));
    float roll = std::atan2(2.0f * (dxRot.w * dxRot.z + dxRot.x * dxRot.y),
                            1.0f - 2.0f * (dxRot.y * dxRot.y + dxRot.z * dxRot.z));

    return DirectX::SimpleMath::Vector3(pitch, yaw, roll);
}


void BoxCollisionC::Update(float dt) {
    Component::Update(dt);
    auto &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();

    // 获取刚体位置与旋转
    RVec3 newPos = bodyInterface.GetCenterOfMassPosition(m_bodyID);
    Quat newRotation = bodyInterface.GetRotation(m_bodyID);

    // 转为DX类型
    Quaternion dxRot(newRotation.GetX(), newRotation.GetY(), newRotation.GetZ(), newRotation.GetW());

    // 旋转offset
    Vector3 worldOffset = Vector3::Transform(m_offset, dxRot);

    // 正确设置Transform
    auto &myTransform = m_gameObject->GetComponent<TransformC>();
    myTransform.SetWorldPosition(Vector3(newPos.GetX(), newPos.GetY(), newPos.GetZ()) - worldOffset);
    myTransform.SetWorldRotation(dxRot);
}
