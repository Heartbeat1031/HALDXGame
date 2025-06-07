//
// Created by clcc777 on 25-6-7.
//

#pragma once
#include "Component.h"
#include <Jolt/Jolt.h>
#include "Jolt/Core/Reference.h"
#include "Jolt/Physics/Ragdoll/Ragdoll.h"

class MixamoRagdollC: public Component {
private:
    JPH::Ref<JPH::Ragdoll>					mRagdoll;
public:
    MixamoRagdollC();
    ~MixamoRagdollC() override;
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;
};
