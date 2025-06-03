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
	class  GameObject* m_gameObject = nullptr;

public:
	Component();
	virtual ~Component();
	// コンポーネントの所属するGameObjectを設定
	void SetGameObject(GameObject* gameObject);
	// コンポーネントのUIDを設定
	void SetUID(UID uuid);
	// コンポーネントのUIDを取得
	UID GetUID() const;

	virtual void Init();
	virtual void Uninit();
	virtual void Update(float dt);
};
