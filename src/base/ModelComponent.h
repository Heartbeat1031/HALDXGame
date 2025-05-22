//
// Created by lclcl on 25-5-22.
//

#pragma once
#include <string_view>
#include "Component.h"
#include "Game.h"
#include "SoAStorage.h"

class ModelComponent : public Component {
private:
    void CheckTransform();
public:
    using Component::Component;
    void Init() override;
    void Update() override;
    void Uninit() override;
    SoAHandle handle = -1;

    void Load(std::string_view filename);
};
