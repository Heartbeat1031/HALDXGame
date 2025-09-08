//
// Created by lclcl on 25-5-22.
//
#pragma once
#include "GameObject.h"
//----------------------------------------------------------------//
// House类是一个示例游戏对象，代表一个房子
// Houseクラスは、ゲームオブジェクトの一例であり、家を表します
//----------------------------------------------------------------//

class Idol:public GameObject {
private:
    int HP = 10;
public:
    Idol();
    ~Idol() override;
    void Init() override;
    void Update(float dt) override;

    void Uninit() override;
public:
    void BeAttacked(int damage);
};
