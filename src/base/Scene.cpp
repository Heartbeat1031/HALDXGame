//
// Created by lclcl on 25-5-29.
//
#include "Scene.h"

#include "Component.h"
#include "GameObject.h"
#include "TransformC.h"

Scene::Scene() = default;

Scene::~Scene() = default;

void Scene::InitBase() {
    // rootを初期化
    root = new GameObject();
    UID handle = m_GameObjectStorage.Add(std::unique_ptr<GameObject>(std::move(root)));
    root->SetUID(handle);
    root->SetName("root");
    root->InitBase();
    rootTransform = root->GetComponent<TransformC>();
    rootTransform->SetLocalPosition(Vector3::Zero);
    rootTransform->SetLocalRotation(Quaternion::Identity);
    rootTransform->SetLocalScale(Vector3(1, 1, 1));
    Init();
}

void Scene::UninitBase() {
    // 释放所有的GameObject, m_Component不用在这里释放, 因为GameObject会释放它们
    // すべでのGameObjectを解放します。m_Componentはここで解放する必要はありません。なぜなら、GameObjectがそれらを解放するからです。
    m_GameObjectStorage.ForEachActive([this](UID id, GameObject &game_object) {
        game_object.UninitBase();
    });
    m_GameObjectStorage.Reset();
    Uninit();
}

void Scene::UpdateBase(float dt) {
    // 先更新所有的Component, 再更新所有GameObject
    // すべてのComponentを先に更新し、その後にすべてのGameObjectを更新します。
    m_ComponentStorage.ForEachActive([this, dt](UID id, Component &component) {
        component.Update(dt);
    });
    m_GameObjectStorage.ForEachActive([this, dt](UID id, GameObject &game_object) {
        game_object.UpdateBase(dt);
    });
    Update();
    m_SceneHierarchy.Draw();
}

void Scene::RemoveGameObject(UID handle) {
    m_GameObjectStorage.Get(handle).UninitBase();
    m_GameObjectStorage.Remove(handle);
}

void Scene::RemoveComponent(UID handle) {
    m_ComponentStorage.Get(handle).Uninit();
    m_ComponentStorage.Remove(handle);
}
