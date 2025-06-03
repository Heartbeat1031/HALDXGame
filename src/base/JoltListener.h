#pragma once
#include <Jolt/Jolt.h>                    // 必须：包含 Defines.h 和一切 Jolt 基础
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Math/Vec3.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Math/Real.h>
#include <Jolt/Core/Core.h> // 确保定义了 uint64 等类型

class JoltContactListener : public JPH::ContactListener {
public:
    // Called to validate a contact point before it is added
    virtual JPH::ValidateResult OnContactValidate(
        const JPH::Body &inBody1,
        const JPH::Body &inBody2,
        JPH::RVec3Arg inBaseOffset,
        const JPH::CollideShapeResult &inCollisionResult
    ) override;

    // Called when a new contact point has been added
    virtual void OnContactAdded(
        const JPH::Body &inBody1,
        const JPH::Body &inBody2,
        const JPH::ContactManifold &inManifold,
        JPH::ContactSettings &ioSettings
    ) override;

    // Called when an existing contact persists
    virtual void OnContactPersisted(
        const JPH::Body &inBody1,
        const JPH::Body &inBody2,
        const JPH::ContactManifold &inManifold,
        JPH::ContactSettings &ioSettings
    ) override;

    // Called when a contact point is removed
    virtual void OnContactRemoved(
        const JPH::SubShapeIDPair &inSubShapePair
    ) override;
};

class JoltBodyActivationListener : public JPH::BodyActivationListener {
public:
    // Called when a body becomes active (e.g. due to a collision or force)
    virtual void OnBodyActivated(
        const JPH::BodyID &inBodyID,
        JPH::uint64 inBodyUserData
    ) override;

    // Called when a body goes to sleep
    virtual void OnBodyDeactivated(
        const JPH::BodyID &inBodyID,
        JPH::uint64 inBodyUserData
    ) override;
};
