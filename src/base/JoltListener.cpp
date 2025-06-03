#include <iostream>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Math/Vec3.h>
#include "JoltListener.h"

using namespace std;

JPH::ValidateResult JoltContactListener::OnContactValidate(
    const JPH::Body &inBody1,
    const JPH::Body &inBody2,
    JPH::RVec3Arg inBaseOffset,
    const JPH::CollideShapeResult &inCollisionResult)
{
    cout << "Contact validate callback" << endl;
    return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void JoltContactListener::OnContactAdded(
    const JPH::Body &inBody1,
    const JPH::Body &inBody2,
    const JPH::ContactManifold &inManifold,
    JPH::ContactSettings &ioSettings)
{
    cout << "A contact was added" << endl;
}

void JoltContactListener::OnContactPersisted(
    const JPH::Body &inBody1,
    const JPH::Body &inBody2,
    const JPH::ContactManifold &inManifold,
    JPH::ContactSettings &ioSettings)
{
    cout << "A contact was persisted" << endl;
}

void JoltContactListener::OnContactRemoved(const JPH::SubShapeIDPair &inSubShapePair)
{
    cout << "A contact was removed" << endl;
}

void JoltBodyActivationListener::OnBodyActivated(
    const JPH::BodyID &inBodyID,
    JPH::uint64 inBodyUserData)
{
    cout << "A body got activated" << endl;
}

void JoltBodyActivationListener::OnBodyDeactivated(
    const JPH::BodyID &inBodyID,
    JPH::uint64 inBodyUserData)
{
    cout << "A body went to sleep" << endl;
}
