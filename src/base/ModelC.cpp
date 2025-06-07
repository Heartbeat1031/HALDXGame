//
// Created by lclcl on 25-5-22.
//

#include "ModelC.h"

#include <iostream>

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
    TransformC &transformComponent = GetComponent<TransformC>();
    if (HasComponent<MixamoRagdollC>()) {
        auto &ragdollC = GetComponent<MixamoRagdollC>();
        modelObject.SetBoneMatrices(&ragdollC.GetFinalBoneMatrices());
        // 更新模型Transform以跟随ragdoll根节点
        // RVec3 outPosition;
        // Quat outRotation;
        // ragdollC.mRagdoll->GetRootTransform(outPosition, outRotation);
        // transformComponent.SetWorldPosition(Vector3(outPosition.GetX(), outPosition.GetY(), outPosition.GetZ()));
        // transformComponent.SetWorldRotation(
        //     DirectX::SimpleMath::Quaternion(outRotation.GetX(), outRotation.GetY(), outRotation.GetZ(),
        //                                     outRotation.GetW()));
        // transformComponent.SetLocalRotationEuler(transformComponent.GetLocalRotationEuler());
    } else if (HasComponent<AnimatorC>()) {
        auto &animC = GetComponent<AnimatorC>();
        modelObject.SetBoneMatrices(&animC.GetFinalBoneMatrices());
    }
    modelTransform.SetWorldMatrix(transformComponent.GetWorldMatrix());
}
