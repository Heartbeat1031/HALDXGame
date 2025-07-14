#include "CSprite.h"
#include "renderer.h"

/// @brief �X�v���C�g�p�̋��ʃV�F�[�_
CShader CSprite::m_Shader{};

/**
 * @brief �}�e���A��������������
 *
 * �f�t�H���g�̃��C�e�B���O�Ȃ����}�e���A����ݒ肵�A�e�N�X�`����L���ɂ��܂��B
 */
void CSprite::CreateMtrl() {
	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shiness = 0;
	mtrl.TextureEnable = TRUE;

	m_Material.Create(mtrl);
}

/**
 * @brief �}�e���A��������������
 *
 * �f�t�H���g�̃��C�e�B���O�Ȃ����}�e���A����ݒ肵�A�e�N�X�`����L���ɂ��܂��B
 */
void CSprite::ModifyMtrl(MATERIAL mtrl) {
	m_Material.SetMaterial(mtrl);
}
/**
 * @brief �X�v���C�g�p�̃V�F�[�_��ǂݍ��݁E��������
 *
 * ���_�V�F�[�_�ƃs�N�Z���V�F�[�_�����[�h���ăo�C���h�\�ȏ�Ԃɂ��܂��B
 */
void CSprite::CreateShader() {
	m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
}

/**
 * @brief �e�N�X�`����ǂݍ���
 *
 * @param texfilename �ǂݍ��ރe�N�X�`���t�@�C���̃p�X
 *
 * @note �ǂݍ��݂Ɏ��s�����ꍇ�� assert �ɂ��N���b�V�����܂��B
 */
void CSprite::CreateTexture(const std::string& texfilename) {
	bool sts = m_Texture.Load(texfilename);
	assert(sts == true);
}

/**
 * @brief �X�v���C�g�̃R���X�g���N�^
 *
 * �w�肳�ꂽ���E�����E�e�N�X�`���EUV�ŃX�v���C�g�����������܂��B
 *
 * @param width �X�v���C�g�̕��i�s�N�Z���j
 * @param height �X�v���C�g�̍����i�s�N�Z���j
 * @param texfilename �g�p����e�N�X�`���̃t�@�C����
 * @param uv UV���W�i����A�E��A�����A�E���j�̔z��
 */

CSprite::CSprite(int width, int height, std::string texfilename, std::array<Vector2, 4> uv)
{
	m_Width = width;
	m_Height = height;

	CreateMtrl();
	CreateShader();
	CreateTexture(texfilename);

	m_Vertices.clear();
	m_Vertices.resize(4);

	m_Vertices[0].Position = Vector3(-width / 2.0f, -height / 2.0f, 0);
	m_Vertices[1].Position = Vector3(width / 2.0f, -height / 2.0f, 0);
	m_Vertices[2].Position = Vector3(-width / 2.0f, height / 2.0f, 0);
	m_Vertices[3].Position = Vector3(width / 2.0f, height / 2.0f, 0);

	for (int i = 0; i < 4; ++i) {
		m_Vertices[i].Diffuse = Color(1, 1, 1, 1);
		m_Vertices[i].TexCoord = uv[i];
	}

	m_VertexBuffer.Create(m_Vertices);

	std::vector<unsigned int> indices = { 0, 1, 2, 3 };
	m_IndexBuffer.Create(indices);
}

/**
 * @brief UV���W���X�V����
 *
 * @param uv �V����UV���W�i�z��4�F����E�E��E�����E�E���j
 *
 * @note �ύX��͒��_�o�b�t�@���č\�z����܂��B
 */
void CSprite::ModifyUV(const Vector2* uv)
{
	m_Vertices[0].TexCoord = uv[0];
	m_Vertices[1].TexCoord = uv[1];
	m_Vertices[2].TexCoord = uv[2];
	m_Vertices[3].TexCoord = uv[3];

	m_VertexBuffer.Modify(m_Vertices);
}

/**
 * @brief �X�v���C�g��`�悷��
 *
 * �w�肳�ꂽ�X�P�[���E��]�E�ʒu�ŃX�v���C�g��`�悵�܂��B
 *
 * @param scale �X�P�[�����O�iX, Y, Z�j
 * @param rotation ��]�p�iX, Y, Z�F���W�A���j
 * @param pos ���[���h��ԏ�̈ʒu
 */
void CSprite::Draw(Vector3 scale, Vector3 rotation, Vector3 pos)
{
	SRT srt;
	srt.scale = scale;
	srt.rot = rotation;
	srt.pos = pos;

	Matrix4x4 worldmtx = srt.GetMatrix();

	Renderer::SetWorldViewProjection2D();
	Renderer::SetWorldMatrix(&worldmtx);

	ID3D11DeviceContext* devicecontext = Renderer::GetDeviceContext();
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();
	m_Material.SetGPU();
	m_Texture.SetGPU();

	devicecontext->DrawIndexed(
		4, // �C���f�b�N�X��
		0, // �J�n�C���f�b�N�X
		0  // ���_�o�b�t�@�̃I�t�Z�b�g
	);
}

/**
 * @brief �X�v���C�g��`��
 *
 * @param Matrix4x4 �s��
 */
void CSprite::Draw(Matrix4x4 mtx) 
{
	Renderer::SetWorldViewProjection2D();
	Renderer::SetWorldMatrix(&mtx);

	ID3D11DeviceContext* devicecontext = Renderer::GetDeviceContext();
	devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();
	m_Material.SetGPU();
	m_Texture.SetGPU();

	devicecontext->DrawIndexed(
		4, // �C���f�b�N�X��
		0, // �J�n�C���f�b�N�X
		0  // ���_�o�b�t�@�̃I�t�Z�b�g
	);
}

/**
 * @brief �g�p���Ă��郊�\�[�X���������
 *
 * @note ���݂͖������ł����A�����I�ȉ��������ǉ�����ۂɎg�p���܂��B
 */
void CSprite::Dispose()
{
	// �K�v�ɉ����ă��\�[�X������L�q
}
