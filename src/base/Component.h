#pragma once
#include "SoAStorage.h"

//------------------------------------------------------//
// 这个文件用来定义组件基类 负责实现GameObject的附属功能
// このファイルはコンポーネントの基底クラスを定義し、GameObjectの付属機能を実装します
//------------------------------------------------------//

class Component
{
private:
	// コンポーネントの一意の識別子
	UID uid = -1;
protected:
	// 所属するGameObject
	class  GameObject* m_gameobject = nullptr;

public:
	Component(GameObject* parent) { m_gameobject = parent; }
	virtual ~Component() {}
	// コンポーネントのUIDを設定
	void SetUID(UID uuid) { uid = uuid; }
	// コンポーネントのUIDを取得
	UID GetUID() const { return uid; }

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update(float dt) {};
};
