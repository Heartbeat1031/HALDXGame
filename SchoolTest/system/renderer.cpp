/**
 * @file Renderer.cpp
 * @brief DirectX を用いたレンダリング処理を実装する Renderer クラスの定義（実装部）。
 *
 * このファイルでは、Direct3D デバイス、スワップチェーン、レンダーターゲット、定数バッファなどの
 * 初期化と解放、描画の開始・終了処理、各種レンダリング状態（深度、ブレンド、マトリックスなど）の設定を行います。
 */

#include "renderer.h"
#include "Application.h"

//------------------------------------------------------------------------------
// スタティックメンバ変数の初期化
//------------------------------------------------------------------------------

D3D_FEATURE_LEVEL       Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device* Renderer::m_Device{};
ID3D11DeviceContext* Renderer::m_DeviceContext{};
IDXGISwapChain* Renderer::m_SwapChain{};
ID3D11RenderTargetView* Renderer::m_RenderTargetView{};
ID3D11DepthStencilView* Renderer::m_DepthStencilView{};

ID3D11Buffer* Renderer::m_WorldBuffer{};
ID3D11Buffer* Renderer::m_ViewBuffer{};
ID3D11Buffer* Renderer::m_ProjectionBuffer{};
ID3D11Buffer* Renderer::m_MaterialBuffer{};
ID3D11Buffer* Renderer::m_LightBuffer{};

ID3D11DepthStencilState* Renderer::m_DepthStateEnable{};
ID3D11DepthStencilState* Renderer::m_DepthStateDisable{};

ID3D11BlendState* Renderer::m_BlendState[MAX_BLENDSTATE];
ID3D11BlendState* Renderer::m_BlendStateATC{};

//------------------------------------------------------------------------------
// Renderer クラスの各関数の実装
//------------------------------------------------------------------------------

/**
 * @brief Renderer の初期化処理を行います。
 *
 * Direct3D デバイスとスワップチェーンの作成、レンダーターゲットビュー、デプスステンシルビュー、
 * ビューポート、ラスタライザ、ブレンドステート、深度ステンシルステート、サンプラーステート、
 * 定数バッファの生成、初期ライトおよびマテリアルの設定などを実施します。
 */
void Renderer::Init()
{
    HRESULT hr = S_OK;

    // --- デバイス、スワップチェーン作成 ---
    DXGI_SWAP_CHAIN_DESC swapChainDesc{};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = Application::GetWidth();
    swapChainDesc.BufferDesc.Height = Application::GetHeight();
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = Application::GetWindow();
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT, // DIRECTWRITE 対応
        NULL,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_SwapChain,
        &m_Device,
        &m_FeatureLevel,
        &m_DeviceContext);

    // --- レンダーターゲットビューの作成 ---
    ID3D11Texture2D* renderTarget{};
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
    m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
    renderTarget->Release();

    // --- デプスステンシルバッファとビューの作成 ---
    ID3D11Texture2D* depthStencile{};
    D3D11_TEXTURE2D_DESC textureDesc{};
    textureDesc.Width = swapChainDesc.BufferDesc.Width;
    textureDesc.Height = swapChainDesc.BufferDesc.Height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_D16_UNORM;
    textureDesc.SampleDesc = swapChainDesc.SampleDesc;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;
    m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
    depthStencilViewDesc.Format = textureDesc.Format;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Flags = 0;
    m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
    depthStencile->Release();

    // --- レンダーターゲットとデプスステンシルビューのバインド ---
    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

    // --- ビューポート設定 ---
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(Application::GetWidth());
    viewport.Height = static_cast<FLOAT>(Application::GetHeight());
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    m_DeviceContext->RSSetViewports(1, &viewport);

    // --- ラスタライザステート設定 ---
    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    // rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;  // ワイヤーフレーム表示の場合
       rasterizerDesc.CullMode = D3D11_CULL_BACK;
    // rasterizerDesc.CullMode = D3D11_CULL_NONE;       // カリング無効の場合
    // rasterizerDesc.CullMode = D3D11_CULL_FRONT;      // フロントカリングの場合
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.MultisampleEnable = FALSE;

    ID3D11RasterizerState* rs;
    m_Device->CreateRasterizerState(&rasterizerDesc, &rs);
    m_DeviceContext->RSSetState(rs);
    rs->Release();

    // --- ブレンドステートの生成 ---
    D3D11_BLEND_DESC BlendDesc;
    ZeroMemory(&BlendDesc, sizeof(BlendDesc));
    BlendDesc.AlphaToCoverageEnable = FALSE;
    BlendDesc.IndependentBlendEnable = TRUE;
    BlendDesc.RenderTarget[0].BlendEnable = FALSE;
    BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    m_Device->CreateBlendState(&BlendDesc, &m_BlendState[0]);

    // アルファ ブレンド用ブレンドステート生成
    // BlendDesc.AlphaToCoverageEnable = TRUE; // 必要に応じて
    BlendDesc.RenderTarget[0].BlendEnable = TRUE;
    m_Device->CreateBlendState(&BlendDesc, &m_BlendState[1]);
    m_Device->CreateBlendState(&BlendDesc, &m_BlendStateATC);

    // 加算合成用ブレンドステート生成
    BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    m_Device->CreateBlendState(&BlendDesc, &m_BlendState[2]);

    // 減算合成用ブレンドステート生成
    BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
    m_Device->CreateBlendState(&BlendDesc, &m_BlendState[3]);

    // 初期ブレンドステートとしてアルファ ブレンドを設定
    SetBlendState(BS_ALPHABLEND);

    // --- 深度ステンシルステートの設定 ---
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    // depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; // 必要に応じて
    depthStencilDesc.StencilEnable = FALSE;

    m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable); // 深度テスト有効
    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable); // 深度テスト無効

    m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);

    // --- サンプラーステート設定 ---
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MaxAnisotropy = 4;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ID3D11SamplerState* samplerState{};
    m_Device->CreateSamplerState(&samplerDesc, &samplerState);
    m_DeviceContext->PSSetSamplers(0, 1, &samplerState);
    samplerState->Release();

    // --- 定数バッファ生成 ---
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = sizeof(Matrix4x4);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_WorldBuffer);
    m_DeviceContext->VSSetConstantBuffers(0, 1, &m_WorldBuffer);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_ViewBuffer);
    m_DeviceContext->VSSetConstantBuffers(1, 1, &m_ViewBuffer);

    m_Device->CreateBuffer(&bufferDesc, NULL, &m_ProjectionBuffer);
    m_DeviceContext->VSSetConstantBuffers(2, 1, &m_ProjectionBuffer);

    bufferDesc.ByteWidth = sizeof(MATERIAL);
    m_Device->CreateBuffer(&bufferDesc, NULL, &m_MaterialBuffer);
    m_DeviceContext->VSSetConstantBuffers(3, 1, &m_MaterialBuffer);
    m_DeviceContext->PSSetConstantBuffers(3, 1, &m_MaterialBuffer);

    bufferDesc.ByteWidth = sizeof(LIGHT);
    m_Device->CreateBuffer(&bufferDesc, NULL, &m_LightBuffer);
    m_DeviceContext->VSSetConstantBuffers(4, 1, &m_LightBuffer);
    m_DeviceContext->PSSetConstantBuffers(4, 1, &m_LightBuffer);

    // --- ライト初期化 ---
    LIGHT light{};
    light.Enable = true;
    light.Direction = Vector4(0.5f, -1.0f, 0.8f, 0.0f);
    light.Direction.Normalize();
    light.Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
    light.Diffuse = Color(1.5f, 1.5f, 1.5f, 1.0f);
    SetLight(light);

    // --- マテリアル初期化 ---
    MATERIAL material{};
    material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
    material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);
}

/**
 * @brief Renderer で確保したリソースの解放処理を行います。
 *
 * 定数バッファ、レンダーターゲット、スワップチェーン、デバイスコンテキスト、
 * デバイスなどの DirectX リソースを解放します。
 */
void Renderer::Uninit()
{
    for (auto& bs : m_BlendState) {
		if (bs) {
			bs->Release();
			bs = nullptr;
		}
    }

//    m_BlendStateATC->Release();

    m_DepthStateEnable->Release();
//	m_DepthStateDisable->Release();

    m_WorldBuffer->Release();
    m_ViewBuffer->Release();
    m_ProjectionBuffer->Release();
    m_LightBuffer->Release();
    m_MaterialBuffer->Release();

    m_DeviceContext->ClearState();
    m_RenderTargetView->Release();
    m_SwapChain->Release();
    m_DeviceContext->Release();
    m_Device->Release();
}

/**
 * @brief 描画処理の開始を宣言します。
 *
 * レンダーターゲットビューおよび深度ステンシルビューのクリアを行います。
 */
void Renderer::Begin()
{
    float clearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/**
 * @brief 描画処理の終了を宣言し、スワップチェーンを Present します。
 */
void Renderer::End()
{
    m_SwapChain->Present(1, 0);
}

/**
 * @brief 深度テストの有効/無効を設定します。
 * @param Enable true なら深度テストを有効にし、false なら無効にします。
 */
void Renderer::SetDepthEnable(bool Enable)
{
    if (Enable)
        m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
    else
        m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);
}

/**
 * @brief Alpha-to-Coverage（ATC）のブレンドステートを設定します。
 * @param Enable true なら ATC を有効にし、false なら無効にします。
 */
void Renderer::SetATCEnable(bool Enable)
{
    float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    if (Enable)
        m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
    else
        m_DeviceContext->OMSetBlendState(m_BlendState[0], blendFactor, 0xffffffff);
}

/**
 * @brief 2D 描画用のワールド、ビュー、射影行列を設定します。
 *
 * ワールドおよびビュー行列は単位行列とし、射影行列は左上原点の直交射影に設定します。
 */
void Renderer::SetWorldViewProjection2D()
{
    Matrix4x4 world;
    world = Matrix4x4::Identity;       // 単位行列に設定
    world = world.Transpose();      // 転置して定数バッファ用に調整
    m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

    Matrix4x4 view;
    view = Matrix4x4::Identity;        // 単位行列に設定
    view = view.Transpose();        // 転置して定数バッファ用に調整
    m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

    Matrix4x4 projection;
    // 2D描画用の直交射影行列（左上原点）を作成
    projection = DirectX::XMMatrixOrthographicOffCenterLH(
        0.0f,
        static_cast<float>(Application::GetWidth()),  // ビューボリュームの最小 X
        static_cast<float>(Application::GetHeight()), // ビューボリュームの最小 Y
        0.0f,                                         // ビューボリュームの最大 Y
        0.0f,
        1.0f);
    projection = projection.Transpose();

    m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

/**
 * @brief ワールド行列の定数バッファを更新します。
 * @param WorldMatrix ワールド行列へのポインタ
 */
void Renderer::SetWorldMatrix(Matrix4x4* WorldMatrix)
{
    Matrix4x4 world;
    world = WorldMatrix->Transpose();  // 転置して定数バッファ用に調整
    m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

/**
 * @brief ビュー行列の定数バッファを更新します。
 * @param ViewMatrix ビュー行列へのポインタ
 */
void Renderer::SetViewMatrix(Matrix4x4* ViewMatrix)
{
    Matrix4x4 view;
    view = ViewMatrix->Transpose();  // 転置して定数バッファ用に調整
    m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

/**
 * @brief 射影行列の定数バッファを更新します。
 * @param ProjectionMatrix 射影行列へのポインタ
 */
void Renderer::SetProjectionMatrix(Matrix4x4* ProjectionMatrix)
{
    Matrix4x4 projection;
    projection = ProjectionMatrix->Transpose();  // 転置して定数バッファ用に調整
    m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

/**
 * @brief マテリアル情報の定数バッファを更新します。
 * @param Material マテリアル情報
 */
void Renderer::SetMaterial(MATERIAL Material)
{
    m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
}

/**
 * @brief ライト情報の定数バッファを更新します。
 * @param Light ライト情報
 */
void Renderer::SetLight(LIGHT Light)
{
    m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}
