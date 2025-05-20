#pragma once

#include <WinMin.h>
#include "d3dApp.h"
#include "Effects.h"
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
    TextureManager m_TextureManager;
    ModelManager m_ModelManager;
    BasicEffect m_BasicEffect;                                               // 对象渲染特效管理
    std::unique_ptr<Depth2D> m_pDepthTexture;                 // 深度缓冲区
    SoAStorage<ModelObject> m_ModelObjectStorage;	   // 模型对象存储

protected:
    std::shared_ptr<ThirdPersonCamera> m_pCamera;			// 摄像机
    bool Init() override;
    void OnResize() override;
    void PreUpdate(float dt) override;
    void Update(float dt) override;
    void PostUpdate(float dt) override;
    void Draw() override;
public:
    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp() override;

    SoAHandle GameApp::AddModel(std::string_view filename);
    ModelObject &GetModelObject(SoAHandle handle);
    bool RemoveModel(SoAHandle handle);

};
