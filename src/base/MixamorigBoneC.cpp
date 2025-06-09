//
// Created by clcc777 on 25-6-10.
//
#include "MixamorigBoneC.h"

#include <set>

#include "BoneObj.h"
#include "Global.h"
#include "ModelC.h"
#include "ModelManager.h"
#include "TransformC.h"

MixamorigBoneC::MixamorigBoneC() {
}

MixamorigBoneC::~MixamorigBoneC() {
}

void MixamorigBoneC::Init() {
    Component::Init();
    ModelC &modelC = GetComponent<ModelC>();
    ModelObject &modelObject = halgame->GetModelObject(modelC.GetHandle());
    const Model *model = modelObject.GetModel();
    auto it = model->boneNameToIndex.find("mixamorig:Hips");
    if (it == model->boneNameToIndex.end()) {
        throw std::runtime_error(" 'mixamorig:Hips' 骨骼が見つかりません。");
    }

    int hipsIndex = it->second;
    const BoneInfo &hipsBone = model->bones[hipsIndex];
    const BoneInfo &rootBone = model->bones[hipsBone.parentIndex];
    std::function<void (const BoneInfo &boneInfo, const GameObject &gameObject, std::set<int>& visited)> addBone =  [&](const BoneInfo& boneInfo, const GameObject &gameObject, std::set<int>& visited) {
        std::string boneName = boneInfo.name;
        TransformC &transform_c = gameObject.GetComponent<TransformC>();
        // 新しいBoneObjを作成
        BoneObj &boneObj = halgame->GetScene()->AddGameObject<BoneObj>();
        boneObj.SetName(boneName);
        TransformC &boneTransform = boneObj.GetComponent<TransformC>();
        transform_c.AddChild(&boneTransform);

        // 骨骼のnodeTransform から位置、回転、スケールを抽出
        DirectX::XMMATRIX mat = DirectX::XMLoadFloat4x4(&boneInfo.nodeTransform);
        DirectX::XMVECTOR scaleVec, rotQuat, transVec;
        DirectX::XMMatrixDecompose(&scaleVec, &rotQuat, &transVec, mat);
        DirectX::XMFLOAT3 scale, position;
        DirectX::XMFLOAT4 rotation;
        DirectX::XMStoreFloat3(&scale, scaleVec);
        DirectX::XMStoreFloat3(&position, transVec);
        DirectX::XMStoreFloat4(&rotation, rotQuat);
        // スケール、位置、回転を設定
        boneTransform.SetLocalPosition(position);
        boneTransform.SetLocalRotation(DirectX::SimpleMath::Quaternion(rotation.x, rotation.y, rotation.z, rotation.w));
        boneTransform.SetLocalScale(DirectX::SimpleMath::Vector3(scale.x, scale.y, scale.z));

        //  子骨骼を再帰的に追加
        for (int index : boneInfo.children) {
            if (visited.find(index) != visited.end()) {
                continue; // 既に訪問済みの骨骼はスキップ
            }
            visited.insert(index);
            addBone(model->bones[index], boneObj, visited);
            visited.erase(index);
        }
    };
    std::set<int> visited;
    visited.insert(hipsIndex);
    addBone(hipsBone, *m_gameObject, visited);
}

void MixamorigBoneC::Update(float dt) {
    Component::Update(dt);
}

void MixamorigBoneC::Uninit() {
    Component::Uninit();
}

