//
// Created by clcc777 on 25-9-30.
//

#pragma once
#include "NetGameObject.h"

class NetPlayer : public NetGameObject {
private:


public:
    NetPlayer();
    ~NetPlayer() override;
    void Init() override;
    void Update(float dt) override;
    void Uninit() override;
};
