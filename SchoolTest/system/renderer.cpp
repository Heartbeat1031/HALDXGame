/**
 * @file Renderer.cpp
 * @brief DirectX ��p���������_�����O�������������� Renderer �N���X�̒�`�i�������j�B
 *
 * ���̃t�@�C���ł́ADirect3D �f�o�C�X�A�X���b�v�`�F�[���A�����_�[�^�[�Q�b�g�A�萔�o�b�t�@�Ȃǂ�
 * �������Ɖ���A�`��̊J�n�E�I�������A�e�탌���_�����O��ԁi�[�x�A�u�����h�A�}�g���b�N�X�Ȃǁj�̐ݒ���s���܂��B
 */

#include "renderer.h"
#include "Application.h"

//------------------------------------------------------------------------------
// �X�^�e�B�b�N�����o�ϐ��̏�����
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
// Renderer �N���X�̊e�֐��̎���
//------------------------------------------------------------------------------

/**
 * @brief Renderer �̏������������s���܂��B
 *
 * Direct3D �f�o�C�X�ƃX���b�v�`�F�[���̍쐬�A�����_�[�^�[�Q�b�g�r���[�A�f�v�X�X�e���V���r���[�A
 * �r���[�|�[�g�A���X�^���C�U�A�u�����h�X�e�[�g�A�[�x�X�e���V���X�e�[�g�A�T���v���[�X�e�[�g�A
 * �萔�o�b�t�@�̐����A�������C�g����у}�e���A���̐ݒ�Ȃǂ����{���܂��B
 */
void Renderer::Init()
{
    HRESULT hr = S_OK;

    // --- �f�o�C�X�A�X���b�v�`�F�[���쐬 ---
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
        D3D11_CREATE_DEVICE_BGRA_SUPPORT, // DIRECTWRITE �Ή�
        NULL,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &m_SwapChain,
        &m_Device,
        &m_FeatureLevel,
        &m_DeviceContext);

    // --- �����_�[�^�[�Q�b�g�r���[�̍쐬 ---
    ID3D11Texture2D* renderTarget{};
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
    m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
    renderTarget->Release();

    // --- �f�v�X�X�e���V���o�b�t�@�ƃr���[�̍쐬 ---
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

    // --- �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���r���[�̃o�C���h ---
    m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

    // --- �r���[�|�[�g�ݒ� ---
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(Application::GetWidth());
    viewport.Height = static_cast<FLOAT>(Application::GetHeight());
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    m_DeviceContext->RSSetViewports(1, &viewport);

    // --- ���X�^���C�U�X�e�[�g�ݒ� ---
    D3D11_RASTERIZER_DESC rasterizerDesc{};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    // rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;  // ���C���[�t���[���\���̏ꍇ
       rasterizerDesc.CullMode = D3D11_CULL_BACK;
    // rasterizerDesc.CullMode = D3D11_CULL_NONE;       // �J�����O�����̏ꍇ
    // rasterizerDesc.CullMode = D3D11_CULL_FRONT;      // �t�����g�J�����O�̏ꍇ
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.MultisampleEnable = FALSE;

    ID3D11RasterizerState* rs;
    m_Device->CreateRasterizerState(&rasterizerDesc, &rs);
    m_DeviceContext->RSSetState(rs);
    rs->Release();

    // --- �u�����h�X�e�[�g�̐��� ---
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

    // �A���t�@ �u�����h�p�u�����h�X�e�[�g����
    // BlendDesc.AlphaToCoverageEnable = TRUE; // �K�v�ɉ�����
    BlendDesc.RenderTarget[0].BlendEnable = TRUE;
    m_Device->CreateBlendState(&BlendDesc, &m_BlendState[1]);
    m_Device->CreateBlendState(&BlendDesc, &m_BlendStateATC);

    // ���Z�����p�u�����h�X�e�[�g����
    BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    m_Device->CreateBlendState(&BlendDesc, &m_BlendState[2]);

    // ���Z�����p�u�����h�X�e�[�g����
    BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
    m_Device->CreateBlendState(&BlendDesc, &m_BlendState[3]);

    // �����u�����h�X�e�[�g�Ƃ��ăA���t�@ �u�����h��ݒ�
    SetBlendState(BS_ALPHABLEND);

    // --- �[�x�X�e���V���X�e�[�g�̐ݒ� ---
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    // depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; // �K�v�ɉ�����
    depthStencilDesc.StencilEnable = FALSE;

    m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable); // �[�x�e�X�g�L��
    depthStencilDesc.DepthEnable = FALSE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    m_Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable); // �[�x�e�X�g����

    m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);

    // --- �T���v���[�X�e�[�g�ݒ� ---
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

    // --- �萔�o�b�t�@���� ---
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

    // --- ���C�g������ ---
    LIGHT light{};
    light.Enable = true;
    light.Direction = Vector4(0.5f, -1.0f, 0.8f, 0.0f);
    light.Direction.Normalize();
    light.Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
    light.Diffuse = Color(1.5f, 1.5f, 1.5f, 1.0f);
    SetLight(light);

    // --- �}�e���A�������� ---
    MATERIAL material{};
    material.Diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
    material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);
}

/**
 * @brief Renderer �Ŋm�ۂ������\�[�X�̉���������s���܂��B
 *
 * �萔�o�b�t�@�A�����_�[�^�[�Q�b�g�A�X���b�v�`�F�[���A�f�o�C�X�R���e�L�X�g�A
 * �f�o�C�X�Ȃǂ� DirectX ���\�[�X��������܂��B
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
 * @brief �`�揈���̊J�n��錾���܂��B
 *
 * �����_�[�^�[�Q�b�g�r���[����ѐ[�x�X�e���V���r���[�̃N���A���s���܂��B
 */
void Renderer::Begin()
{
    float clearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/**
 * @brief �`�揈���̏I����錾���A�X���b�v�`�F�[���� Present ���܂��B
 */
void Renderer::End()
{
    m_SwapChain->Present(1, 0);
}

/**
 * @brief �[�x�e�X�g�̗L��/������ݒ肵�܂��B
 * @param Enable true �Ȃ�[�x�e�X�g��L���ɂ��Afalse �Ȃ疳���ɂ��܂��B
 */
void Renderer::SetDepthEnable(bool Enable)
{
    if (Enable)
        m_DeviceContext->OMSetDepthStencilState(m_DepthStateEnable, NULL);
    else
        m_DeviceContext->OMSetDepthStencilState(m_DepthStateDisable, NULL);
}

/**
 * @brief Alpha-to-Coverage�iATC�j�̃u�����h�X�e�[�g��ݒ肵�܂��B
 * @param Enable true �Ȃ� ATC ��L���ɂ��Afalse �Ȃ疳���ɂ��܂��B
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
 * @brief 2D �`��p�̃��[���h�A�r���[�A�ˉe�s���ݒ肵�܂��B
 *
 * ���[���h����уr���[�s��͒P�ʍs��Ƃ��A�ˉe�s��͍��㌴�_�̒����ˉe�ɐݒ肵�܂��B
 */
void Renderer::SetWorldViewProjection2D()
{
    Matrix4x4 world;
    world = Matrix4x4::Identity;       // �P�ʍs��ɐݒ�
    world = world.Transpose();      // �]�u���Ē萔�o�b�t�@�p�ɒ���
    m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

    Matrix4x4 view;
    view = Matrix4x4::Identity;        // �P�ʍs��ɐݒ�
    view = view.Transpose();        // �]�u���Ē萔�o�b�t�@�p�ɒ���
    m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

    Matrix4x4 projection;
    // 2D�`��p�̒����ˉe�s��i���㌴�_�j���쐬
    projection = DirectX::XMMatrixOrthographicOffCenterLH(
        0.0f,
        static_cast<float>(Application::GetWidth()),  // �r���[�{�����[���̍ŏ� X
        static_cast<float>(Application::GetHeight()), // �r���[�{�����[���̍ŏ� Y
        0.0f,                                         // �r���[�{�����[���̍ő� Y
        0.0f,
        1.0f);
    projection = projection.Transpose();

    m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

/**
 * @brief ���[���h�s��̒萔�o�b�t�@���X�V���܂��B
 * @param WorldMatrix ���[���h�s��ւ̃|�C���^
 */
void Renderer::SetWorldMatrix(Matrix4x4* WorldMatrix)
{
    Matrix4x4 world;
    world = WorldMatrix->Transpose();  // �]�u���Ē萔�o�b�t�@�p�ɒ���
    m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

/**
 * @brief �r���[�s��̒萔�o�b�t�@���X�V���܂��B
 * @param ViewMatrix �r���[�s��ւ̃|�C���^
 */
void Renderer::SetViewMatrix(Matrix4x4* ViewMatrix)
{
    Matrix4x4 view;
    view = ViewMatrix->Transpose();  // �]�u���Ē萔�o�b�t�@�p�ɒ���
    m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

/**
 * @brief �ˉe�s��̒萔�o�b�t�@���X�V���܂��B
 * @param ProjectionMatrix �ˉe�s��ւ̃|�C���^
 */
void Renderer::SetProjectionMatrix(Matrix4x4* ProjectionMatrix)
{
    Matrix4x4 projection;
    projection = ProjectionMatrix->Transpose();  // �]�u���Ē萔�o�b�t�@�p�ɒ���
    m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}

/**
 * @brief �}�e���A�����̒萔�o�b�t�@���X�V���܂��B
 * @param Material �}�e���A�����
 */
void Renderer::SetMaterial(MATERIAL Material)
{
    m_DeviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &Material, 0, 0);
}

/**
 * @brief ���C�g���̒萔�o�b�t�@���X�V���܂��B
 * @param Light ���C�g���
 */
void Renderer::SetLight(LIGHT Light)
{
    m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}
