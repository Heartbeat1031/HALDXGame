//
// Game.h
//

#pragma once
#include "pch.h"
#include "DeviceResources.h"
#include "StepTimer.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class DXGame : public DX::IDeviceNotify
{
public:

    DXGame() noexcept(false);

    virtual ~DXGame() = default;

    DXGame(DXGame&&) = default;
    DXGame& operator= (DXGame&&) = default;

    DXGame(DXGame const&) = delete;
    DXGame& operator= (DXGame const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();
    void Clear();
    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();
    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;
    // Rendering loop timer.
    DX::StepTimer                           m_timer;
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;

    std::unique_ptr<DirectX::CommonStates> m_states;

    RECT m_fullscreenRect;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_background;
    std::unique_ptr<DirectX::SpriteFont> m_font;
    DirectX::SimpleMath::Vector2 m_fontPos;

    using VertexType = DirectX::VertexPositionTexture;

    std::unique_ptr<DirectX::BasicEffect> m_effect;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    DirectX::SimpleMath::Matrix m_world;
    DirectX::SimpleMath::Matrix m_view;
    DirectX::SimpleMath::Matrix m_proj;

    std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
    std::unique_ptr<DirectX::Model> m_model;
};
