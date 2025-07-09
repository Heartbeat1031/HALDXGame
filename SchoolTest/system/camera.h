#pragma once

#include	"CommonTypes.h"
#include	"CPolar3D.h"
#include	"renderer.h"
#include	"application.h"

class Camera {
protected:
	Vector3	m_Position = Vector3(0.0f, 0.0f, 0.0f);
	Vector3	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	Vector3	m_Scale = Vector3(1.0f, 1.0f, 1.0f);

	Vector3		m_Target{};
	Matrix4x4	m_ViewMatrix{};
	Matrix4x4   m_ProjectionMatrix;

public:
	void Init();
	void Dispose();
	void Update();
	void Draw();
	Matrix4x4 GetViewMatrix() { return m_ViewMatrix; }
	Matrix4x4 GetProjMatrix() { return m_ProjectionMatrix; }
};

// �t���[�J�����N���X
class FreeCamera : public Camera {
	float   m_elevation = 90.0f * DirectX::XM_PI / 180.0f;		// �p
	float	m_azimuth = 0.0f;								// ���ʊp	
	float   m_radius = 100.0f;								// ���a	

	Vector3 m_UP = Vector3(0.0f, 1.0f, 0.0f);

public:
	void SetElevation(float elevation) {
		m_elevation = elevation;
	}

	void SetAzimuth(float azimuth) {
		m_azimuth = azimuth;
	}

	void SetRadius(float radius) {
		m_radius = radius;
	}

	void Update() {
		// �p�ƕ��ʊp����J�����̈ʒu���v�Z
		CPolor3D polar(m_radius, m_elevation, m_azimuth);

		m_Position = polar.ToOyler();

		CPolor3D polarup(m_radius, (m_elevation + 90.0f * DirectX::XM_PI / 180.0f), m_azimuth);

		m_UP = polarup.ToOyler();
	}

	void Draw() {
		m_ViewMatrix =
			DirectX::XMMatrixLookAtLH(
				m_Position,
				m_Target,
				m_UP);				// ����n�ɂ����@20230511 by suzuki.tomoki

		// DIRECTXTK�̃��\�b�h�͉E��n�@20230511 by suzuki.tomoki
		// �E��n�ɂ���ƂR�p�`���_�������v���ɂȂ�̂ŕ`�悳��Ȃ��Ȃ�̂Œ���
		// ���̃R�[�h�͊m�F�e�X�g�̂��߂Ɏc��
		//	m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

		Renderer::SetViewMatrix(&m_ViewMatrix);

		//�v���W�F�N�V�����s��̐���
		constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // ����p

		float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// �A�X�y�N�g��	
		float nearPlane = 1.0f;       // �j�A�N���b�v
		float farPlane = 3000.0f;      // �t�@�[�N���b�v

		//�v���W�F�N�V�����s��̐���
		Matrix4x4 projectionMatrix;
		projectionMatrix =
			DirectX::XMMatrixPerspectiveFovLH(
				fieldOfView,
				aspectRatio,
				nearPlane,
				farPlane);	// ����n�ɂ����@20230511 by suzuki.tomoki

		// DIRECTXTK�̃��\�b�h�͉E��n�@20230511 by suzuki.tomoki
		// �E��n�ɂ���ƂR�p�`���_�������v���ɂȂ�̂ŕ`�悳��Ȃ��Ȃ�̂Œ���
		// ���̃R�[�h�͊m�F�e�X�g�̂��߂Ɏc��
	//	projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

		Renderer::SetProjectionMatrix(&projectionMatrix);

	}
};