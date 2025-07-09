#include	<iostream>
#include    <memory>
#include    <array>
#include    "animationsystem.h"
#include    "explosion.h"
#include    "system/renderer.h"
#include    "system/cshader.h"	
#include    "system/CTexture.h"
#include    "system/CVertexBuffer.h"
#include    "system/CIndexBuffer.h"
#include    "system/CMaterial.h"

// �����\���̔z��
static Explosion g_explosion[EXPLOSIONMAX];				// �����\����

// �����p�V�F�[�_
static CShader g_explosionshader;		// �����p�V�F�[�_
static CTexture g_explosiontexture;		// �����p�e�N�X�`��	

// �����̃e�N�X�`�����W��`
static UVQuad g_explosionpicture[16] = {
	{	Vector2(0.0f / 8.0f,		1.0f / 4.0f),		Vector2(1.0f / 8.0f,	1.0f / 4.0f),	Vector2(0.0f / 8.0f,		2.0f / 4.0f),	Vector2(1.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(1.0f / 8.0f,		1.0f / 4.0f),		Vector2(2.0f / 8.0f,	1.0f / 4.0f),	Vector2(1.0f / 8.0f,		2.0f / 4.0f),	Vector2(2.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(2.0f / 8.0f,		1.0f / 4.0f),		Vector2(3.0f / 8.0f,	1.0f / 4.0f),	Vector2(2.0f / 8.0f,		2.0f / 4.0f),	Vector2(3.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(3.0f / 8.0f,		1.0f / 4.0f),		Vector2(4.0f / 8.0f,	1.0f / 4.0f),	Vector2(3.0f / 8.0f,		2.0f / 4.0f),	Vector2(4.0f / 8.0f,	2.0f / 4.0f) },

	{	Vector2(4.0f / 8.0f,		1.0f / 4.0f),		Vector2(5.0f / 8.0f,	1.0f / 4.0f),	Vector2(4.0f / 8.0f,		2.0f / 4.0f),	Vector2(5.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(5.0f / 8.0f,		1.0f / 4.0f),		Vector2(6.0f / 8.0f,	1.0f / 4.0f),	Vector2(5.0f / 8.0f,		2.0f / 4.0f),	Vector2(6.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(6.0f / 8.0f,		1.0f / 4.0f),		Vector2(7.0f / 8.0f,	1.0f / 4.0f),	Vector2(6.0f / 8.0f,		2.0f / 4.0f),	Vector2(7.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(7.0f / 8.0f,		1.0f / 4.0f),		Vector2(8.0f / 8.0f,	1.0f / 4.0f),	Vector2(7.0f / 8.0f,		2.0f / 4.0f),	Vector2(8.0f / 8.0f,	2.0f / 4.0f) },

	{	Vector2(0.0f / 8.0f,		2.0f / 4.0f),		Vector2(1.0f / 8.0f,	2.0f / 4.0f),	Vector2(0.0f / 8.0f,		3.0f / 4.0f),	Vector2(1.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(1.0f / 8.0f,		2.0f / 4.0f),		Vector2(2.0f / 8.0f,	2.0f / 4.0f),	Vector2(1.0f / 8.0f,		3.0f / 4.0f),	Vector2(2.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(2.0f / 8.0f,		2.0f / 4.0f),		Vector2(3.0f / 8.0f,	2.0f / 4.0f),	Vector2(2.0f / 8.0f,		3.0f / 4.0f),	Vector2(3.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(3.0f / 8.0f,		2.0f / 4.0f),		Vector2(4.0f / 8.0f,	2.0f / 4.0f),	Vector2(3.0f / 8.0f,		3.0f / 4.0f),	Vector2(4.0f / 8.0f,	3.0f / 4.0f) },

	{	Vector2(4.0f / 8.0f,		2.0f / 4.0f),		Vector2(5.0f / 8.0f,	2.0f / 4.0f),	Vector2(4.0f / 8.0f,		3.0f / 4.0f),	Vector2(5.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(5.0f / 8.0f,		2.0f / 4.0f),		Vector2(6.0f / 8.0f,	2.0f / 4.0f),	Vector2(5.0f / 8.0f,		3.0f / 4.0f),	Vector2(6.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(6.0f / 8.0f,		2.0f / 4.0f),		Vector2(7.0f / 8.0f,	2.0f / 4.0f),	Vector2(6.0f / 8.0f,		3.0f / 4.0f),	Vector2(7.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(7.0f / 8.0f,		2.0f / 4.0f),		Vector2(8.0f / 8.0f,	2.0f / 4.0f),	Vector2(7.0f / 8.0f,		3.0f / 4.0f),	Vector2(8.0f / 8.0f,	3.0f / 4.0f) }

};

// �`��p
static CMaterial g_material{};
static CIndexBuffer g_indexbuffer;
static CVertexBuffer<VERTEX_3D> g_vertexbuffer;

static std::vector<VERTEX_3D> g_vertices = {};

// �����e�[�u��
static AnimationTbl	g_explosionanim[]{
	{ 0,2 },
	{ 1,2 },
	{ 2,2 },
	{ 3,2 },
	{ 4,2 },
	{ 5,2 },
	{ 6,2 },
	{ 7,2 },
	{ 8,2 },
	{ 9,2 },
	{ 10,2 },
	{ 11,2 },
	{ 12,2 },
	{ 13,2 },
	{ 14,2 },
	{ 15,2 },
	{ -1,-1 }
};

// �A�j���[�V�����e�[�u���S��
static AnimationTbl* g_AnimTableAll[4]{
	g_explosionanim
};

// �e�\���̂�����������
Explosion InitializeExplosionStruct()
{
	Explosion obj;

	obj.sts = -1;
	obj.position = Vector3(0, 0, 0);
	obj.rotation = Vector3(0, 0, 0);
	obj.scale = Vector3(1, 1, 1);
	obj.currentAnimIdx = 0;
	obj.currentAnimTble = g_explosionanim;
	obj.lifetime = EXPLOSION_LIFETIME;

	return obj;
}

void initexplosion()
{

	std::array<Vector2,4> uvs = {
		Vector2(0.0f,			0.0f),
		Vector2(1.0f / 3.0f,	0.0f),
		Vector2(0.0f,			1.0f / 4.0f),
		Vector2(1.0f / 3.0f,	1.0f / 4.0f)
	};

	// �e��S�Ė��g�p�ɂ���
	for (int i = 0; i < EXPLOSIONMAX; i++) {
		Explosion& expl = g_explosion[i];

		// ����������
		expl = InitializeExplosionStruct();

		// ���g�p�ɂ���
		expl.sts = -1;
	}

	// �}�e���A������
	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shiness = 0;
	mtrl.TextureEnable = TRUE;

	g_material.Create(mtrl);

	g_explosionshader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");

	bool sts = g_explosiontexture.Load("assets/texture/explosion.png");
	assert(sts == true);

	g_vertices.clear();
	g_vertices.resize(4);

	float width = 1.0f;
	float height = 1.0f;

	g_vertices[0].Position = Vector3(-width / 2.0f, -height / 2.0f, 0);
	g_vertices[1].Position = Vector3(width / 2.0f, -height / 2.0f, 0);
	g_vertices[2].Position = Vector3(-width / 2.0f, height / 2.0f, 0);
	g_vertices[3].Position = Vector3(width / 2.0f, height / 2.0f, 0);

	for (int i = 0; i < 4; ++i) {
		g_vertices[i].Diffuse = Color(1, 1, 1, 1);
		g_vertices[i].TexCoord = g_explosionpicture[0].tex[i];
	}
	g_vertexbuffer.Create(g_vertices);

	std::vector<unsigned int> indices = { 0, 1, 2, 3 };
	g_indexbuffer.Create(indices);

}

// �����ɍ��킹���p�x���Z�b�g
void SetRotation(int direction, Explosion& expl)
{

	Vector3 rotation[4] =
	{
		{0,0, -90.0f * PI / 180.0f},			// ��
		{0,0,  90.0f * PI / 180.0f},			// ��
		{0,0, 180.0f * PI / 180.0f},			// ��
		{0,0,0}								    // �E
	};

	// �p�����Z�b�g
	expl.rotation = rotation[direction];
}

void drawexplosion()
{
	// �����_�����O�̏���
	g_explosiontexture.SetGPU();
	g_explosionshader.SetGPU();
	g_material.SetGPU();

	Renderer::SetWorldViewProjection2D();

	// �e�`��
	for (int i = 0; i < EXPLOSIONMAX; i++) {

		Explosion& expl = g_explosion[i];

		// �g�p���̒e�̂ݕ`��
		if (expl.sts >= 0) {
			// �w�肵���s�N�`���[�e�[�u���ƃA�j���[�V�����e�[�u���̃C���f�b�N�X�l���g�p���ĉ摜��\������
			UVQuad nowuv;
			nowuv = GetPictureUV(
				expl.currentAnimIdx,		// ���ݎg�p���Ă���A�j���[�V�����e�[�u���̃C���f�b�N�X�l
				expl.AnimCounter,			// ���݂̕\���R�}��
				expl.currentAnimTble,		// �g�p���Ă���A�j���[�V�����e�[�u��
				g_explosionpicture);		// �g�p���Ă���s�N�`���[�e�[�u��

			g_vertices[0].TexCoord = nowuv.tex[0];
			g_vertices[1].TexCoord = nowuv.tex[1];
			g_vertices[2].TexCoord = nowuv.tex[2];
			g_vertices[3].TexCoord = nowuv.tex[3];

			g_vertexbuffer.Modify(g_vertices);

			SRT srt;
			srt.scale = Vector3(EXPLOSION_WIDTH,EXPLOSION_HEIGHT,1);
			srt.rot = Vector3(0, 0, 0); // ��]�͕s�v
			srt.pos = expl.position;

			Matrix4x4 worldmtx = srt.GetMatrix();
			Renderer::SetWorldMatrix(&worldmtx);

			ID3D11DeviceContext* devicecontext = Renderer::GetDeviceContext();
			devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			g_vertexbuffer.SetGPU();
			g_indexbuffer.SetGPU();

			devicecontext->DrawIndexed(
				4, // �C���f�b�N�X��
				0, // �J�n�C���f�b�N�X
				0  // ���_�o�b�t�@�̃I�t�Z�b�g
			);
		}
	}
}

void updateexplosion()
{

	// �g�p���̒e�����X�V����
	for (int i = 0; i < EXPLOSIONMAX; i++) {

		Explosion& expl = g_explosion[i];
		if (expl.sts > 0) {

			// �������Ԃ̍X�V
			expl.lifetime--;
			if (expl.lifetime <= 0) {
				expl.sts = -1;
			}
		}
	}
}

void disposeexplosion()
{
}

// �v���C���e�@���ˏ���
void startexplosion(Vector3 pos, Vector3 scale, int dir)
{

	static DWORD lastFireTime = 0;

	// ���ݎ������擾����
	DWORD nowtime = timeGetTime();

	// ���g�p�̒e���擾
	for (int i = 0; i < EXPLOSIONMAX; i++) {

		Explosion& expl = g_explosion[i];
		if (expl.sts < 0) {
			// ����������
			expl = InitializeExplosionStruct();

			// �g�p���ɂ���
			expl.sts = 1;

			// �p�x���Z�b�g
			SetRotation(dir, expl);

			// ���ˈʒu���Z�b�g
			expl.position = pos;

			// �ŏI���ˎ�����ۑ�
			lastFireTime = nowtime;

			break;
		}
	}
}

void resetexplosion() 
{

	// �e��S�Ė��g�p�ɂ���
	for (int i = 0; i < EXPLOSIONMAX; i++) {
		Explosion& expl = g_explosion[i];

		// ����������
		expl = InitializeExplosionStruct();

		// ���g�p�ɂ���
		expl.sts = -1;
	}
}