#pragma once

#include <list>
#include <array>
#include <vector>
#include <typeinfo>
#include "GameObject.h"

class Scene {
protected:
    std::array<std::list<GameObject *>, 4> m_GameObject;

public:
    Scene() {
    }

    virtual ~Scene() {
    }

    virtual void Init() {
    }

    virtual void Uninit() {
    }

    virtual void Update() {
    }

    void InitBase() {
        Init();
    }

    void UninitBase() {
        for (auto &objectList: m_GameObject) {
            for (GameObject *object: objectList) {
                object->UninitBase();
                delete object;
            }
            objectList.clear();
        }
        Uninit();
    }

    void UpdateBase() {
        for (auto &objectList: m_GameObject) {
            for (GameObject *object: objectList) {
                object->UpdateBase();
            }
            objectList.remove_if([](GameObject *object) {
                return object->Destroy();
            });
        }
        Update();
    }

    template<typename T>
    T *AddGameObject(int Layer) {
        T *gameObject = new T();
        m_GameObject[Layer].push_back(gameObject);

        gameObject->Init();

        return gameObject;
    }

    template<typename T, std::enable_if_t<std::is_base_of_v<GameObject, T>, int>  = 0>
    T *GetGameObject() {
        for (auto &objectList: m_GameObject) {
            for (GameObject *object: objectList) {
                // 利用 RTTI (Run-Time Type Information) 判斷實際型別
                if (typeid(*object) == typeid(T)) {
                    return (T *) object;
                }
            }
        }
        return nullptr;
    }

    template<typename T, std::enable_if_t<std::is_base_of_v<GameObject, T>, int>  = 0>
    std::vector<T *> GetGameObjects() {
        std::vector<T *> objects;

        for (auto &objectList: m_GameObject) {
            for (GameObject *object: objectList) {
                if (typeid(*object) == typeid(T)) {
                    objects.push_back((T *) object);
                }
            }
        }
        return objects;
    }
};
