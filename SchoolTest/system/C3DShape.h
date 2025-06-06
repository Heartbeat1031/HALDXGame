#pragma once
#include <string>
#include "CommonTypes.h"
#include "transform.h"

/**
 * @brief 3D�}�`�̒��ۊ��N���X
 *
 * ���ׂĂ�3D�}�`�N���X�͂��̃N���X���p�����A�`���T�C�Y�ݒ�̂��߂�
 * �C���^�[�t�F�[�X����������K�v������܂��B
 */
class C3DShape {
protected:
	static bool m_first;                // <-- �������t���O
public:
    /**
     * @brief �R���X�g���N�^
     */
    C3DShape();

    /**
     * @brief �f�X�g���N�^
     */
    virtual ~C3DShape() {}

    /**
     * @brief SRT�iScale, Rotation, Translation�j�Ő}�`��`��
     * @param srt �X�P�[���E��]�E���s�ړ����
     * @param col �`��F
     */
    virtual void Draw(SRT srt, Color col) = 0;

    /**
     * @brief �s����g�p���Đ}�`��`��
     * @param mtx �`��Ɏg�p����ϊ��s��
     * @param col �`��F
     */
    virtual void Draw(Matrix4x4 mtx, Color col) = 0;

    /**
     * @brief �}�`�̃T�C�Y��ݒ�
     * @param radius ���a�܂��͕�
     * @param height ����
     * @param depth ���s��
     */
    virtual void SetSize(float radius, float height, float depth) = 0;

    /**
     * @brief �}�`�̖��O���擾
     * @return �}�`�̖��O������
     */
    virtual std::string getName() const = 0;
};

/**
 * @brief ���̂�3D�}�`�N���X
 */
class Sphere : public C3DShape {
private:
    float m_radius;
public:
    /**
        * @brief �R���X�g���N�^
        * @param r ���a
    */
    Sphere(float r) : m_radius(r) {}

    void Draw(SRT srt, Color col) override;
    void Draw(Matrix4x4 mtx, Color col) override;

    /**
     * @brief ���a��ݒ�
    * @param r ���a
    */
	void SetRadius(float r) {
		m_radius = r;
	}

    void SetSize(float radius, float height=0 ,float depth=0) override {
		m_radius = radius;
    }

    std::string getName() const override {
        return "Sphere";
    }
};

/**
 * @brief �~����3D�}�`�N���X
 */
class Cone : public C3DShape {
private:
    float m_radius; ///< ��ʂ̔��a
    float m_height; ///< ����
public:
    Cone(float r, float h) : m_radius(r), m_height(h) {}

    void Draw(SRT srt, Color col) override;

    void Draw(Matrix4x4 mtx, Color col);

    void SetSize(float radius, float height, float depth) override {
        m_height = height;
        m_radius = radius;
    }

    std::string getName() const override {
        return "Cone";
    }
};
/**
 * @brief �{�b�N�X�i�����́j��3D�}�`�N���X
 */
class Box : public C3DShape {
private:
    float m_width;
    float m_height;
    float m_depth;
public:
    Box(float w, float h, float d) : m_width(w), m_height(h), m_depth(d) {}

    void Draw(SRT srt, Color col) override;

    void Draw(Matrix4x4 mtx, Color col);

    void SetSize(float width, float height, float depth) override {
        m_width = width;
		m_height = height;
		m_depth = depth;
	}

    std::string getName() const override {
        return "Box";
    }
};

/**
 * @brief �~����3D�}�`�N���X
 */
class Cylinder : public C3DShape {
private:
    float m_radius;
    float m_height;
public:
    Cylinder(float r, float h) : m_radius(r), m_height(h) {}

    void Draw(SRT srt, Color col) override;

    void Draw(Matrix4x4 mtx, Color col) override;

    void SetSize(float radius, float height,float depth =0 ) override {
        m_height = height;
        m_radius = radius;
    }

	float GetRadius() const {
		return m_radius;
	}

    float GetHeight() const {
        return m_height;
    }

    std::string getName() const override {
        return "Cylinder";
    }
};

/**
 * @brief �����i�Z�O�����g�j��3D�}�`�N���X
 */
class Segment : public C3DShape {
private:
    Vector3 m_start;     ///< �n�_
    Vector3 m_end;       ///< �I�_
    Vector3 m_direction; ///< �����x�N�g��
    float m_length;      ///< ����
public:
    Segment(Vector3 s,Vector3 e):m_start(s),m_end(e),m_direction(e-s),m_length((e-s).Length()){}

    void Draw(SRT srt, Color col) override;

    void Draw(Matrix4x4 mtx, Color col) override;

    void SetSize(float radius, float height, float depth) override {
    }

    std::string getName() const override {
        return "Segment";
    }
};

/**
 * @brief �J�v�Z���i�~���{���j��3D�}�`�N���X
 */
class Capsule : public C3DShape {
private:
    float m_radius; ///< ���a
    float m_height; ///< ����
public:
    Capsule(float r, float h) : m_radius(r), m_height(h) {}

    void SetRadius(float r) {
        m_radius = r;
    }

    void SetSize(float radius, float height ,float depth=0) override{
        m_height = height;
        m_radius = radius;
    }
    
    float GetHeight() const {
        return m_height;
    }

    float GetRadius() const {
        return m_radius;
    }
    void Draw(SRT srt, Color col) override;

    void Draw(Matrix4x4 mtx, Color col) override;

    std::string getName() const override {
        return "Capsule";
    }
};