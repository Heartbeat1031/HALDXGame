//
// Created by lclcl on 25-5-29.
//
#include "GameObject.h"
#include "Global.h"
#include "Scene.h"
#include "TransformC.h"

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
    // デフォルトでTransformComponentを追加
    AddComponent<TransformC>();
    // ユーザー定義の初期化
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