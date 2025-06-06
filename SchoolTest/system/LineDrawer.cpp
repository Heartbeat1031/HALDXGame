#include	<iostream>

#include	"CommonTypes.h"
#include	"CMeshRenderer.h"
#include	"CMaterial.h"
#include	"CLineMesh.h"
#include    "CShader.h"

static CLineMesh g_mesh;
static CMeshRenderer g_renderer;
static CMaterial g_material;

static CShader g_shader;

void LineDrawerInit() 
{
	g_mesh.Init(Vector3(0, 0, 0), Vector3(0, 0, 1), 1);
	g_renderer.Init(g_mesh);

	MATERIAL mtrl;
	// �}�e���A������
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shiness = 0;
	mtrl.TextureEnable = FALSE;

	g_material.Create(mtrl);

	// �V�F�[�_�[�̏�����
	g_shader.Create(
		"shader/unlitTextureVS.hlsl",			// ���_�V�F�[�_�[
		"shader/unlitTexturePS.hlsl",			// �s�N�Z���V�F�[�_�[
		"shader/GeometryShader.hlsl"			// �W�I���g���V�F�[�_
		);			

}

void LineDrawerDraw(
	float length,
	Vector3 start,
	Vector3 direction, 
	Color col)
{
	g_mesh.Clear();
	g_mesh.Init(start, direction, length);
	Matrix4x4 mtx = Matrix4x4::Identity;

	// ���_�o�b�t�@���X�V
	g_renderer.Modify(g_mesh.GetVertices());

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_shader.SetGPU();
	g_material.SetGPU();
	g_renderer.Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
}
