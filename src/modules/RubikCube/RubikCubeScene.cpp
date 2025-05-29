#include    <memory>
#include	"RubikCubeScene.h"

// 角度から姿勢行列をつくる
void RubikCubeScene::debugRubikCubeRotation()
{

	ImGui::Begin("DebugRubikCube Rotation");

	ImGui::SliderFloat("X Rotation", &m_Rotation.x, 0.0f, PI);
	ImGui::SliderFloat("Y Rotation", &m_Rotation.y, 0.0f, PI);
	ImGui::SliderFloat("Z Rotation", &m_Rotation.z, 0.0f, PI);

	// 回転角度から回転行列を作成
	Matrix4x4 rotmtxX = Matrix4x4::CreateRotationX(m_Rotation.x);
	Matrix4x4 rotmtxY = Matrix4x4::CreateRotationY(m_Rotation.y);
	Matrix4x4 rotmtxZ = Matrix4x4::CreateRotationZ(m_Rotation.z);

	// 合成
	m_RotationMtx = rotmtxX * rotmtxY * rotmtxZ;

	// カメラの位置を極座標からデカルト座標に変換
	ImGui::End();
}

RubikCubeScene::RubikCubeScene()
{

}

void RubikCubeScene::update(uint64_t deltatime)
{

}

void RubikCubeScene::draw(uint64_t deltatime)
{
	m_camera.Draw();

	// 3軸カラー
	Color axiscol[3] = {
		Color(1, 0, 0, 1), 
		Color(0, 1, 0, 1),
		Color(0, 1, 1, 1)
	};

	// 3本のローカル軸を描画
	for (int cnt=0;cnt<m_segments.size();cnt++)
	{
		m_segments[cnt]->Draw(m_RotationMtx, axiscol[cnt]);
	}

	for (int x = 0; x < CUBELAYERNUM; x++)
	{
		for (int y = 0; y < CUBELAYERNUM; y++)
		{
			for (int z = 0; z < CUBELAYERNUM; z++)
			{
				float posx;
				float posy;
				float posz;

				posx = -(CUBELAYERNUM - 1) * 0.5f * CUBE_SIZE + x * CUBE_SIZE;
				posy = -(CUBELAYERNUM - 1) * 0.5f * CUBE_SIZE + y * CUBE_SIZE;
				posz = -(CUBELAYERNUM - 1) * 0.5f * CUBE_SIZE + z * CUBE_SIZE;
				Matrix4x4 transmtx = Matrix4x4::CreateTranslation(posx, posy, posz);
				Matrix4x4 worldmtx = transmtx * m_RotationMtx;
				// キューブを描画
				m_shapecube->Draw(worldmtx, Color(1, 1, 1, 0.2f));
			}
		}
	}

}


void RubikCubeScene::init()
{
	// カメラ(3D)の初期化
	m_camera.Init();

	// ボックスの初期化
	m_shapecube = std::make_unique<Box>(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);

	// ローカル軸表示用線分の初期化
	m_segments[0] = std::make_unique<Segment>(Vector3(0, 0, 0), Vector3(CUBE_SIZE * 10, 0,				0));
	m_segments[1] = std::make_unique<Segment>(Vector3(0, 0, 0), Vector3(0,				CUBE_SIZE * 10, 0));
	m_segments[2] = std::make_unique<Segment>(Vector3(0, 0, 0), Vector3(0,				0,				CUBE_SIZE * 10));

	DebugUI::RedistDebugFunction([this]() {
		debugRubikCubeRotation();
		});

}


void RubikCubeScene::dispose()
{

}