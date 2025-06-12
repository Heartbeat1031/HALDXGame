//
// Created by lclcl on 25-5-22.
//

#pragma once
#include "Miku.h"
#include "Scene.h"
//----------------------------------------------------------------//
// 这是一个测试场景类，用于演示如何使用Scene类和GameObject类
// このクラスは、SceneクラスとGameObjectクラスの使用方法を示すためのテストシーンです。
//----------------------------------------------------------------//

// サンプルのテストシーン
class TestScene : public Scene{
public:
    Miku *miku = nullptr;

    void Init() override;
    void Update() override;
    void Uninit() override;
};
