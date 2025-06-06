#include	"SphereDrawer.h"
#include	"CylinderDrawer.h"
#include    "CCylinderMesh.h"
#include	"CSphereMesh.h"
#include	"CMeshRenderer.h"
#include	"CapsuleDrawer.h"
#include	"CMaterial.h"
#include	"CShader.h"

static CCylinderMesh g_meshcylinder;
static CSphereMesh g_meshsphere;

static CMeshRenderer g_renderercylinder;
static CMeshRenderer g_renderersphere;

static CMaterial g_material;
static CShader g_shader;

void CapsuleDrawerInit() 
{
	g_meshsphere.Init(1, Color(1, 1, 1, 1), 100, 100);
	g_renderersphere.Init(g_meshsphere);

	g_meshcylinder.Init(50,		// ������
		1,						// ���a
		1,						// ����
		Color(1, 1, 1, 1));

	g_renderercylinder.Init(g_meshcylinder);

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
		"shader/unlitTextureVS.hlsl",			// ���_�V�F�[�_�[
		"shader/unlitTexturePS.hlsl");			// �s�N�Z���V�F�[�_�[

}
