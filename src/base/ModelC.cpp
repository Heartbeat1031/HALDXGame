//
// Created by lclcl on 25-5-22.
//

#include "ModelC.h"

#include "AnimatorC.h"
#include "GameObject.h"
#include "Global.h"
#include "MixamoRagdollC.h"
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
    UpdateBones();
}

void ModelC::Uninit() {
    Component::Uninit();
    // ハンドルに基づいてモデルを削除します
    halgame->RemoveModel(handle);
}

ModelObject & ModelC::GetModelObject() {
        return halgame->GetModelObject(handle);
}

void ModelC::CheckTransform() const {
    if (handle == -1) {
        return;
    }
    ModelObject &modelObject = halgame->GetModelObject(handle);
    Transform &modelTransform = modelObject.GetTransform();
    TransformC &transformComponent = m_gameObject->GetComponent<TransformC>();
    modelTransform.SetWorldMatrix(transformComponent.GetWorldMatrix());
}

void ModelC::UpdateBones() {
    ModelObject &modelObject = halgame->GetModelObject(handle);

    //if (HasComponent<MixamoRagdollC>() && GetComponent<MixamoRagdollC>().mRagdoll->IsActive()) {
    if (false){
        auto &ragdollC = GetComponent<MixamoRagdollC>();
        modelObject.SetBoneMatrices(&ragdollC.GetFinalBoneMatrices());
    } else if (HasComponent<MixamoRagdollC>()) {
        auto &animC = GetComponent<AnimatorC>();
        modelObject.SetBoneMatrices(&animC.GetFinalBoneMatrices());
    }
}
