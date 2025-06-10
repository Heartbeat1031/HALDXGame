//
// Created by clcc777 on 25-6-4.
//

#include "Component.h"


Component::Component() = default;

Component::~Component() = default;

void Component::SetGameObject(GameObject *gameObject) {
    m_gameObject = gameObject;
}

GameObject *Component::GetGameObject() const {
    return m_gameObject;
}

void Component::SetUID(UID uuid) {
    uid = uuid;
}

UID Component::GetUID() const {
    return uid;
}

void Component::Init() {
}

void Component::Uninit() {
}

void Component::Update(float dt) {
}
