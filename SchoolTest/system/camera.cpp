#include "CommonTypes.h"
#include "renderer.h"
#include "camera.h"
#include "Application.h"

void Camera::Init()
{
	m_Position = Vector3(0.0f, 10.0f, -100.0f);
	m_Target = Vector3(0.0f, 10.0f, 0.0f);
}

void Camera::Dispose()
{

}


void Camera::Update()
{

}

void Camera::Draw()
{
	// ビュー変換後列作成
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
	m_ViewMatrix = 
		DirectX::XMMatrixLookAtLH(
			m_Position, 
			m_Target, 
			up);				// 左手系にした　20230511 by suzuki.tomoki

	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
	//	m_ViewMatrix = m_ViewMatrix.CreateLookAt(m_Position, m_Target, up);					

	Renderer::SetViewMatrix(&m_ViewMatrix);

	//プロジェクション行列の生成
	constexpr float fieldOfView = DirectX::XMConvertToRadians(45.0f);    // 視野角
	
	float aspectRatio = static_cast<float>(Application::GetWidth()) / static_cast<float>(Application::GetHeight());	// アスペクト比	
	float nearPlane = 1.0f;       // ニアクリップ
	float farPlane = 1000.0f;      // ファークリップ

	//プロジェクション行列の生成
	m_ProjectionMatrix =
		DirectX::XMMatrixPerspectiveFovLH(
			fieldOfView, 
			aspectRatio, 
			nearPlane, 
			farPlane);	// 左手系にした　20230511 by suzuki.tomoki

	// DIRECTXTKのメソッドは右手系　20230511 by suzuki.tomoki
	// 右手系にすると３角形頂点が反時計回りになるので描画されなくなるので注意
	// このコードは確認テストのために残す
//	projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);

	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}