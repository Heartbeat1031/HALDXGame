//
// Created by clcc777 on 25-6-4.
//

#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/MotionType.h>
#include <Jolt/Physics/Body/BodyID.h>
#include "Component.h"
#include "SimpleMath.h"

class BoxCollisionC: public Component {
private:
    JPH::BodyID  m_bodyID;
    JPH::EMotionType m_MotionType;
    DirectX::SimpleMath::Vector3 m_size;   // サイズ（幅、高さ、奥行き）
    DirectX::SimpleMath::Vector3 m_offset; // 位置オフセット
    DirectX::SimpleMath::Vector3 m_rotation; // 回転（ピッチ、ヨー、ロール）
public:
    explicit BoxCollisionC(DirectX::SimpleMath::Vector3 size = DirectX::SimpleMath::Vector3::Zero, JPH::EMotionType motionType = JPH::EMotionType::Dynamic);
    void SetOffset(DirectX::SimpleMath::Vector3 offset);
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;
    void SetPosition(DirectX::SimpleMath::Vector3 position);
    void SetRotation(DirectX::SimpleMath::Quaternion rotation);
    void SetRotationEuler(DirectX::SimpleMath::Vector3 eulerRadians);
    DirectX::SimpleMath::Vector3 GetPosition() const;
    DirectX::SimpleMath::Vector3 GetRotationEuler() const;
};
