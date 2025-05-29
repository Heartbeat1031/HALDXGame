#pragma once

#include "Component.h"
#include "GameObject.h"

class Scene {
protected:
    SoAStorage<GameObject> m_GameObjectStorage;
    SoAStorage<Component> m_ComponentStorage;

public:
    Scene();

    virtual ~Scene();

    virtual void Init() = 0;

    virtual void Uninit() = 0;

    virtual void Update() = 0;

    void InitBase();

    void UninitBase();

    void UpdateBase(float dt);

    // ゲームオブジェクトを追加
    template<typename T>
    T &AddGameObject();

    // ゲームオブジェクトを取得
    template<typename T>
    T &GetGameObject(UID handle);

    // ゲームオブジェクトを削除
    void RemoveGameObject(UID handle);

    template<typename T>
    T &AddComponent(GameObject *parent);

    template<typename T>
    T &GetComponent(UID handle);

    void RemoveComponent(UID handle);
};

template<typename T>
T &Scene::AddGameObject() {
    static_assert(std::is_base_of<GameObject, T>::value, "T は GameObject を継承する必要があります。");
    std::unique_ptr<T> gameObject = std::make_unique<T>();
    T *rawPtr = gameObject.get();
    UID handle = m_GameObjectStorage.Add(std::unique_ptr<GameObject>(std::move(gameObject)));
    rawPtr->SetUID(handle);
    rawPtr->InitBase();
    return *rawPtr;
}

template<typename T>
T &Scene::GetGameObject(UID handle) {
    static_assert(std::is_base_of<GameObject, T>::value, "T は GameObject を継承する必要があります。");
    GameObject &base = m_GameObjectStorage.Get(handle);
    return dynamic_cast<T&>(base);
}

template<typename T>
T &Scene::AddComponent(GameObject *parent) {
    static_assert(std::is_base_of<Component, T>::value, "T は Component を継承する必要があります。");
    std::unique_ptr<T> component = std::make_unique<T>(parent);
    T *rawPtr = component.get();
    UID uid = m_ComponentStorage.Add(std::unique_ptr<Component>(std::move(component)));
    rawPtr->SetUID(uid);
    rawPtr->Init();
    return *rawPtr;
}

template<typename T>
T &Scene::GetComponent(UID handle) {
    static_assert(std::is_base_of<Component, T>::value, "T は Component を継承する必要があります。");
    Component &base = m_ComponentStorage.Get(handle);
    return dynamic_cast<T&>(base);
}
