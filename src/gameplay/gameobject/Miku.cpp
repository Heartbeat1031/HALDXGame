//
// Created by lclcl on 25-5-30.
//

#include "Miku.h"

#include "ModelC.h"
#include "TransformC.h"

Miku::Miku() {
}

Miku::~Miku() {
}

void Miku::Init() {
    GameObject::Init();
    //AddComponent<ModelC>().Load("assets\\models\\GothicMiku\\Gothic Miku (Apearance Ver.)5.pmx");
}

void Miku::Update(float dt) {
    GameObject::Update(dt);
    if (ImGui::IsKeyPressed(ImGuiKey_Q)) {
        halgame->GetScene()->RemoveGameObject(GetUID());
    }
}

void Miku::Uninit() {
    GameObject::Uninit();
}
