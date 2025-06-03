#include "JoltListener.h"
#include <iostream>

using namespace std;

// ValidateResult JoltContactListener::OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult)
// {
//     cout << "Contact validate callback" << endl;
//     return ValidateResult::AcceptAllContactsForThisBodyPair;
// }
//
// void JoltContactListener::OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings)
// {
//     cout << "A contact was added" << endl;
// }
//
// void JoltContactListener::OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings)
// {
//     cout << "A contact was persisted" << endl;
// }
//
// void JoltContactListener::OnContactRemoved(const SubShapeIDPair &inSubShapePair)
// {
//     cout << "A contact was removed" << endl;
// }
//
// void JoltBodyActivationListener::OnBodyActivated(const BodyID &inBodyID, JPH::uint64 inBodyUserData)
// {
//     cout << "A body got activated" << endl;
// }
//
// void JoltBodyActivationListener::OnBodyDeactivated(const BodyID &inBodyID, JPH::uint64 inBodyUserData)
// {
//     cout << "A body went to sleep" << endl;
// }
