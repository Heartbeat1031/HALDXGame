//
// Created by clcc777 on 25-9-30.
//

#pragma once
#include "GameObject.h"


class NetGameObject : public GameObject {
public:
    NetGameObject();

    ~NetGameObject() override;

    void Init() override;

    void Update(float dt) override;

    void Uninit() override;
};
