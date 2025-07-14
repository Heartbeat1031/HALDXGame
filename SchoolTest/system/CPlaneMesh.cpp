#include	"CPlaneMesh.h"
#include	<iostream>

void CPlaneMesh::Init(
	int divx, int divy,
	float width, float height,
	Color color,
	Vector3 normal,
	bool xzflag,
	bool cwflag) 
{
	// �T�C�Y�Z�b�g�i���ƍ����j�iXY���ʁj
	m_width = width;
	m_height = height;

	// ������
	m_divY = divy;
	m_divX = divx;

	// �@���x�N�g��
	m_normal = normal;

	// ���_�J���[
	m_color = color;

	if (xzflag) {
		// ���_�f�[�^����
		CreateVertexXZ();
	}
	else {
		// ���_�f�[�^����
		CreateVertex();
	}

	// �C���f�b�N�X�f�[�^����
	if (cwflag) {
		CreateIndexCW();
	}
	else {
		CreateIndexCCW();
	}
}

void CPlaneMesh::CreateVertex() {
	// ���_�f�[�^�N���A
	m_vertices.clear();

	for (unsigned int y = 0; y <= m_divY; y++) {
		VERTEX_3D	vtx{};

		for (unsigned int x = 0; x <= m_divX; x++) {

			// ���_���W�Z�b�g
			vtx.Position.x = -m_width / 2.0f + x * m_width / m_divX;
			vtx.Position.y = -m_height / 2.0f + y * m_height / m_divY;
			vtx.Position.z = 0.0f;

			// �@���x�N�g���Z�b�g
			vtx.Normal = m_normal;				// �@�����Z�b�g

			vtx.Diffuse = m_color;					// �J���[�l�Z�b�g

			// �J��Ԃ��ɑΉ��i�e�N�X�`���́j
			float texu = 1.0f * m_divX;
			float texv = 1.0f * m_divY;
			vtx.TexCoord.x = (texu * x / m_divX);
//			vtx.TexCoord.y = (texv * y / m_divY);			// �e�N�X�`���㉺�������܃o�O���C��
			vtx.TexCoord.y = texv-(texv * y / m_divY);

			m_vertices.emplace_back(vtx);		// ���_�f�[�^�Z�b�g
		}
	}
}

void CPlaneMesh::CreateVertexXZ() {
	// ���_�f�[�^�N���A
	m_vertices.clear();

	for (unsigned int y = 0; y <= m_divY; y++) {
		VERTEX_3D	vtx{};

		for (unsigned int x = 0; x <= m_divX; x++) {

			// ���_���W�Z�b�g
			vtx.Position.x = -m_width / 2.0f + x * m_width / m_divX;
			vtx.Position.y = 0.0f;
			vtx.Position.z = -m_height / 2.0f + y * m_height / m_divY;

			// �@���x�N�g���Z�b�g
			vtx.Normal = m_normal;				// �@�����Z�b�g

			vtx.Diffuse = m_color;					// �J���[�l�Z�b�g

			// �J��Ԃ��ɑΉ��i�e�N�X�`���́j
			float texu = 1.0f * m_divX;
			float texv = 1.0f * m_divY;
			vtx.TexCoord.x = (texu * x / m_divX);
			vtx.TexCoord.y = (texv * y / m_divY);

			m_vertices.emplace_back(vtx);		// ���_�f�[�^�Z�b�g
		}
	}
}

void CPlaneMesh::CreateIndexCW() {
	// �C���f�b�N�X�f�[�^�N���A
	m_indices.clear();

	// �C���f�b�N�X����
	for (unsigned int y = 0; y < m_divY; y++) {
		for (unsigned int x = 0; x < m_divX; x++) {
			int count = (m_divX + 1) * y + x;		// �������W�̃C���f�b�N�X

			// ������
			{
				FACE face{};

				face.idx[0] = count;						// ����
				face.idx[1] = count + 1 + (m_divX + 1);		// �E��
				face.idx[2] = count + 1;					// �E

				m_indices.emplace_back(face.idx[0]);
				m_indices.emplace_back(face.idx[1]);
				m_indices.emplace_back(face.idx[2]);

			}

			// �㔼��
			{
				FACE face{};
				face.idx[0] = count;						// ����
				face.idx[1] = count + (m_divX + 1);			// ��
				face.idx[2] = count + (m_divX + 1) + 1;		// �E��

				m_indices.emplace_back(face.idx[0]);
				m_indices.emplace_back(face.idx[1]);
				m_indices.emplace_back(face.idx[2]);

			}
		}
	}
}

void CPlaneMesh::CreateIndexCCW() {
	// �C���f�b�N�X�f�[�^�N���A
	m_indices.clear();

	// �C���f�b�N�X����
	for (unsigned int y = 0; y < m_divY; y++) {
		for (unsigned int x = 0; x < m_divX; x++) {
			int count = (m_divX + 1) * y + x;		// �������W�̃C���f�b�N�X

			// ������
			{
				FACE face{};

				face.idx[0] = count;						// ����
				face.idx[1] = count + 1 + (m_divX + 1);		// �E��
				face.idx[2] = count + 1;					// �E

				m_indices.emplace_back(face.idx[0]);
				m_indices.emplace_back(face.idx[2]);
				m_indices.emplace_back(face.idx[1]);

			}

			// �㔼��
			{
				FACE face{};
				face.idx[0] = count;						// ����
				face.idx[1] = count + (m_divX + 1);			// ��
				face.idx[2] = count + (m_divX + 1) + 1;		// �E��

				m_indices.emplace_back(face.idx[0]);
				m_indices.emplace_back(face.idx[2]);
				m_indices.emplace_back(face.idx[1]);

			}
		}
	}
}

// ��
float CPlaneMesh::GetWidth() {
	return m_width;
}

// ����
float CPlaneMesh::GetHeight() {
	return m_height;
}

int CPlaneMesh::GetDivX() {
	return m_divX;
}

int CPlaneMesh::GetDivY() {
	return m_divY;
}

