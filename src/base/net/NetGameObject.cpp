//
// Created by clcc777 on 25-9-30.
//

#include "NetGameObject.h"

NetGameObject::NetGameObject() {
}

NetGameObject::~NetGameObject() {
}

void NetGameObject::Init() {
    GameObject::Init();
}

void NetGameObject::Update(float dt) {
    GameObject::Update(dt);
}

void NetGameObject::Uninit() {
    GameObject::Uninit();
}
