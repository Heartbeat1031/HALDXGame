//
// Created by lclcl on 25-5-22.
//

#include "ModelComponent.h"

#include "GameObject.h"
#include "Global.h"
#include "SimpleMath.h"
#include "TransformComponent.h"

void ModelComponent::CheckTransform() const {
    if (handle == -1) {
        return;
    }
    ModelObject &modelObject = halgame->GetModelObject(handle);
    Transform &modelTransform = modelObject.GetTransform();
    TransformComponent &transformComponent = m_gameobject->GetComponent<TransformComponent>();
    modelTransform.SetWorldMatrix(transformComponent.GetWorldMatrix());
}

void ModelComponent::Init() {
    Component::Init();
}

void ModelComponent::Update(float dt) {
    Component::Update(dt);
    CheckTransform();
}

void ModelComponent::Uninit() {
    Component::Uninit();
    halgame->RemoveModel(handle);
}

void ModelComponent::Load(std::string_view filename) {
    handle = halgame->AddModel(filename);
}
