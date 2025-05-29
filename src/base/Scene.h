#pragma once

#include "GameObject.h"

class Scene {
protected:
    SoAStorage<GameObject> m_GameObjectStorage;

public:
    Scene() {
    }

    virtual ~Scene() {
    }
    virtual void Init() =0;
    virtual void Uninit() =0;
    virtual void Update() =0;

    void InitBase() {
        Init();
    }

    void UninitBase() {
        m_GameObjectStorage.ForEachActive([this](SoAHandle id, GameObject &game_object) {
            game_object.UninitBase();
        });
        m_GameObjectStorage.Reset();
        Uninit();
    }

    void UpdateBase(float dt) {
        m_GameObjectStorage.ForEachActive([this, dt](SoAHandle id, GameObject &game_object) {
            game_object.UpdateBase(dt);
        });
        Update();
    }

    // ゲームオブジェクトを追加
    template<typename T>
    T *AddGameObject() {
        static_assert(std::is_base_of<GameObject, T>::value, "T は GameObject を継承する必要があります。");
        std::unique_ptr<T> gameObject = std::make_unique<T>();
        T *rawPtr = gameObject.get();
        SoAHandle handle = m_GameObjectStorage.Add(std::unique_ptr<GameObject>(std::move(gameObject)));
        rawPtr->SetHandle(handle);
        rawPtr->Init();
        return rawPtr;
    }

    // ゲームオブジェクトを取得
    template<typename T>
    T *GetGameObject(SoAHandle handle) {
        static_assert(std::is_base_of<GameObject, T>::value, "T は GameObject を継承する必要があります。");
        return m_GameObjectStorage.Get<T>(handle);
    }

    // ゲームオブジェクトを削除
    template<typename T>
    void RemoveGameObject(SoAHandle handle) {
        static_assert(std::is_base_of<GameObject, T>::value, "T は GameObject を継承する必要があります。");
        GetGameObject<T>(handle)->UninitBase();
        m_GameObjectStorage.Remove(handle);
    }
};
