#include "GameApp.h"
#include <XUtil.h>
#include <DXTrace.h>

using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance, const std::wstring &windowName, int initWidth, int initHeight)
    : D3DApp(hInstance, windowName, initWidth, initHeight) {
}

GameApp::~GameApp() = default;

ID3D11Device * GameApp::GetDevice() const {
    return  m_pd3dDevice1 ? m_pd3dDevice1.Get() : m_pd3dDevice.Get();
}

ID3D11DeviceContext * GameApp::GetDeviceContext() const {
    return  m_pd3dImmediateContext1 ? m_pd3dImmediateContext1.Get() : m_pd3dImmediateContext.Get();
}

bool GameApp::Init() {
    if (!D3DApp::Init()) {
        return false;
    }

    m_TextureManager.Init(m_pd3dDevice.Get());
    m_ModelManager.Init(m_pd3dDevice.Get());

    // 必ず最初にすべてのレンダリングステートを初期化し、以下のエフェクトで使用できるようにする
    RenderStates::InitAll(m_pd3dDevice.Get());

    if (!m_BasicEffect.InitAll(m_pd3dDevice.Get()))
        return false;

    // ******************
    // カメラの初期化
    //

    auto camera = std::make_shared<ThirdPersonCamera>();
    m_pCamera = camera;

    camera->SetViewPort(0.0f, 0.0f, (float) m_ClientWidth, (float) m_ClientHeight);
    camera->SetTarget(XMFLOAT3(0.0f, 0.5f, 0.0f));
    camera->SetDistance(15.0f);
    camera->SetDistanceMinMax(6.0f, 100.0f);
    camera->SetRotationX(XM_PIDIV4);
    camera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);

    m_BasicEffect.SetWorldMatrix(XMMatrixIdentity());
    m_BasicEffect.SetViewMatrix(camera->GetViewMatrixXM());
    m_BasicEffect.SetProjMatrix(camera->GetProjMatrixXM());
    m_BasicEffect.SetEyePos(camera->GetPosition());

    // ******************
    // 変更されない値の初期化
    //

    // 環境光（ディレクショナルライト）
    DirectionalLight dirLight{};
    dirLight.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    dirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
    dirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
    dirLight.direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
    m_BasicEffect.SetDirLight(0, dirLight);

    // ポイントライト（点光源）
    PointLight pointLight{};
    pointLight.position = XMFLOAT3(0.0f, 20.0f, 0.0f);
    pointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
    pointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
    pointLight.specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
    pointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
    pointLight.range = 30.0f;
    m_BasicEffect.SetPointLight(0, pointLight);

    // ******************
    // ゲームオブジェクトの初期化（サブクラスに委ねる）
    //

    return true;
}

void GameApp::PreUpdate(float dt) {
    // フレーム更新前の処理（必要に応じて実装）
}

void GameApp::Update(float dt) {
    // 毎フレームの更新処理（必要に応じて実装）
}

void GameApp::PostUpdate(float dt) {
    // フレーム更新後にビュー行列とカメラ位置を更新
    m_BasicEffect.SetViewMatrix(m_pCamera->GetViewMatrixXM());
    m_BasicEffect.SetEyePos(m_pCamera->GetPosition());
}

void GameApp::Draw() {
    // バックバッファのレンダーターゲットビューを作成
    if (m_FrameCount < m_BackBufferCount) {
        ComPtr<ID3D11Texture2D> pBackBuffer;
        m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()));
        CD3D11_RENDER_TARGET_VIEW_DESC rtvDesc(D3D11_RTV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
        m_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc,
                                             m_pRenderTargetViews[m_FrameCount].ReleaseAndGetAddressOf());
    }

    // 画面のクリア
    float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    m_pd3dImmediateContext->ClearRenderTargetView(GetBackBufferRTV(), black);
    m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthTexture->GetDepthStencil(),
                                                  D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    ID3D11RenderTargetView *pRTVs[1] = {GetBackBufferRTV()};
    m_pd3dImmediateContext->OMSetRenderTargets(1, pRTVs, m_pDepthTexture->GetDepthStencil());
    D3D11_VIEWPORT viewport = m_pCamera->GetViewPort();
    m_pd3dImmediateContext->RSSetViewports(1, &viewport);

    // 描画設定の適用
    m_BasicEffect.SetRenderDefault();

    // モデル描画キュー
    m_ModelObjectStorage.ForEachActive([this](UID id, ModelObject &modelObject) {
        modelObject.Draw(m_pd3dImmediateContext.Get(), m_BasicEffect);
    });

    // ImGuiの描画
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // フレームの表示
    HR(m_pSwapChain->Present(0, m_IsDxgiFlipModel ? DXGI_PRESENT_ALLOW_TEARING : 0));
}

// ウィンドウサイズが変更された際に呼び出される関数
void GameApp::OnResize() {
    D3DApp::OnResize();

    m_pDepthTexture = std::make_unique<Depth2D>(m_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight);
    m_pDepthTexture->SetDebugObjectName("DepthTexture");

    // カメラの視野更新
    if (m_pCamera != nullptr) {
        m_pCamera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);
        m_pCamera->SetViewPort(0.0f, 0.0f, (float) m_ClientWidth, (float) m_ClientHeight);
        m_BasicEffect.SetProjMatrix(m_pCamera->GetProjMatrixXM());
    }
}

////////////////////////////新しいコード/////////////////////////////////////
// モデルを追加
UID GameApp::AddModel(std::string_view filename) {
    Model *model = m_ModelManager.GetModel(filename);
    if (model == nullptr) {
        model = m_ModelManager.CreateFromFile(filename);
    }
    std::unique_ptr<ModelObject> pModelObject = std::make_unique<ModelObject>();
    pModelObject->SetModel(model);
    return m_ModelObjectStorage.Add(std::move(pModelObject));
}

// モデルオブジェクトの取得
ModelObject &GameApp::GetModelObject(UID handle) {
    return m_ModelObjectStorage.Get(handle);
}

// モデルの削除
bool GameApp::RemoveModel(UID handle) {
    m_ModelObjectStorage.Remove(handle);
    return true;
}
