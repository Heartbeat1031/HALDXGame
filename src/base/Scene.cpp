//
// Created by lclcl on 25-5-29.
//
#include "Scene.h"

Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::InitBase() {
    Init();
}

void Scene::UninitBase() {
    m_GameObjectStorage.ForEachActive([this](UID id, GameObject &game_object) {
        game_object.UninitBase();
    });
    m_GameObjectStorage.Reset();
    Uninit();
}

void Scene::UpdateBase(float dt) {
    m_ComponentStorage.ForEachActive([this, dt](UID id, Component &component) {
        component.Update(dt);
    });
    m_GameObjectStorage.ForEachActive([this, dt](UID id, GameObject &game_object) {
        game_object.UpdateBase(dt);
    });
    Update();
}

void Scene::RemoveGameObject(UID handle) {
    m_GameObjectStorage.Get(handle).UninitBase();
    m_GameObjectStorage.Remove(handle);
}

void Scene::RemoveComponent(UID handle) {
    m_ComponentStorage.Get(handle).Uninit();
    m_ComponentStorage.Remove(handle);
}
