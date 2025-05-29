//
// Created by lclcl on 25-5-29.
//
#include "GameObject.h"
#include "Global.h"
#include "Scene.h"

GameObject::GameObject() {
}

GameObject::~GameObject() {
}

void GameObject::SetUID(UID uuid) {
    uid = uuid;
}

UID GameObject::GetUID() const {
    return uid;
}

void GameObject::InitBase() {
    Init();
}

void GameObject::UninitBase() {
    Scene* scene = halgame->GetScene();
    for (const auto& [type, uid] : m_ComponentMap) {
        scene->RemoveComponent(uid);
    }
    Uninit();
}

void GameObject::UpdateBase(float dt) {
    Update(dt);
}