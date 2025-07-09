#include <memory>
#include <numbers>
#include <array>
#include "camera.h"
#include "Scene.h"
#include "renderer.h"
#include "DebugUI.h"
#include "CDirectInput.h"
#include "DirectWrite.h"
#include "SphereDrawer.h"
#include "BoxDrawer.h"
#include "CylinderDrawer.h"
#include "ConeDrawer.h"
#include "LineDrawer.h"
#include "CapsuleDrawer.h"

#include "../MyGame.h"

// DirectWrite�N���X�̃C���X�^���X
static std::unique_ptr<DirectWrite> g_DirectWrite;

// �J����
static Camera g_Camera;

void SceneInit() 
{
	Renderer::Init();

	DebugUI::Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

	CDirectInput::GetInstance().Init(Application::GetHInstance(),Application::GetWindow(),Application::GetWidth(),Application::GetWidth());

	g_DirectWrite = std::make_unique<DirectWrite>();
	g_DirectWrite->Init(Renderer::GetSwapChain());

	//	�J����������
	g_Camera.Init();	

	// �f�o�b�O�p�̂���
	SphereDrawerInit();
	BoxDrawerInit();
	CylinderDrawerInit();
	ConeDrawerInit(true);
	LineDrawerInit();
	CapsuleDrawerInit();

	// �Q�[���̏�����
	GameInit();

}

void SceneUpdate(uint64_t delta) 
{
	// �f�o�C�X�̏�Ԃ��擾
	CDirectInput::GetInstance().GetKeyBuffer();
	CDirectInput::GetInstance().GetMouseState();

	// �Q�[���̍X�V
	GameUpdate(delta);

}

void SceneRender(uint64_t delta) 
{
	Renderer::Begin();

	g_Camera.Draw();

	// �Q�[���̕`��
	GameDraw(delta);

	DebugUI::Render();

	Renderer::End();
}

void SceneDispose() 
{
	// �Q�[���̏I������
	GameDispose();

	DebugUI::DisposeUI();
	Renderer::Uninit();

}	