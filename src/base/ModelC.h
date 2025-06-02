//
// Created by lclcl on 25-5-22.
//

#pragma once
#include <string_view>
#include "Component.h"
#include "Game.h"
#include "SoAStorage.h"

//---------------------------------------------------------------//
// ModelC类用于给GameObject添加模型显示功能
// ModelCクラスはGameObjectにモデル表示機能を追加するためのものです
//---------------------------------------------------------------//

class ModelC : public Component {
public:
    using Component::Component;
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;
    // 缓存模型句柄
    // モデルハンドルをキャッシュします
    UID handle = -1;
    // 加载模型 添加ModelC的时候需要调用此函数
    // モデルをロードします ModelCを追加するときにこの関数を呼び出す必要があります
    void Load(std::string_view filename);

private:
    // 检查并且更新模型的Transform
    // モデルのTransformをチェックして更新します
    void CheckTransform() const;
};
