#pragma once

#include <WinMin.h>
#include "d3dApp.h"
#include "Common/Effects.h"
#include <Camera.h>
#include <RenderStates.h>
#include <ModelObject.h>
#include <Texture2D.h>
#include <Buffer.h>
#include <ModelManager.h>
#include <TextureManager.h>

#include "SoAStorage.h"

class GameApp : public D3DApp
{
private:
    TextureManager m_TextureManager;                     // テクスチャマネージャー
    ModelManager m_ModelManager;                         // モデルマネージャー
    BasicEffect m_BasicEffect;                           // オブジェクト描画用エフェクト管理
    std::unique_ptr<Depth2D> m_pDepthTexture;            // デプスバッファ
    SoAStorage<ModelObject> m_ModelObjectStorage;        // モデルオブジェクトのストレージ
protected:
    bool Init() override;                                // 初期化処理
    void OnResize() override;                            // リサイズ時の処理
    void PreUpdate(float dt) override;                   // 更新前処理
    void Update(float dt) override;                      // 更新処理
    void PostUpdate(float dt) override;                  // 更新後処理
    void Draw() override;                                // 描画処理

public:
    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp() override;
    ID3D11Device *GetDevice() const ;
    // デバイスコンテキストを取得
    ID3D11DeviceContext *GetDeviceContext() const;
    std::shared_ptr<ThirdPersonCamera> m_pCamera;        // サードパーソンカメラ

    UID AddModel(std::string_view filename);       // モデルの追加
    ModelObject& GetModelObject(UID handle);       // モデルオブジェクトの取得
    bool RemoveModel(UID handle);                  // モデルの削除
};
