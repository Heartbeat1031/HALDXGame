//
// Created by lclcl on 25-5-22.
//

#include "ModelC.h"

#include <iostream>

#include "AnimatorC.h"
#include "GameObject.h"
#include "Global.h"
#include "MixamorigBoneC.h"
#include "SimpleMath.h"
#include "TransformC.h"

ModelC::ModelC(std::string_view filename) {
    handle = halgame->AddModel(filename);
}

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

ModelObject &ModelC::GetModelObject() {
    return halgame->GetModelObject(handle);
}

void ModelC::CheckTransform() {
    if (handle == -1) {
        return;
    }
    ModelObject &modelObject = halgame->GetModelObject(handle);
    Transform &modelTransform = modelObject.GetTransform();
    TransformC &transformComponent = GetComponentRef<TransformC>();
    if (HasComponent<MixamorigBoneC>()) {
        auto &ragdollC = GetComponentRef<MixamorigBoneC>();
        modelObject.SetBoneMatrices(&ragdollC.GetFinalBoneMatrices());
    } else if (HasComponent<AnimatorC>()) {
        auto &animC = GetComponentRef<AnimatorC>();
        modelObject.SetBoneMatrices(&animC.GetFinalBoneMatrices());
    }
    modelTransform.SetWorldMatrix(transformComponent.GetWorldMatrix());
}
