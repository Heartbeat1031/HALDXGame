#pragma once

#include <SimpleMath.h>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>
#include "Game.h"
#include "Global.h"
#include "Scene.h"
#include "SoAStorage.h"

using namespace DirectX::SimpleMath;

class GameObject {
private:
    UID uid = -1;
    std::unordered_map<std::type_index, UID> m_ComponentMap;

protected:
    virtual void Init(){};

    virtual void Uninit(){};

    virtual void Update(float dt){};

public:
    GameObject();

    virtual ~GameObject();

    void SetUID(UID uuid);

    UID GetUID() const;

    void InitBase();

    void UninitBase();

    void UpdateBase(float dt);

    template<typename T>
    T &AddComponent();

    template<typename T>
    T &GetComponent();
};


template<typename T>
T &GameObject::AddComponent() {
    auto it = m_ComponentMap.find(std::type_index(typeid(T)));
    if (it != m_ComponentMap.end()) {
        assert(false && "ゲームオブジェクトに重複コンポーネントが追加されました");
        return GetComponent<T>();
    }
    Scene *scene = halgame->GetScene();
    T &component = scene->AddComponent<T>(this);
    m_ComponentMap[std::type_index(typeid(T))] = component.GetUID();
    return component;
}

template<typename T>
T &GameObject::GetComponent() {
    auto it = m_ComponentMap.find(std::type_index(typeid(T)));
    if (it == m_ComponentMap.end()) {
        throw std::runtime_error("ゲームオブジェクトにコンポーネントが見つかりません");
    }
    return halgame->GetScene()->GetComponent<T>(it->second);
}
