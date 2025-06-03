// JoltListener.h
#pragma once

#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Math/Real.h>

#include "Jolt/Core/Core.h"

class JoltContactListener : public ContactListener
{
public:
    virtual ValidateResult OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult) override;

    virtual void OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override;

    virtual void OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override;

    virtual void OnContactRemoved(const SubShapeIDPair &inSubShapePair) override;
};

class JoltBodyActivationListener : public BodyActivationListener
{
public:
    virtual void OnBodyActivated(const BodyID &inBodyID, JPH::uint64 inBodyUserData) override;

    virtual void OnBodyDeactivated(const BodyID &inBodyID, JPH::uint64 inBodyUserData) override;
};
