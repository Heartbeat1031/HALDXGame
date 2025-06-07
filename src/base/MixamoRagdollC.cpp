//
// Created by clcc777 on 25-6-7.
//

#include "MixamoRagdollC.h"
#include <Jolt/Jolt.h>

#include "Global.h"
#include "Layers.h"
#include "Jolt/Core/Reference.h"
#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"
#include "Jolt/Physics/Collision/Shape/Shape.h"
#include "Jolt/Physics/Constraints/SwingTwistConstraint.h"
#include "Jolt/Physics/Ragdoll/Ragdoll.h"
#include "Jolt/Skeleton/Skeleton.h"

MixamoRagdollC::MixamoRagdollC() {
}

MixamoRagdollC::~MixamoRagdollC() {
    mRagdoll->RemoveFromPhysicsSystem();
}

void MixamoRagdollC::Init() {
    Component::Init();
    JPH::Ref<JPH::Skeleton> skeleton = new JPH::Skeleton();
    JPH::uint hips = skeleton->AddJoint("mixamorig:Hips"); // 根节点
    JPH::uint lower_body = skeleton->AddJoint("mixamorig:Spine", hips);
    JPH::uint mid_body = skeleton->AddJoint("mixamorig:Spine1", lower_body);
    JPH::uint upper_body = skeleton->AddJoint("mixamorig:Spine2", mid_body);
    JPH::uint head = skeleton->AddJoint("mixamorig:Head", upper_body);

    JPH::uint upper_arm_l = skeleton->AddJoint("mixamorig:LeftArm", upper_body);
    JPH::uint lower_arm_l = skeleton->AddJoint("mixamorig:LeftForeArm", upper_arm_l);

   JPH:: uint upper_arm_r = skeleton->AddJoint("mixamorig:RightArm", upper_body);
    JPH::uint lower_arm_r = skeleton->AddJoint("mixamorig:RightForeArm", upper_arm_r);

    JPH::uint upper_leg_l = skeleton->AddJoint("mixamorig:LeftUpLeg", hips);
    JPH::uint lower_leg_l = skeleton->AddJoint("mixamorig:LeftLeg", upper_leg_l);

    JPH::uint upper_leg_r = skeleton->AddJoint("mixamorig:RightUpLeg", hips);
    JPH::uint lower_leg_r = skeleton->AddJoint("mixamorig:RightLeg", upper_leg_r);

    // ---------------------- RagdollSettings 配置 ----------------------

    RagdollSettings *mRagdollSettings = new JPH::RagdollSettings;
    mRagdollSettings->mSkeleton = skeleton;
    mRagdollSettings->mParts.resize(skeleton->GetJointCount());

    // 物理形状（Capsule）参数，可以根据实际模型微调
    JPH::Ref<JPH::Shape> shapes[] = {
        new JPH::CapsuleShape(0.20f, 0.11f), // hips
        new JPH::CapsuleShape(0.15f, 0.09f), // lower_body (Spine)
        new JPH::CapsuleShape(0.13f, 0.08f), // mid_body (Spine1)
        new JPH::CapsuleShape(0.11f, 0.07f), // upper_body (Spine2)
        new JPH::CapsuleShape(0.12f, 0.07f), // head

        new JPH::CapsuleShape(0.14f, 0.045f), // upper_arm_l
        new JPH::CapsuleShape(0.12f, 0.04f), // lower_arm_l

        new JPH::CapsuleShape(0.14f, 0.045f), // upper_arm_r
        new JPH::CapsuleShape(0.12f, 0.04f), // lower_arm_r

        new JPH::CapsuleShape(0.18f, 0.055f), // upper_leg_l
        new JPH::CapsuleShape(0.18f, 0.055f), // upper_leg_r

        new JPH::CapsuleShape(0.16f, 0.045f), // lower_leg_l
        new JPH::CapsuleShape(0.16f, 0.045f), // lower_leg_r
    };

    // 参考的初始世界位置（可以用蒙皮骨骼的位置，也可近似用默认T姿）
    JPH::RVec3 positions[] = {
        JPH::RVec3(0, 1.0f, 0), // hips
        JPH::RVec3(0, 1.2f, 0), // lower_body (Spine)
        JPH::RVec3(0, 1.4f, 0), // mid_body (Spine1)
        JPH::RVec3(0, 1.55f, 0), // upper_body (Spine2)
        JPH::RVec3(0, 1.8f, 0), // head

        JPH::RVec3(-0.32f, 1.55f, 0), // upper_arm_l
        JPH::RVec3(-0.60f, 1.55f, 0), // lower_arm_l

        JPH::RVec3(0.32f, 1.55f, 0), // upper_arm_r
        JPH::RVec3(0.60f, 1.55f, 0), // lower_arm_r

        JPH::RVec3(-0.13f, 0.85f, 0), // upper_leg_l
        JPH::RVec3(0.13f, 0.85f, 0), // upper_leg_r

        JPH::RVec3(-0.13f, 0.5f, 0), // lower_leg_l
        JPH::RVec3(0.13f, 0.5f, 0), // lower_leg_r
    };

    // 旋转/朝向，胶囊对齐Y轴即可，T-pose时手臂可转90°
    JPH::Quat rotations[] = {
        JPH::Quat::sIdentity(), // hips
        JPH::Quat::sIdentity(), // lower_body
        JPH::Quat::sIdentity(), // mid_body
        JPH::Quat::sIdentity(), // upper_body
        JPH::Quat::sIdentity(), // head

        JPH::Quat::sRotation(JPH::Vec3::sAxisZ(), 0.5f * JPH::JPH_PI), // upper_arm_l
        JPH::Quat::sRotation(JPH::Vec3::sAxisZ(), 0.5f * JPH::JPH_PI), // lower_arm_l

        JPH::Quat::sRotation(JPH::Vec3::sAxisZ(), 0.5f * JPH::JPH_PI), // upper_arm_r
        JPH::Quat::sRotation(JPH::Vec3::sAxisZ(), 0.5f * JPH::JPH_PI), // lower_arm_r

        JPH::Quat::sIdentity(), // upper_leg_l
        JPH::Quat::sIdentity(), // upper_leg_r

        JPH::Quat::sIdentity(), // lower_leg_l
        JPH::Quat::sIdentity(), // lower_leg_r
    };

    // constraint 相关参数（可根据需要调整，示例用一般值）
    JPH::RVec3 constraint_positions[] = {
        JPH::RVec3::sZero(), // hips/root
        JPH::RVec3(0, 1.1f, 0), // lower_body
        JPH::RVec3(0, 1.3f, 0), // mid_body
        JPH::RVec3(0, 1.5f, 0), // upper_body
        JPH::RVec3(0, 1.7f, 0), // head

        JPH::RVec3(-0.19f, 1.55f, 0), // upper_arm_l
        JPH::RVec3(-0.45f, 1.55f, 0), // lower_arm_l

        JPH::RVec3(0.19f, 1.55f, 0), // upper_arm_r
        JPH::RVec3(0.45f, 1.55f, 0), // lower_arm_r

        JPH::RVec3(-0.13f, 0.75f, 0), // upper_leg_l
        JPH::RVec3(0.13f, 0.75f, 0), // upper_leg_r

        JPH::RVec3(-0.13f, 0.38f, 0), // lower_leg_l
        JPH::RVec3(0.13f, 0.38f, 0), // lower_leg_r
    };

    // twist轴
    JPH::Vec3 twist_axis[] = {
        JPH::Vec3::sZero(), // hips
        JPH::Vec3::sAxisY(), // lower_body
        JPH::Vec3::sAxisY(), // mid_body
        JPH::Vec3::sAxisY(), // upper_body
        JPH::Vec3::sAxisY(), // head

        -JPH::Vec3::sAxisX(), // upper_arm_l
        -JPH::Vec3::sAxisX(), // lower_arm_l

        JPH::Vec3::sAxisX(), // upper_arm_r
        JPH::Vec3::sAxisX(), // lower_arm_r

        -JPH::Vec3::sAxisY(), // upper_leg_l
        -JPH::Vec3::sAxisY(), // upper_leg_r

        -JPH::Vec3::sAxisY(), // lower_leg_l
        -JPH::Vec3::sAxisY(), // lower_leg_r
    };

    float twist_angle[] = {
        0.0f, 5.0f, 5.0f, 5.0f, 90.0f,
        45.0f, 45.0f, 45.0f, 45.0f, 45.0f, 45.0f, 45.0f, 45.0f
    };
    float normal_angle[] = {
        0.0f, 10.0f, 10.0f, 10.0f, 45.0f,
        90.0f, 0.0f, 90.0f, 0.0f, 45.0f, 45.0f, 0.0f, 0.0f
    };
    float plane_angle[] = {
        0.0f, 10.0f, 10.0f, 10.0f, 45.0f,
        45.0f, 90.0f, 45.0f, 90.0f, 45.0f, 45.0f, 60.0f, 60.0f
    };

    // 配置各部分
    for (int p = 0; p < skeleton->GetJointCount(); ++p) {
        JPH::RagdollSettings::Part &part = mRagdollSettings->mParts[p];
        part.SetShape(shapes[p]);
        part.mPosition = positions[p];
        part.mRotation = rotations[p];
        part.mMotionType = JPH::EMotionType::Dynamic;
        part.mObjectLayer = Layers::MOVING;

        if (p > 0) {
            auto *constraint = new JPH::SwingTwistConstraintSettings;
            constraint->mDrawConstraintSize = 0.1f;
            constraint->mPosition1 = constraint->mPosition2 = constraint_positions[p];
            constraint->mTwistAxis1 = constraint->mTwistAxis2 = twist_axis[p];
            constraint->mPlaneAxis1 = constraint->mPlaneAxis2 = JPH::Vec3::sAxisZ();
            constraint->mTwistMinAngle = -DegreesToRadians(twist_angle[p]);
            constraint->mTwistMaxAngle = DegreesToRadians(twist_angle[p]);
            constraint->mNormalHalfConeAngle = DegreesToRadians(normal_angle[p]);
            constraint->mPlaneHalfConeAngle = DegreesToRadians(plane_angle[p]);
            part.mToParent = constraint;
        }
    }

    // 推荐加的初始化
    mRagdollSettings->Stabilize();
    mRagdollSettings->DisableParentChildCollisions();
    mRagdollSettings->CalculateBodyIndexToConstraintIndex();

    mRagdoll = mRagdollSettings->CreateRagdoll(0, 0, halgame->GetPhysicsSystem());
    mRagdoll->AddToPhysicsSystem(EActivation::Activate);
}

void MixamoRagdollC::Update(float dt) {
    Component::Update(dt);
}

void MixamoRagdollC::Uninit() {
    Component::Uninit();
}
