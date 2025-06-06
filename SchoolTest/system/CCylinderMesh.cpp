#include	<iostream>
#include	"CCylinderMesh.h"

/**
 * @brief �~�����b�V���̏���������
 *
 * �w�肳�ꂽ�������E���a�E�����E�F�Ɋ�Â��ĉ~���̒��_�ƃC���f�b�N�X�𐶐����܂��B
 *
 * @param divx �~�������̕�����
 * @param radius �~���̔��a
 * @param height �~���̍���
 * @param color ���_�J���[
 */
void CCylinderMesh::Init(
	int		divx,
	float	radius,
	float	height,
	Color color)
{
	// �T�C�Y�Z�b�g�i���ƍ����j�iXY���ʁj
	m_height = height;
	m_width = 2.0f * PI * radius;		// ���a�~�~����
	m_radius = radius;

	// ������
	m_division_x = divx;

	// ���_�J���[
	m_color = color;

	// ���_�f�[�^����
	CreateVertex();
}

/**
 * @brief �~�����b�V���̒��_�ƃC���f�b�N�X�f�[�^�𐶐�����
 *
 * ���ʁA��ʁA��ʂ��\�����邷�ׂĂ̒��_�E�C���f�b�N�X���v�Z�� `m_vertices` ����� `m_indices` �Ɋi�[���܂��B
 */
void CCylinderMesh::CreateVertex() {

	// ���_�f�[�^�N���A
	m_vertices.clear();

	// ��ʂ̉~����̒��_�f�[�^���쐬
	float azimuth = 0.0f;			// ���ʊp

	// ���ʂ̒��_�f�[�^�i��ʁj
	for (unsigned int i = 0; i <= m_division_x; i++) {
		azimuth = (2 * PI * (float)i) / (float)m_division_x;	// ���ʊp���Z�b�g
		VERTEX_3D	v{};

		v.Position.x = m_radius * cosf(azimuth);
		v.Position.y = 0.0f;
		v.Position.z = m_radius * sinf(azimuth);

		v.Normal = v.Position;
		v.Normal.Normalize();			// �@���x�N�g���𐳋K��	
		v.Diffuse = m_color;

		m_vertices.emplace_back(v);
	}

	// ���ʂ̒��_�f�[�^�i��ʁj
	for (unsigned int i = 0; i <= m_division_x; i++) {
		azimuth = (2 * PI * (float)i) / (float)m_division_x;	// ���ʊp���Z�b�g
		VERTEX_3D	v{};

		v.Position.x = m_radius * cosf(azimuth);
		v.Position.y = m_height;
		v.Position.z = m_radius * sinf(azimuth);

		v.Normal = Vector3(0.0f, 1.0f, 0.0f);	// �@���͏����
		v.Diffuse = m_color;

		m_vertices.emplace_back(v);
	}

	// ���ʁi�O�p�`�X�g���b�v�j�C���f�b�N�X�i���i�j
	for (unsigned int i = 0; i < m_division_x; i++) {
		m_indices.push_back(i);
		m_indices.push_back(i + m_division_x + 2);
		m_indices.push_back(i + 1);
	}

	// ���ʁi�O�p�`�X�g���b�v�j�C���f�b�N�X�i��i�j
	for (unsigned int i = 0; i < m_division_x; i++) {
		m_indices.push_back(i);
		m_indices.push_back(i + m_division_x + 1);
		m_indices.push_back(i + m_division_x + 2);
	}

	// ��ʒ��S�̒��_��ǉ�
	{
		VERTEX_3D v{};
		v.Position = Vector3(0.0f, 0.0f, 0.0f);
		v.Normal = Vector3(0.0f, 1.0f, 0.0f);
		v.Diffuse = m_color;
		m_vertices.emplace_back(v);
	}

	// ��ʂ̒��S�C���f�b�N�X
	unsigned int bottomcenteridx = static_cast<unsigned int>(m_vertices.size() - 1);

	// ��ʂ̎O�p�`�C���f�b�N�X
	for (unsigned int i = 0; i < m_division_x; i++) {
		m_indices.push_back(bottomcenteridx);
		m_indices.push_back(i);
		m_indices.push_back(i + 1);
	}

	// ��ʒ��S�̒��_��ǉ�
	{
		VERTEX_3D v{};
		v.Position = Vector3(0.0f, m_height, 0.0f);
		v.Normal = Vector3(0.0f, 1.0f, 0.0f);
		v.Diffuse = m_color;
		m_vertices.emplace_back(v);
	}

	// ��ʂ̒��S�C���f�b�N�X
	int topcenteridx = static_cast<int>(m_vertices.size() - 1);

	// ��ʂ̎O�p�`�C���f�b�N�X
	for (unsigned int i = 0; i < m_division_x; i++) {
		m_indices.push_back(topcenteridx);
		m_indices.push_back(i + m_division_x + 2);
		m_indices.push_back(i + m_division_x + 1);
	}
}
