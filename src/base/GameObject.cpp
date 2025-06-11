//
// Created by lclcl on 25-5-29.
//
#include "GameObject.h"
#include "Global.h"
#include "Scene.h"
#include "TransformC.h"

GameObject::GameObject() {
    // デフォルトでTransformComponentを追加
    AddComponent<TransformC>();
};

GameObject::~GameObject() = default;

void GameObject::SetUID(UID uuid) {
    uid = uuid;
}

UID GameObject::GetUID() const {
    return uid;
}

void GameObject::InitBase() {
    // ユーザー定義の初期化
    Init();
}

void GameObject::UninitBase() {
    Scene* scene = halgame->GetScene();
    // すべてのComponentを削除
    for (const auto& [type, uid] : m_ComponentMap) {
        scene->RemoveComponent(uid);
    }
    Uninit();
}

void GameObject::UpdateBase(float dt) {
    // 子クラスのUpdateを呼び出す
    Update(dt);
}