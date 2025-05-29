#pragma once

#include <SimpleMath.h>
#include "Component.h"
#include "SoAStorage.h"

using namespace DirectX::SimpleMath;

class GameObject {
private:
    UID uid = -1;

protected:
    virtual void Init() =0;
    virtual void Uninit() = 0;
    virtual void Update(float dt) =0;
    GameObject *m_Parent = nullptr;
    SoAStorage<Component> m_ComponentStorage;

public:
    GameObject() {
    }
    virtual ~GameObject() {
    }

    void SetUID(UID uuid) { uid = uuid; }
    UID GetUID() const { return uid; }

    template<typename T>
    T *AddComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T は Component を継承する必要があります。");
        std::unique_ptr<T> component = std::make_unique<T>(this);
        T *rawPtr = component.get();
        UID handle = m_ComponentStorage.Add(std::unique_ptr<Component>(std::move(component)));
        rawPtr->SetUID(handle);
        rawPtr->Init();
        return rawPtr;
    }

    void InitBase() {
        Init();
    }

    void UninitBase() {
        Uninit();
    }

    void UpdateBase(float dt) {
        Update(dt);
    }
};
