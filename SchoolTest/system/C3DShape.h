#pragma once
#include "CommonTypes.h"
#include "renderer.h"

// ���N���X C3DShape
class C3DShape {
protected:
    static bool m_first;
public:
    C3DShape();
    virtual ~C3DShape() {}

    // �F���w�肵�ĕ`�悷�� (�������z�֐�)
    virtual void Draw(SRT rts, Color col) = 0;

    // �F���w�肵�ĕ`�悷�� (�������z�֐�)
    virtual void Draw(Matrix4x4 mtx, Color col) = 0;

    // �`��̖��O���擾
    virtual std::string getName() const = 0;
};

// ��(ok)
class Sphere : public C3DShape {
private:
    float m_radius;
public:
    Sphere(float r) : m_radius(r) {}

    void Draw(SRT rts, Color col) override;
    void Draw(Matrix4x4 mtx, Color col) override;

    std::string getName() const override {
        return "Sphere";
    }
};

// �~��
class Cone : public C3DShape {
private:
    float m_radius;
    float m_height;
public:
    Cone(float r, float h) : m_radius(r), m_height(h) {}

    void Draw(SRT rts, Color col) override;

    void Draw(Matrix4x4 mtx, Color col);

    std::string getName() const override {
        return "Cone";
    }
};

// �{�b�N�X(ok)
class Box : public C3DShape {
private:
    float m_width;
    float m_height;
    float m_depth;
public:
    Box(float w, float h, float d) : m_width(w), m_height(h), m_depth(d) {}

    void Draw(SRT rts, Color col) override;

    void Draw(Matrix4x4 mtx, Color col);

    std::string getName() const override {
        return "Box";
    }
};

// �~��
class Cylinder : public C3DShape {
private:
    float m_radius;
    float m_height;
public:
    Cylinder(float r, float h) : m_radius(r), m_height(h) {}

    void Draw(SRT rts, Color col) override;

    void Draw(Matrix4x4 mtx, Color col) override;

    std::string getName() const override {
        return "Cylinder";
    }
};

// �Z�O�����g
class CSegment : public C3DShape {
private:
	Vector3 m_start;
	Vector3 m_end;
	Vector3 m_direction;
	float m_length;
public:
    CSegment(Vector3 s,Vector3 e):m_start(s),m_end(e),m_direction(e-s),m_length((e-s).Length()){}

    void Draw(SRT rts, Color col) override;
    void Draw(Matrix4x4 mtx, Color col) override;

    std::string getName() const override {
        return "CSegment";
    }
};

// �Z�O�����g
class CCapusule : public C3DShape {
private:
    float m_radius;
    float m_height;
public:
    CCapusule(float r, float h) : m_radius(r), m_height(h) {}

    void SetRadius(float r) {
        m_radius = r;
    }

    void SetHeight(float h) {
        m_height = h;
    }

    float GetRadius() const {
        return m_radius;
    }

    float GetHeight() const {
        return m_height;
    }

    void Draw(SRT rts, Color col) override;

    void Draw(Matrix4x4 mtx, Color col) override;

    std::string getName() const override {
        return "Capsule";
    }
};