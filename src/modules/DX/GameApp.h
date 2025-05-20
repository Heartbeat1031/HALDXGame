#pragma once

#include <WinMin.h>
#include "d3dApp.h"
#include "Effects.h"
#include <Camera.h>
#include <RenderStates.h>
#include <GameObject.h>
#include <Texture2D.h>
#include <Buffer.h>
#include <ModelManager.h>
#include <TextureManager.h>

#include "AoSNodeManager.h"

class GameApp : public D3DApp
{
private:
    TextureManager m_TextureManager;
    ModelManager m_ModelManager;

    BasicEffect m_BasicEffect;                                  // 对象渲染特效管理

    std::unique_ptr<Depth2D> m_pDepthTexture;                   // 深度缓冲区

    std::shared_ptr<ThirdPersonCamera> m_pCamera;				// 摄像机

    GameObject m_House;										    // 房屋
    GameObject m_House1;										    // 房屋
    GameObject m_Ground;										// 地面

    AoSNodeManager m_NodeManager;									// 节点管理器

    bool InitResource();

public:
    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    ~GameApp() override;

    bool Init() override;
    void OnResize() override;
    void UpdateScene(float dt) override;
    void DrawScene() override;
};
