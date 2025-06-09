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


//---------------------------------------------------------------//
// GameObject类是一切游戏物体的基础：角色、敌人、道具、地板……统统都是 GameObject。
// 可以添加Component来扩展功能，比如添加ModelC的话就可以拥有模型显示功能，添加TransformC就可以拥有位置、旋转、缩放等变换功能。
// 注意：TransformC默认会被添加到每个GameObject中，所以不需要手动添加。
//---------------------------------------------------------------//
// GameObjectクラスは、すべてのゲームオブジェクトの基底クラスです：キャラクター、敵、アイテム、床など、すべてがGameObjectです。
// コンポーネントを追加することで機能を拡張できます。例えば、ModelCを追加するとモデル表示機能が、TransformCを追加すると位置、回転、スケーリングなどの変換機能が得られます。
// 注意：TransformCはデフォルトで各GameObjectに追加されるため、手動で追加する必要はありません。
//---------------------------------------------------------------//


// このファイルはゲームオブジェクトの基底クラスを定義します。
class GameObject {
private:
    // ゲームオブジェクトの一意の識別子
    UID uid = -1;
    std::unordered_map<std::type_index, UID> m_ComponentMap;
protected:
    // GameObjectの初期化
    virtual void Init(){};
    // GameObjectの解放
    virtual void Uninit(){};
    // GameObjectの更新
    virtual void Update(float dt){};
    // ゲームオブジェクトの名前
    std::string m_Name;

public:
    GameObject();
    virtual ~GameObject();
    // ゲームオブジェクトのUIDを設定
    void SetUID(UID uuid);
    // ゲームオブジェクトのUIDを取得
    UID GetUID() const;
    // ゲームオブジェクトの初期化
    void InitBase();
    // ゲームオブジェクトの解放
    void UninitBase();
    // ゲームオブジェクトの更新
    void UpdateBase(float dt);

    // ゲームオブジェクトの名前を設定・取得
    void SetName(std::string name){ m_Name = name; }
    std::string GetName() const { return m_Name; }

    // コンポーネントを追加する
    template<class T, class ... Args>
    T &AddComponent(Args &&... args);

    // コンポーネントを取得する
    template<typename T>
    T &GetComponent() const;

    // コンポーネントが存在するか確認する
    template<class T>
    bool HasComponent() const;
};


template<typename T, typename... Args>
T &GameObject::AddComponent(Args&&... args) {
    // TはComponentの派生クラスである必要があります
    static_assert(std::is_base_of<Component, T>::value, "T は Component の派生クラスでなければなりません");
    // 同じタイプのコンポーネントがすでに存在するか確認
    auto it = m_ComponentMap.find(std::type_index(typeid(T)));
    if (it != m_ComponentMap.end()) {
        // すでに同じタイプのコンポーネントが存在する場合はエラーを出す
        assert(false && "ゲームオブジェクトに重複コンポーネントが追加されました");
        return GetComponent<T>();
    }
    // 今はSceneを取得して、コンポーネントを追加する
    Scene *scene = halgame->GetScene();
    // Tのインスタンスを作成し、GameObjectに追加する
    T &component = scene->AddComponent<T>(this, std::forward<Args>(args)...);
    // コンポーネントのUIDをキャッシュする
    m_ComponentMap[std::type_index(typeid(T))] = component.GetUID();
    return component;
}

template<typename T>
T &GameObject::GetComponent() const {
    auto it = m_ComponentMap.find(std::type_index(typeid(T)));
    if (it == m_ComponentMap.end()) {
        throw std::runtime_error("ゲームオブジェクトにコンポーネントが見つかりません");
    }
    return halgame->GetScene()->GetComponent<T>(it->second);
}

template<typename T>
bool GameObject::HasComponent() const {
    auto it = m_ComponentMap.find(std::type_index(typeid(T)));
    return it != m_ComponentMap.end();
}
