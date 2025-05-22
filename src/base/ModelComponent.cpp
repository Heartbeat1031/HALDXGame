//
// Created by lclcl on 25-5-22.
//

#include "ModelComponent.h"

#include "Global.h"

void ModelComponent::CheckTransform() {
    if (handle == -1) {
        return;
    }
    ModelObject &modelObject = halgame->GetModelObject(handle);
    Transform &houseTransform = modelObject.GetTransform();
    Vector3 position = m_Parent->GetPosition();
    Vector3 rotation = m_Parent->GetRotation();
    Vector3 scale = m_Parent->GetScale();
    houseTransform.SetPosition(position);
    houseTransform.SetRotation(rotation);
    houseTransform.SetScale(scale);
}

void ModelComponent::Init() {
    Component::Init();
}

void ModelComponent::Update() {
    Component::Update();
    CheckTransform();
}

void ModelComponent::Uninit() {
    Component::Uninit();
    halgame->RemoveModel(handle);
}

void ModelComponent::Load(std::string_view filename) {
    handle = halgame->AddModel(filename);
}
