#pragma once
#include "CommonTypes.h"
#include "CMesh.h"

/**
 * @brief ���̃��b�V���𐶐��E�Ǘ�����N���X
 *
 * CMesh ���p�����A�w�肳�ꂽ�������Ɣ��a�Ɋ�Â��ċ��̂̒��_�E�C���f�b�N�X�𐶐����܂��B
 */
class CSphereMesh : public CMesh {
public:
    /**
     * @brief �����b�V���̏��������s��
     *
     * @param radius ���̔��a
     * @param color ���_�J���[
     * @param division_horizontal �o�x�����i���j�̕�����
     * @param division_vertical �ܓx�����i�c�j�̕�����
     */
    void Init(float radius,
        Color color,
        int division_horizontal,
        int division_vertical)
    {
        m_divX = division_horizontal;
        m_divY = division_vertical;
        m_radius = radius;
        m_color = color;

        CreateIndex();
        CreateVertex();
    }

    /**
     * @brief ���̂̒��_�f�[�^�𐶐�����
     *
     * �p�ƕ��ʊp�Ɋ�Â����̂̒��_�ʒu�Ɩ@���x�N�g�����v�Z���Ċi�[���܂��B
     */
    void CreateVertex() {
        m_vertices.clear();

        float azimuth = 0.0f;   ///< ���ʊp
        float elevation = 0.0f; ///< �p

        Vector3 Normal;

        for (unsigned int y = 0; y <= m_divY; y++) {
            elevation = (PI * (float)y) / (float)m_divY;
            float r = m_radius * sinf(elevation);

            for (unsigned int x = 0; x <= m_divX; x++) {
                azimuth = (2 * PI * (float)x) / (float)m_divX;

                VERTEX_3D v;
                v.Position.x = r * cosf(azimuth);
                v.Position.y = m_radius * cosf(elevation);
                v.Position.z = r * sinf(azimuth);

                Normalize(v.Position, Normal); ///< �@���x�N�g���𐳋K��
                v.Normal = Normal;
                v.Diffuse = m_color;

                m_vertices.emplace_back(v);
            }
        }
    }

    /**
     * @brief ���̂̃C���f�b�N�X�f�[�^�𐶐�����
     *
     * ���_�C���f�b�N�X���O�p�`�|���S���̃��X�g�Ƃ��Đ������܂��B
     */
    void CreateIndex() {
        struct FACE {
            unsigned int idx[3];
        };

        m_indices.clear();

        for (unsigned int y = 0; y < m_divY; y++) {
            for (unsigned int x = 0; x < m_divX; x++) {
                int count = (m_divX + 1) * y + x;

                // ��O�p�`
                m_indices.emplace_back(count);
                m_indices.emplace_back(count + 1);
                m_indices.emplace_back(count + 1 + (m_divX + 1));

                // ���O�p�`
                m_indices.emplace_back(count);
                m_indices.emplace_back(count + (m_divX + 1) + 1);
                m_indices.emplace_back(count + (m_divX + 1));
            }
        }
    }

private:
    /**
     * @brief �@���x�N�g���𐳋K������
     *
     * @param vec ���̓x�N�g��
     * @param Normal �o�͂���鐳�K���x�N�g���i�Q�Ɠn���j
     */
    void Normalize(Vector3 vec, Vector3& Normal) {
        vec.Normalize();
        Normal = vec;
    }

    unsigned int m_divX = 1;   ///< �������̕������i�o�x�j
    unsigned int m_divY = 1;   ///< �c�����̕������i�ܓx�j
    float m_radius = 100.0f;   ///< ���̔��a
    Color m_color;             ///< ���_�J���[
};
