//
// Created by lclcl on 25-5-22.
//

#include "ModelC.h"

#include "GameObject.h"
#include "Global.h"
#include "SimpleMath.h"
#include "TransformC.h"

void ModelC::Init() {
    Component::Init();
}

void ModelC::Update(float dt) {
    Component::Update(dt);
    // 毎フレームモデルのTransformをチェックして更新します
    CheckTransform();
}

void ModelC::Uninit() {
    Component::Uninit();
    // ハンドルに基づいてモデルを削除します
    halgame->RemoveModel(handle);
}

void ModelC::Load(std::string_view filename) {
    handle = halgame->AddModel(filename);
}

void ModelC::CheckTransform() const {
    if (handle == -1) {
        return;
    }
    ModelObject &modelObject = halgame->GetModelObject(handle);
    Transform &modelTransform = modelObject.GetTransform();
    TransformC &transformComponent = m_gameobject->GetComponent<TransformC>();
    modelTransform.SetWorldMatrix(transformComponent.GetWorldMatrix());
}