//
// Created by clcc777 on 25-6-7.
//

#pragma once

#include <Jolt/Jolt.h>
#include "Component.h"
#include "Jolt/Physics/Ragdoll/Ragdoll.h"

class MixamoRagdollC: public Component {
private:
    JPH::RagdollSettings *mRagdollSettings;
public:
    MixamoRagdollC();
    ~MixamoRagdollC() override;
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;

    JPH::Ref<JPH::Ragdoll> mRagdoll ; // ragdoll本体
    JPH::SkeletonPose			mRagdollPose;

    void UpdateFinalBoneMatrices();
};
