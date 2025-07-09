#include   "CommonTypes.h"
#include	<iostream>

#include	"CMeshRenderer.h"
#include	"CMaterial.h"
#include	"CSphereMesh.h"
#include    "CShader.h"

static CSphereMesh g_mesh;
static CMeshRenderer g_renderer;
static CMaterial g_material;
static CShader g_shader;

void SphereDrawerInit() 
{
	g_mesh.Init(1, Color(1, 1, 1, 1), 50, 50);
	g_renderer.Init(g_mesh);

	MATERIAL mtrl;
	// �}�e���A������
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 0, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shiness = 0;
	mtrl.TextureEnable = FALSE;

	g_material.Create(mtrl);

	// �V�F�[�_�[�̏�����
	g_shader.Create(
		"shader/unlitTextureVS.hlsl",				// ���_�V�F�[�_�[
		"shader/unlitTexturePS.hlsl");			// �s�N�Z���V�F�[�_�[

}

void SphereDrawerDraw(float radius,Color col,float ex, float ey, float ez)
{
	Matrix4x4 mtx = Matrix4x4::CreateScale(radius);

	mtx._41 = ex;
	mtx._42 = ey;
	mtx._43 = ez;

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_shader.SetGPU();

	g_material.SetGPU();
	g_renderer.Draw();
}

void SphereDrawerDraw(SRT rts ,Color col)
{
	Matrix4x4 mtx = rts.GetMatrix();

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_shader.SetGPU();

	g_material.SetGPU();
	g_renderer.Draw();
}

void SphereDrawerDraw(Matrix4x4 mtx, Color col)
{
	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_shader.SetGPU();

	g_material.SetGPU();
	g_renderer.Draw();
}
