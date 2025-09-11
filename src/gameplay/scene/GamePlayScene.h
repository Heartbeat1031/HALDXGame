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

    float FireTimer = 0.0f;
    int   FiredCount = 0;

    static constexpr int   TotalShots   = 180;
    static constexpr float FireInterval = 0.25f;   // 每秒4发
    static constexpr float BulletSpeed  = 20.0f;

    Vector3 FireCenter = Vector3(0.0f, 4.0f, 0.0f); // 如果要 0.4f，把 4.0f 改成 0.4f
    float   FireRadius = 10.0f;                     // 由 (0,4,-10) 到中心 (0,4,0) 的距离
public:
    Player *miku = nullptr;

    void Init() override;
    void Update(float dt) override;
    void Uninit() override;

    void FireBullet(Vector3 StartPosition, Vector3 Direction, float Speed);
};
