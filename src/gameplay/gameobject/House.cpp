//
// Created by lclcl on 25-5-22.
//

#include "House.h"
#include "ModelComponent.h"

House::House() {
}

House::~House() {
}

void House::Init() {
    GameObject::Init();
    AddComponent<ModelComponent>()->Load("assets\\models\\house.obj");
    SetScale(Vector3(0.015f, 0.015f, 0.015f));
    SetPosition(Vector3(0, -1, 0));
}

