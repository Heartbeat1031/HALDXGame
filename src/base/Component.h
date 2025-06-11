#pragma once
#include <stdexcept>

#include "SoAStorage.h"

//------------------------------------------------------//
// 这个文件用来定义组件基类 负责实现GameObject的附属功能
// このファイルはコンポーネントの基底クラスを定義し、GameObjectの付属機能を実装します
//------------------------------------------------------//

class GameObject;

class Component
{
private:
	// コンポーネントの一意の識別子
	UID uid = -1;
protected:
	std::string m_Name = "Component"; // コンポーネントの名前
	// 所属するGameObject
	GameObject* m_gameObject = nullptr;

public:
	Component();
	virtual ~Component();
	bool operator==(const Component * selected) const;
	// コンポーネントの所属するGameObjectを設定
	void SetGameObject(GameObject* gameObject);

	GameObject *GetGameObject() const;
	// コンポーネントのUIDを設定
	void SetUID(UID uuid);
	// コンポーネントのUIDを取得
	UID GetUID() const;

	virtual void Init();
	virtual void Uninit();
	virtual void Update(float dt);

	// コンポーネントの名前を設定・取得
	void SetName(std::string name){ m_Name = name; }
	std::string GetName() const { return m_Name; }

	template<class T>
	T &GetComponentRef();

	template<class T>
	bool HasComponent() const;

	virtual void OnInspectorGUI();
};

template<typename T>
T &Component::GetComponentRef() {
	if (m_gameObject == nullptr) {
		throw std::runtime_error("GameObjectが設定されていません"); // GameObjectが設定されていない場合は例外を投げる
	}
	return m_gameObject->GetComponentRef<T>();
}

template<typename T>
bool Component::HasComponent() const {
	if (m_gameObject == nullptr) {
		return false; // GameObjectが設定されていない場合はfalseを返す
	}
	return m_gameObject->HasComponent<T>();
}