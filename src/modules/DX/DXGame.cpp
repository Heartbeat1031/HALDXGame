//
// Game.cpp
//


#include "DXGame.h"
#include <filesystem>

#include "RenderStates.h"

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

DXGame::DXGame() noexcept(false) {
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void DXGame::Initialize(HWND window, int width, int height) {
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    auto device = m_deviceResources->GetD3DDevice();
    m_TextureManager.Init(device);
    m_ModelManager.Init(device);

    // 务必先初始化所有渲染状态，以供下面的特效使用
    RenderStates::InitAll(device);
    //m_BasicEffect.InitAll(device);

    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}

void DXGame::Tick() {
    m_timer.Tick([&]() {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void DXGame::Update(DX::StepTimer const &timer) {
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    //BoneAnime
    float time = float(timer.GetTotalSeconds());

    m_world = XMMatrixRotationY(time * 0.1f);
}

void DXGame::Render() {
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0) {
        return;
    }

    Clear();

    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.

    //Model Render
    //m_model->Draw(context, *m_states, m_world, m_view, m_proj);
    //m_House.Draw(context, m_BasicEffect);
    // Show the new frame.
    m_deviceResources->Present();
    m_graphicsMemory->Commit();
}

// Helper method to clear the back buffers.
void DXGame::Clear() {
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}

// Message handlers
void DXGame::OnActivated() {
    // TODO: Game is becoming active window.
}

void DXGame::OnDeactivated() {
    // TODO: Game is becoming background window.
}

void DXGame::OnSuspending() {
    // TODO: Game is being power-suspended (or minimized).
}

void DXGame::OnResuming() {
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void DXGame::OnWindowMoved() {
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void DXGame::OnDisplayChange() {
    m_deviceResources->UpdateColorSpace();
}

void DXGame::OnWindowSizeChanged(int width, int height) {
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void DXGame::GetDefaultSize(int &width, int &height) const noexcept {
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void DXGame::CreateDeviceDependentResources() {
    // TODO: Initialize device dependent objects here (independent of window size).

    //Graphics
    auto device = m_deviceResources->GetD3DDevice();
    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);

    //BoneAnime
    m_states = std::make_unique<CommonStates>(device);
    //std::filesystem::path exePath = std::filesystem::current_path();
    //m_fxFactory = std::make_unique<EffectFactory>(device);
    // static_cast<DirectX::EffectFactory *>(m_fxFactory.get())->SetDirectory(L"assets/models");
    // m_model = Model::CreateFromSDKMESH(device, L"assets\\models\\tank.sdkmesh",
    //                                    *m_fxFactory,
    //                                    ModelLoader_CounterClockwise | ModelLoader_IncludeBones);

    DXModel* pModel = m_ModelManager.CreateFromFile("assets\\models\\house.obj");
    m_House.SetModel(pModel);

    m_world = Matrix::Identity;
}

// Allocate all memory resources that change on a window SizeChanged event.
void DXGame::CreateWindowSizeDependentResources() {
    //BoneAnime
    auto size = m_deviceResources->GetOutputSize();
    m_view = Matrix::CreateLookAt(Vector3(1000, 500, 0),
                                  Vector3(0, 150, 0), Vector3::UnitY);
    m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
                                                  float(size.right) / float(size.bottom), 0.1f, 10000.f);
}

void DXGame::OnDeviceLost() {
    // TODO: Add Direct3D resource cleanup here.
    m_graphicsMemory.reset();
    m_spriteBatch.reset();

    //Model Reset
    m_states.reset();
     m_fxFactory.reset();
    // m_model.reset();
}

void DXGame::OnDeviceRestored() {
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}
