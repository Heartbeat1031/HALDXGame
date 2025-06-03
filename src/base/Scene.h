#pragma once

#include "Component.h"
#include "GameObject.h"

//---------------------------------------------------------------//
// Scene是游戏当前所有GameObject和Component的容器, 负责管理GameObject和Component的生命周期和更新。
// Sceneは現在のゲームのすべてのGameObjectとComponentのコンテナであり、GameObjectとComponentのライフサイクルと更新を管理します。
//----------------------------------------------------------------//

class Scene {
protected:
    // GameObjectのコンテナ
    SoAStorage<GameObject> m_GameObjectStorage;
    // Componentのコンテナ
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
    template<class T, class ... Args>
    T &AddGameObject(Args &&... args);

    // ゲームオブジェクトを取得
    template<typename T>
    T &GetGameObject(UID handle);


    // ゲームオブジェクトを削除
    void RemoveGameObject(UID handle);

    // コンポーネントを追加
    template<class T, class ... Args>
    T &AddComponent(GameObject *parent, Args &&... args);

    // コンポーネントを取得
    template<typename T>
    T &GetComponent(UID handle);

    // コンポーネントを削除
    void RemoveComponent(UID handle);
};

template<typename T, typename... Args>
T &Scene::AddGameObject(Args&&... args) {
    static_assert(std::is_base_of<GameObject, T>::value, "T は GameObject を継承する必要があります。");
    std::unique_ptr<T> gameObject = std::make_unique<T>(std::forward<Args>(args)...);
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
    // 这里使用的强制转换, 虽然有点费效率, 但是优化费劲,调用频率不高, 所以暂时放弃优化. 懒!!
    // ここでは強制キャストを使用しています。効率は少し低下しますが、最適化は面倒で、呼び出し頻度も高くないため、今のところ最適化は行いません。
    return dynamic_cast<T&>(base);
}

template<typename T, typename... Args>
T &Scene::AddComponent(GameObject *parent, Args&&... args) {
    static_assert(std::is_base_of<Component, T>::value, "T は Component を継承する必要があります。");
    std::unique_ptr<T> component = std::make_unique<T>(std::forward<Args>(args)...);
    T *rawPtr = component.get();
    UID uid = m_ComponentStorage.Add(std::unique_ptr<Component>(std::move(component)));
    rawPtr->SetGameObject(parent);
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
