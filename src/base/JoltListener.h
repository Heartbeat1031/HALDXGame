#pragma once

#include <iostream>
#include "Jolt/Core/Core.h"
#include "Jolt/Math/Real.h"
#include "Jolt/Math/Vec3.h"
#include "Jolt/Physics/Body/BodyActivationListener.h"
#include "Jolt/Physics/Collision/ContactListener.h"

using namespace JPH;
using namespace std;

class JoltContactListener : public ContactListener {
public:
    ValidateResult OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult) override {
        cout << "Contact validate callback" << endl;
        return ValidateResult::AcceptAllContactsForThisBodyPair;
    };

    void OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override {
        cout << "A contact was added" << endl;
    };

    void OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override {
        cout << "A contact was persisted" << endl;
    };

    void OnContactRemoved(const SubShapeIDPair &inSubShapePair) override {
        cout << "A contact was removed" << endl;
    };
};


class JoltBodyActivationListener : public BodyActivationListener {
public:
    void OnBodyActivated(const BodyID &inBodyID, JPH::uint64 inBodyUserData) override {
        cout << "A body got activated" << endl;
    };

    void OnBodyDeactivated(const BodyID &inBodyID, JPH::uint64 inBodyUserData) override {
        cout << "A body went to sleep" << endl;
    };
};
