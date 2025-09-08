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

class Idol;

// サンプルのテストシーン
class GamePlayScene : public Scene{
    //一時的な変数です
    Vector3 BaseFirePosition;
private:
    Idol *idol = nullptr;
public:
    Player *miku = nullptr;

    void Init() override;
    void Update(float dt) override;
    void Uninit() override;

    void FireBullet(Vector3 StartPosition, Vector3 Direction, float Speed);
};
