#pragma once

#include <list>
#include <SimpleMath.h>
#include "Component.h"

using namespace DirectX::SimpleMath;

class GameObject {
protected:
    bool m_Destroy = false;
    GameObject* m_Parent = nullptr;
    Vector3 m_Position = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 m_Scale = Vector3(1.0f, 1.0f, 1.0f);
    std::list<Component*> m_Component;

    std::list<GameObject*> m_ChildGameObject;

public:
    GameObject(){}
    virtual ~GameObject() {}

    Vector3 GetPosition() const { return m_Position; }
    Vector3 GetRotation() const { return m_Rotation; }
    Vector3 GetScale() const { return m_Scale; }

    virtual void SetPosition(Vector3 Position) { m_Position = Position; }
    virtual void SetRotation(Vector3 Rotation) { m_Rotation = Rotation; }
    virtual void SetScale(Vector3 Scale) { m_Scale = Scale; }
    virtual void ChangePosition(Vector3 Position){SetPosition(m_Position + Position);}

    virtual void LookAt(Vector3 target)
    {
        Vector3 position = GetWorldPosition();
        Vector3 direction = position - target;
        direction.Normalize();

        float yaw = atan2f(direction.x, direction.z);
        float pitch = asinf(direction.y);

        m_Rotation = Vector3(pitch, yaw, 0.0f);
    }

    // 获取世界坐标
    Vector3 GetWorldPosition() const {
        Vector3 worldPos = m_Position;
        for (GameObject* parent = m_Parent; parent != nullptr; parent = parent->m_Parent)
        {
            worldPos = Vector3::Transform(worldPos, Matrix::CreateFromYawPitchRoll(parent->m_Rotation.y, parent->m_Rotation.x, parent->m_Rotation.z));
            worldPos += parent->m_Position;
        }
        return worldPos;
    }

    // 获取前方
    Vector3 GetForward() const {
        Matrix rot = Matrix::CreateFromYawPitchRoll(m_Rotation.y, m_Rotation.x, m_Rotation.z);

        Vector3 forward;
        forward.x = rot._31;
        forward.y = rot._32;
        forward.z = rot._33;

        return forward;
    }

    Vector3 GetWorldForward()
    {
        Vector3 forward = GetForward();
        for (GameObject* parent = this; parent != nullptr; parent = parent->m_Parent)
        {
            forward = Vector3::Transform(forward, Matrix::CreateFromYawPitchRoll(parent->GetRotation().y, parent->GetRotation().x, parent->GetRotation().z));
        }
        return forward;
    }

    void SetDestroy() { m_Destroy = true; }

    bool Destroy()
    {
        if (m_Destroy)
        {
            UninitBase();
            delete this;
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual void Init() {}
    virtual void Uninit() {}
    virtual void Update() {}
    virtual void Draw() {}
    virtual void PreDraw() {}

    template <typename T>
    T* AddComponent()
    {
        T* component = new T(this);
        m_Component.push_back(component);
        ((Component*)component)->Init();
        return component;
    }

    template <typename T>
    T* GetComponent()
    {
        for (Component* component : m_Component)
        {
            if (typeid(*component) == typeid(T))
            {
                return (T*)component;
            }
        }
        return nullptr;
    }

    template <typename T>
    T* AddChild()
    {
        T* child = new T();
        m_ChildGameObject.push_back(child);
        child->m_Parent = this;
        child->InitBase();
        return child;
    }

    void InitBase()
    {
        Init();
    }

    void UninitBase()
    {
        Uninit();

        for (auto* childgo : m_ChildGameObject)
        {
            childgo->Uninit();

            for (Component* component : childgo->m_Component)
            {
                component->Uninit();
                delete component;
            }
            childgo->m_Component.clear();

            delete childgo;
        }
        m_ChildGameObject.clear();

        for (Component* component : m_Component)
        {
            component->Uninit();
            delete component;
        }
        m_Component.clear();
    }

    void UpdateBase()
    {
        for (Component* component : m_Component)
        {
            component->Update();
        }
        Update();
    }
};
