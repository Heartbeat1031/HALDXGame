#include    "CommonTypes.h"
#include	<iostream>

#include	"CMeshRenderer.h"
#include	"CMaterial.h"
#include	"CBoxMesh.h"
#include    "CShader.h"

static CBoxMesh g_mesh;
static CMeshRenderer g_renderer;
static CMaterial g_material;
static CShader g_shader;

void BoxDrawerInit()
{
	g_mesh.Init(
		1,					// ��
		1,					// ����
		1,					// ���s
		Color(1,1,1,1));

	g_renderer.Init(g_mesh);

	// �V�F�[�_�[�̏�����
	g_shader.Create(
		"shader/unlitTextureVS.hlsl",				// ���_�V�F�[�_�[
		"shader/unlitTexturePS.hlsl");			// �s�N�Z���V�F�[�_�[

	MATERIAL mtrl;
	// �}�e���A������
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 0, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shiness = 0;
	mtrl.TextureEnable = FALSE;
	g_material.Create(mtrl);
}

void BoxDrawerDraw(float width, float height, float depth, 
	Color col, float posx, float posy, float posz)
{
	Matrix4x4 mtx = Matrix4x4::CreateScale(width,height,depth);

	mtx._41 = posx;
	mtx._42 = posy;
	mtx._43 = posz;

	g_shader.SetGPU();

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_material.SetGPU();
	g_renderer.Draw();
}

void BoxDrawerDraw(SRT rts,Color col)
{
	Matrix4x4 mtx = rts.GetMatrix();

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();
	g_material.SetGPU();

	g_shader.SetGPU();

	g_renderer.Draw();
}

void BoxDrawerDraw(Matrix4x4 mtx, Color col)
{
	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();
	g_material.SetGPU();

	g_shader.SetGPU();

	g_renderer.Draw();
}
