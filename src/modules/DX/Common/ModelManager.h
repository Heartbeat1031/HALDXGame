//***************************************************************************************
// GameObject.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// 简易游戏对象
// Simple game object.
//***************************************************************************************

#pragma once

#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "WinMin.h"
#include "Geometry.h"
#include "Material.h"
#include "MeshData.h"
#include <d3d11_1.h>
#include <wrl/client.h>

struct DXModel
{
    DXModel() = default;
    ~DXModel() = default;
    DXModel(DXModel&) = delete;
    DXModel& operator=(const DXModel&) = delete;
    DXModel(DXModel&&) = default;
    DXModel& operator=(DXModel&&) = default;

    std::vector<Material> materials;
    std::vector<MeshData> meshdatas;
    DirectX::BoundingBox boundingbox;
    static void CreateFromFile(DXModel& model, ID3D11Device* device, std::string_view filename);
    static void CreateFromGeometry(DXModel& model, ID3D11Device* device, const GeometryData& data, bool isDynamic = false);

    void SetDebugObjectName(std::string_view name);
};

class ModelManager
{
public:
    ModelManager();
    ~ModelManager();
    ModelManager(ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;
    ModelManager(ModelManager&&) = default;
    ModelManager& operator=(ModelManager&&) = default;

    static ModelManager& Get();
    void Init(ID3D11Device* device);
    DXModel* CreateFromFile(std::string_view filename);
    DXModel* CreateFromFile(std::string_view name, std::string_view filename);
    DXModel* CreateFromGeometry(std::string_view name, const GeometryData& data, bool isDynamic = false);

    const DXModel* GetModel(std::string_view name) const;
    DXModel* GetModel(std::string_view name);
private:
    Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
    std::unordered_map<size_t, DXModel> m_Models;
};


#endif // MODELMANAGER_H
