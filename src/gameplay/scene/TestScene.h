//
// Created by lclcl on 25-5-22.
//

#pragma once
#include "Player.h"
#include "Scene.h"
//----------------------------------------------------------------//
// 这是一个测试场景类，用于演示如何使用Scene类和GameObject类
// このクラスは、SceneクラスとGameObjectクラスの使用方法を示すためのテストシーンです。
//----------------------------------------------------------------//

// サンプルのテストシーン
class TestScene : public Scene{
    //一時的な変数です
    Vector3 BaseFirePosition;
public:
    Player *miku = nullptr;

    void Init() override;
    void Update() override;
    void Uninit() override;

    void FireBulletFromEnemy();
};
