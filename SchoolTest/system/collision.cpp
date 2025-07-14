#include	<SimpleMath.h>
#include	"collision.h"

using namespace DirectX::SimpleMath;

// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width)
{

	// �����ȃx�N�g�������߂�
	Vector3 zaxis(0, 0, 1);
	Vector3 line = endpos - startpos;

	Vector3 ans;
	ans = line.Cross(zaxis);					// �O��
	ans.Normalize();							// ���K��

	Vector3 startposplus = startpos + ans * width / 2.0f;
	Vector3 startposminus = startpos - ans * width / 2.0f;

	Vector3 endposplus = endpos + ans * width / 2.0f;
	Vector3 endposminus = endpos - ans * width / 2.0f;

	v[0] = startposplus;
	v[1] = endposplus;

	v[2] = startposminus;
	v[3] = endposminus;

}

// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3 up,
	DirectX::SimpleMath::Vector3* v,
	float width)
{

	// �����ȃx�N�g�������߂�
	Vector3 zaxis=up;
	Vector3 line = endpos - startpos;



	Vector3 ans;
	ans = line.Cross(zaxis);					// �O��
	ans.Normalize();							// ���K��

	Vector3 startposplus = startpos + ans * width / 2.0f;
	Vector3 startposminus = startpos - ans * width / 2.0f;

	Vector3 endposplus = endpos + ans * width / 2.0f;
	Vector3 endposminus = endpos - ans * width / 2.0f;

	v[0] = startposplus;
	v[1] = endposplus;

	v[2] = startposminus;
	v[3] = endposminus;

}



// �����Ɠ_�̋��������߂�
float calcPointLineDist(
	const Vector3& point,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {

	float distance = 0.0f;

	// �����̃x�N�g�������߂�
	double ABx = segment.endpoint.x - segment.startpoint.x;
	double ABy = segment.endpoint.y - segment.startpoint.y;
	double ABz = segment.endpoint.z - segment.startpoint.z;

	// �����̎n�_�Ɠ_�����ԃx�N�g�������߂�
	double APx = point.x - segment.startpoint.x;
	double APy = point.y - segment.startpoint.y;
	double APz = point.z - segment.startpoint.z;

	// ����AB�̒��������߂�
	double AB2 = ABx * ABx + ABy * ABy + ABz * ABz;

	// AB�x�N�g����AP�x�N�g���̓��ς��v�Z����i�ˉe�������������߂�j
	double ABdotAP = ABx * APx + ABy * APy + ABz * APz;

	// tt���v�Z
	double tt = ABdotAP / AB2;

	// �����̑�
	intersectionpoint.x = static_cast<float>(segment.startpoint.x + ABx * tt);
	intersectionpoint.y = static_cast<float>(segment.startpoint.y + ABy * tt);
	intersectionpoint.z = static_cast<float>(segment.startpoint.z + ABz * tt);

	t = static_cast<float>(tt);
	// �����̑��̒���
	distance = (intersectionpoint - point).Length();
	return distance;
}

// �����ƒ����̒��������߂�
float calcPointSegmentDist(
	const Vector3& p,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {

	float distance = calcPointLineDist(p, segment, intersectionpoint, t);

	// ��_�������̊O�ɂ���i�n�_�ɋ߂��j
	if (t < 0.0f) {

		intersectionpoint = segment.startpoint;		// �J�n�_����_

		float l = (p - intersectionpoint).Length();	// ��_�Ƃ̋��������߂�

		return l;
	}

	// ��_�������̊O�ɂ���i�I�_�ɋ߂��j
	if (t > 1.0f) {

		intersectionpoint = segment.endpoint;		// �I�_����_

		float l = (p - intersectionpoint).Length();	// ��_�Ƃ̋��������߂�

		return l;
	}

	// ��_��������ɂ���i�O�������P�j
	return distance;
}

// �����Ɠ_�̋��������߂���@���g�p����
bool CollisionSphereCylinder(BoundingSphere sphere, BoundingCylinder cylinder)
{
	// �����̑�
	Vector3 intersectionpoint;
	float t{};

	Segment seg;
	seg.startpoint = cylinder.bottom;
	seg.endpoint = cylinder.top;

	// ���̒��S�Ɛ����̋��������߂�
	float length = calcPointSegmentDist(
		sphere.center,
		seg,
		intersectionpoint,
		t);

	// ���̒��S�Ɛ����̋��������a��菬�����ꍇ�͓������Ă���
	if (length > sphere.radius + cylinder.radius) {
		return false;
	}

	// �����̎n�_�ɋ߂�
	if(t<0){
		BoundingBoxAABB aabb;

		aabb.max.x = cylinder.top.x + cylinder.radius;
		aabb.max.y = cylinder.top.y;
		aabb.max.z = cylinder.top.z + cylinder.radius;

		aabb.min.x = cylinder.bottom.x - cylinder.radius;
		aabb.min.y = cylinder.bottom.y;
		aabb.min.z = cylinder.bottom.z - cylinder.radius;

		float length = SqDistPointAABB(sphere.center, aabb);
		length = sqrt(length);

		if (length < sphere.radius) {
			return true;
		}
	}
	// �����̏I�_�ɋ߂�
	else if(t>1){
		BoundingBoxAABB aabb;

		aabb.max.x = cylinder.top.x + cylinder.radius;
		aabb.max.y = cylinder.top.y;
		aabb.max.z = cylinder.top.z + cylinder.radius;

		aabb.min.x = cylinder.bottom.x - cylinder.radius;
		aabb.min.y = cylinder.bottom.y;
		aabb.min.z = cylinder.bottom.z - cylinder.radius;

		float length = SqDistPointAABB(sphere.center, aabb);
		length = sqrt(length);

		if (length < sphere.radius) {
			return true;
		}
	}
	// �����̎n�_�ƏI�_�̊Ԃɂ��邩�H
	else {
		return true;
	}

	return false;
}

// ��
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2)
{

	float length = (p1.center - p2.center).Length();

	if (p1.radius + p2.radius > length) {
		return false;
	}

	return true;
}

// AABB
bool CollisionAABB(BoundingBoxAABB p1, BoundingBoxAABB p2){

	// X���W
	if (p1.max.x < p2.min.x) {
		return false;
	}

	if (p1.min.x > p2.max.x) {
		return false;
	}

	// Y���W
	if (p1.max.y < p2.min.y) {
		return false;
	}

	if (p1.min.y > p2.max.y) {
		return false;
	}

	// Z���W
	if (p1.max.z < p2.min.z) {
		return false;
	}

	if (p1.min.z > p2.max.z) {
		return false;
	}

	return true;
}


BoundingBoxAABB SetAABB(Vector3 centerposition,float width ,float height,float depth) {

	BoundingBoxAABB aabb{};

	width = fabs(width);
	height = fabs(height);
	depth = fabs(depth);

	aabb.min.x = centerposition.x - width / 2.0f;
	aabb.min.y = centerposition.y - height / 2.0f;
	aabb.min.z = centerposition.z - depth / 2.0f;

	aabb.max.x = centerposition.x + width / 2.0f;
	aabb.max.y = centerposition.y + height / 2.0f;
	aabb.max.z = centerposition.z + depth / 2.0f;

	return aabb;
}

// AABB�Ɠ_�̂����Ƌ߂��_�����߂�
void ClosestPtPointAABB(Vector3 p, BoundingBoxAABB aabb, Vector3& q)
{
	q.x = max(aabb.min.x, min(p.x, aabb.max.x));
	q.y = max(aabb.min.y, min(p.y, aabb.max.y));
	q.z = max(aabb.min.z, min(p.z, aabb.max.z));
}

// AABB�Ɠ_�̋��������߂�i�Q��j
float SqDistPointAABB(Vector3 p, BoundingBoxAABB aabb)
{
	float sqDist = 0.0f;

	if (p.x < aabb.min.x) sqDist += (aabb.min.x - p.x) * (aabb.min.x - p.x);
	if (p.x > aabb.max.x) sqDist += (p.x - aabb.max.x) * (p.x - aabb.max.x);

	if (p.y < aabb.min.y) sqDist += (aabb.min.y - p.y) * (aabb.min.y - p.y);
	if (p.y > aabb.max.y) sqDist += (p.y - aabb.max.y) * (p.y - aabb.max.y);

	if (p.z < aabb.min.z) sqDist += (aabb.min.z - p.z) * (aabb.min.z - p.z);
	if (p.z > aabb.max.z) sqDist += (p.z - aabb.max.z) * (p.z - aabb.max.z);

	return sqDist;
}

// AABB�Ƌ��̓����蔻��
bool CollisionSphereAABB(
	BoundingSphere sphere,
	BoundingBoxAABB aabb)
{
	// AABB�Ɠ_�̋��������߂�
	float lng = SqDistPointAABB(sphere.center, aabb);

	if (lng < sphere.radius * sphere.radius)
	{
		return true;
	}

	return false;
}

// OBB�Ɠ_�̂����Ƌ߂��_�����߂�
void ClosestPtPointOBB(Vector3 point, BoundingBoxOBB obb, Vector3& answer)
{
	// �_����OBB�̒��S�ւ̃x�N�g��(���������߂�)
	Vector3 diff = point - obb.worldcenter;

	answer = obb.worldcenter;

	// OBB�̃��[�J�����W�ɕϊ��i���͐��K������Ă��邱�Ƃ�O��Ƃ���j
	float obbaxisx = diff.Dot(obb.axisX);
	float obbaxisy = diff.Dot(obb.axisY);
	float obbaxisz = diff.Dot(obb.axisZ);

	// OBB�̊e���ɉ������ŋߐړ_�����߂�
	obbaxisx = std::clamp(obbaxisx, -0.5f * obb.lengthx, 0.5f * obb.lengthx);
	obbaxisy = std::clamp(obbaxisy, -0.5f * obb.lengthy, 0.5f * obb.lengthy);
	obbaxisz = std::clamp(obbaxisz, -0.5f * obb.lengthz, 0.5f * obb.lengthz);

	// ���[���h���W�ɕϊ�
	answer = obb.worldcenter + obb.axisX * obbaxisx + obb.axisY * obbaxisy + obb.axisZ * obbaxisz;
}

// OBB�Ɠ_�̋��������߂�
float DistPointOBB(Vector3 point, BoundingBoxOBB obb)
{
	Vector3 answer{};

	// OBB�Ɠ_�̂����Ƌ߂��_�����߂�
	ClosestPtPointOBB(point, obb, answer);
	// ���������߂�
	float length = (answer - point).Length();

	return length;
}

// OBB�̐ݒ�
BoundingBoxOBB SetOBB(
	Vector3 rot,					// �p���i��]�p�x�j
	Vector3 pos,					// ���S���W�i���[���h�j
	float width,					// ��
	float height,					// ����
	float depth)					// ���s
{
	BoundingBoxOBB obb{};

	BoundingBoxAABB aabb = SetAABB(pos, width, height, depth);
	obb.max = aabb.max;
	obb.min = aabb.min;

	Matrix mtx = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);

	obb.axisX = Vector3(mtx._11, mtx._12, mtx._13);
	obb.axisY = Vector3(mtx._21, mtx._22, mtx._23);
	obb.axisZ = Vector3(mtx._31, mtx._32, mtx._33);

	obb.lengthx = width;
	obb.lengthy = height;
	obb.lengthz = depth;

	obb.worldcenter = pos;
	obb.center = Vector3(0, 0, 0);

	return obb;
}

// OBB�Ƌ��̓����蔻��
bool CollisionSphereOBB(
	BoundingSphere sphere,
	BoundingBoxOBB obb)
{
	// OBB�Ɠ_�̋��������߂�
	float lng = DistPointOBB(sphere.center, obb);

	if (lng < sphere.radius)
	{
		return true;
	}

	return false;
}

// �J�v�Z�����m�̓����蔻��
bool CollisionCapsule(BoundingCapsule caps1, BoundingCapsule caps2)
{
	// �����̎n�_�ƏI�_�����߂�
	Segment seg1;
	seg1.startpoint = caps1.startpoint;
	seg1.endpoint = caps1.endpoint;

	Segment seg2;
	seg2.startpoint = caps2.startpoint;
	seg2.endpoint = caps2.endpoint;

	// �Q�̐����̍ŒZ���������߂�
	Vector3 p1, p2;
	float s, t;
	float length = ClosestPtSegmentSegment(seg1, seg2, s, t, p1, p2);

	// ���̔��a���l�����ē����蔻��
	if (length < caps1.radius + caps2.radius) {
		return true;
	}

	return false;
}

// �����Ɛ����̍ŒZ���������߂�
float ClosestPtSegmentSegment(
	const Segment& seg1,				// �����P
	const Segment& seg2, 				// �����Q
	float& s,							// �����P�̍ŒZ�����̃p�����[�^
	float& t,							// �����Q�̍ŒZ�����̃p�����[�^
	Vector3& p1, 						// �����P�̍ŒZ�����̓_
	Vector3& p2) 						// �����Q�̍ŒZ�����̓_
{
	Vector3 d1 = seg1.endpoint - seg1.startpoint;	// �����P�̕���
	Vector3 d2 = seg2.endpoint - seg2.startpoint;	// �����Q�̕���
	Vector3 r = seg1.startpoint - seg2.startpoint;	// �����P�̎n�_�Ɛ����Q�̎n�_�����ԃx�N�g��

	float a = d1.Dot(d1);	// �����P�̒����̂Q��
	float e = d2.Dot(d2);	// �����Q�̒����̂Q��
	float f = d2.Dot(r);	// �����Q�̎n�_��������P�̎n�_�ւ̃x�N�g���̒����̂Q��

	// �����P���k�ނ��Ă��Ȃ����H�@���@�����Q���k�ނ��Ă��Ȃ����H
	if (a <= 1e-5 && e <= 1e-5) {
		s = t = 0.0f;
		p1 = seg1.startpoint;
		p2 = seg2.startpoint;
		return (p1 - p2).Length();		// �k�ނ��Ă���ꍇ�͓_�̋���
	}

	if (a <= 1e-5) {		// �����P���k�ނ��Ă���ꍇ
		s = 0.0f;
		t = std::clamp(f / e, 0.0f, 1.0f);
	}
	else {
		float c = d1.Dot(r);
		if (e <= 1e-5) {	// �����Q���k�ނ��Ă���ꍇ
			t = 0.0f;
			s = std::clamp(-c / a, 0.0f, 1.0f);
		}
		else {
			// �������m�̋��������߂�
			float b = d1.Dot(d2);

			// �s�񎮂����߂�
			float denom = a * e - b * b; // Always nonnegative

			// If segments are not parallel, compute closest point on L1 to L2 and
			// clamp to segment S1. Else pick arbitrary s (here 0)

			if (denom != 0.0f) {
				s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
			}
			else {
				s = 0.0f;
			}

			t = (b * s + f) / e;

			if (t < 0.0f) {
				t = 0.0f;
				s = std::clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f) {
				t = 1.0f;
				s = std::clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	p1 = seg1.startpoint + d1 * s;
	p2 = seg2.startpoint + d2 * t;
	return (p1 - p2).Length();

}



