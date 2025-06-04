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
        RVec3(myPos.x +  m_offset.x, myPos.y + m_offset.y, myPos.z + m_offset.z), // 初期位置
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
}

void BoxCollisionC::Update(float dt) {
    Component::Update(dt);
    auto &bodyInterface = halgame->GetPhysicsSystem()->GetBodyInterface();
    if (!bodyInterface.IsActive(m_bodyID)){
        return;
    }
    // 更新刚体位置
    auto &myTransform = m_gameObject->GetComponent<TransformC>();
    RVec3 newPos = bodyInterface.GetCenterOfMassPosition(m_bodyID);
    myTransform.SetWorldPosition(Vector3(newPos.GetX(), newPos.GetY(), newPos.GetZ()) - m_offset);
    Quat newRotation = bodyInterface.GetRotation(m_bodyID);
    myTransform.SetWorldRotation(Quaternion(newRotation.GetX(), newRotation.GetY(), newRotation.GetZ(), newRotation.GetW()));
}
