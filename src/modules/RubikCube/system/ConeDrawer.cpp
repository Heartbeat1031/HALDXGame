
#include	<iostream>
#include	"CommonTypes.h"

#include	"CMeshRenderer.h"
#include	"CMaterial.h"
#include	"CConeMesh.h"
#include	"CShader.h"

static CConeMesh g_mesh;
static CMeshRenderer g_renderer;
static CMaterial g_material;
static CShader g_shader;

void ConeDrawerInit(bool topdownflag)
{
	g_mesh.Init(
		50,						// ������
		1,						// ���a
		1,						// ����
		Color(1, 1, 1, 1),		// ���_�J���[
		topdownflag);			// �t���O

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

void ConeDrawerDraw(float radius, 
	float height, 
	Color col, 
	float posx, float posy, float posz)
{
	Matrix4x4 mtx = Matrix4x4::CreateScale(radius, height, radius);
	//	Matrix mtx = Matrix::Identity;

	mtx._41 = posx;
	mtx._42 = posy;
	mtx._43 = posz;

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_material.SetGPU();
	g_renderer.Draw();
}

void ConeDrawerDraw(
	float radius, 
	float height, 
	Color col, 
	float posx, float posy, float posz, 
	Matrix4x4 rmtx)
{
	Matrix4x4 mtx = Matrix4x4::CreateScale(radius, height, radius) * rmtx;

	mtx._41 = posx;
	mtx._42 = posy;
	mtx._43 = posz;

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_material.SetGPU();
	g_renderer.Draw();
}

void ConeDrawerDraw(
	SRT	rts,
	Color col)
{
	Matrix4x4 mtx = rts.GetMatrix();

	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_material.SetGPU();
	g_renderer.Draw();
}

void ConeDrawerDraw(
	Matrix4x4 mtx,
	Color col)
{
	Renderer::SetWorldMatrix(&mtx);
	g_material.SetDiffuse(col);
	g_material.Update();

	g_shader.SetGPU();
	g_material.SetGPU();
	g_renderer.Draw();
}
