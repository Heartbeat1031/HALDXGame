//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

#include <filesystem>

extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
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

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    ////Mesh render
    //auto time = static_cast<float>(timer.GetTotalSeconds());

    //m_world = Matrix::CreateRotationY(time);


    ////Model Render
    //float time = float(timer.GetTotalSeconds());

    //m_world = Matrix::CreateRotationZ(cosf(time) * 2.f);


    //BoneAnime
    float time = float(timer.GetTotalSeconds());

    m_world = XMMatrixRotationY(time * 0.1f);
}
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    
    ////Mesh Renderer
    //m_shape->Draw(m_world, m_view, m_proj, Colors::White, m_texture.Get());

    //Model Render
    m_model->Draw(context, *m_states, m_world, m_view, m_proj);


    // Show the new frame.
    m_deviceResources->Present();
    m_graphicsMemory->Commit();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
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
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{

    // TODO: Initialize device dependent objects here (independent of window size).

    //Graphics
    auto device = m_deviceResources->GetD3DDevice();
    m_graphicsMemory = std::make_unique<GraphicsMemory>(device);


    ////Mesh Rendering
    //auto context = m_deviceResources->GetD3DDeviceContext();
    //m_shape = GeometricPrimitive::CreateSphere(context);

    //m_world = Matrix::Identity;

    //DX::ThrowIfFailed(
    //    CreateWICTextureFromFile(device, L"earth.bmp", nullptr,
    //        m_texture.ReleaseAndGetAddressOf()));


    ////Model renderer
    //m_states = std::make_unique<CommonStates>(device);

    //m_fxFactory = std::make_unique<EffectFactory>(device);

    //m_model = Model::CreateFromCMO(device, L"cup.cmo", *m_fxFactory);

    //m_world = Matrix::Identity;
    // Make sure you called CreateWICTextureFromFile before this point!


    //BoneAnime
    m_states = std::make_unique<CommonStates>(device);
    //std::filesystem::path exePath = std::filesystem::current_path();
    m_fxFactory = std::make_unique<EffectFactory>(device);
    static_cast<DirectX::EffectFactory*>(m_fxFactory.get())->SetDirectory(L"assets/models");
    m_model = Model::CreateFromSDKMESH(device, L"assets\\models\\tank.sdkmesh",
        *m_fxFactory,
        ModelLoader_CounterClockwise | ModelLoader_IncludeBones);

    m_world = Matrix::Identity;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
    ////Mesh Render
    //auto size = m_deviceResources->GetOutputSize();
    //m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
    //    Vector3::Zero, Vector3::UnitY);
    //m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
    //    float(size.right) / float(size.bottom), 0.1f, 10.f);


    ////Model Renderer
    //auto size = m_deviceResources->GetOutputSize();
    //m_view = Matrix::CreateLookAt(Vector3(2.f, 2.f, 2.f),
    //    Vector3::Zero, Vector3::UnitY);
    //m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
    //    float(size.right) / float(size.bottom), 0.1f, 10.f);


    //BoneAnime
    auto size = m_deviceResources->GetOutputSize();
    m_view = Matrix::CreateLookAt(Vector3(1000, 500, 0),
        Vector3(0, 150, 0), Vector3::UnitY);
    m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
        float(size.right) / float(size.bottom), 0.1f, 10000.f);
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_graphicsMemory.reset();
    m_texture.Reset();
    m_spriteBatch.reset();

    //3DModel Reset
    m_shape.reset();
    m_texture.Reset();

    //Model Reset
    m_states.reset();
    m_fxFactory.reset();
    m_model.reset();

}


void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
