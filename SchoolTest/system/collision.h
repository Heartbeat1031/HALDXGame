#pragma once
#include	"CommonTypes.h"
#include	"transform.h"
#include	"C3DShape.h"
#include	<algorithm>

namespace GM31 {namespace GE {namespace {}
	namespace Collision {
		// �~����`
		struct BoundingCylinder {
			Vector3    bottom;						// ��
			Vector3    top;							// ��
			float      radius;						// ���a
		};

		// ����`
		struct BoundingSphere {
			Vector3	center;
			float	radius;
		};

		// �Z�O�����g�i�����j
		struct Segment {
			Vector3		startpoint;				// �J�n�_
			Vector3		endpoint;				// �I���_
		};

		// ����
		struct Line {
			Vector3		point;					// ������̔C�ӂ̓_
			Vector3		direction;				// ����
		};

		// ���E�{�b�N�XAABB
		struct BoundingBoxAABB {
			Vector3 min;
			Vector3 max;
		};

		// ���E�{�b�N�XOBB
		struct BoundingBoxOBB : public BoundingBoxAABB {
			Vector3 center;
			Vector3 worldcenter;

			// ���S���iX���AY���AZ���j
			Vector3 axisX;
			Vector3 axisY;
			Vector3 axisZ;

			// BOX�T�C�Y
			float	lengthx;
			float	lengthy;
			float	lengthz;
		};

		// ���E�J�v�Z��
		struct BoundingCapsule {
			Vector3		startpoint;				// �J�n�_
			Vector3		endpoint;				// �I���_
			float		radius;					// ���a
		};

		// ��
		bool CollisionSphere(BoundingSphere p1, BoundingSphere p2);

		// AABB
		bool CollisionAABB(BoundingBoxAABB p1, BoundingBoxAABB p2);

		// make AABB
		BoundingBoxAABB SetAABB(Vector3 centerposition, float width, float height, float depth);

		// �����Ɠ_�̋��������߂�
		float calcPointLineDist(
			const Vector3& point,
			const Segment& segment,
			Vector3& intersectionpoint,
			float& t);

		// �����Ɠ_�̋��������߂�
		float calcPointLineDist(
			const Vector3& point,
			const Segment& segment,
			Vector3& intersectionpoint,
			float& t);

		// �����ƒ����̒��������߂�
		float calcPointSegmentDist(
			const Vector3& p,
			const Segment& segment,
			Vector3& intersectionpoint,
			float& t);

		// �~���Ƌ��̓����蔻��
		bool CollisionSphereCylinder(
			BoundingSphere sphere,
			BoundingCylinder cylinder);

		// AABB�Ɠ_�̍ŋߐړ_�����߂�
		void ClosestPtPointAABB(Vector3 p, BoundingBoxAABB aabb, Vector3& q);

		// AABB�Ɠ_�̋��������߂�
		float SqDistPointAABB(Vector3 p, BoundingBoxAABB aabb);

		// AABB�Ƌ��̓����蔻��
		bool CollisionSphereAABB(
			BoundingSphere sphere,
			BoundingBoxAABB aabb);

		// OBB�Ɠ_�̂����Ƌ߂��_�����߂�
		void ClosestPtPointOBB(Vector3 point, BoundingBoxOBB obb, Vector3& answer);

		// OBB�Ɠ_�̋��������߂�
		float DistPointOBB(Vector3 point, BoundingBoxOBB obb);

		BoundingBoxOBB SetOBB(
			Vector3 rot,					// �p���i��]�p�x�j
			Vector3 pos,					// ���S���W�i���[���h�j
			float width,					// ��
			float height,					// ����
			float depth);					// ���s��

		// OBB�Ƌ��̓����蔻��
		bool CollisionSphereOBB(
			BoundingSphere sphere,
			BoundingBoxOBB obb);

		// �J�v�Z�����m�̓����蔻��
		bool CollisionCapsule(BoundingCapsule p1, BoundingCapsule p2);

		// �����Ɛ����̍ŒZ���������߂�
		float ClosestPtSegmentSegment(
			const Segment& seg1,				// �����P
			const Segment& seg2, 				// �����Q	
			float& s,							// �����P�̍ŒZ�����̃p�����[�^
			float& t,							// �����Q�̍ŒZ�����̃p�����[�^
			Vector3& p1, 						// �����P�̍ŒZ�����̓_	
			Vector3& p2);						// �����Q�̍ŒZ�����̓_

		// �S���_�����W�ϊ��������AABB���߂�
		BoundingBoxAABB calcAABB(
			const std::vector<Vector3>& vertices,
			SRT transform);

		// AABB����8���_���擾
		std::vector<Vector3> GetCorners(const BoundingBoxAABB& box);

		// ���W�ϊ��������AABB�����߂�
		BoundingBoxAABB TransformAABB(
			const BoundingBoxAABB& inAABB,
			SRT transform);

		// OBB��OBB�̓����蔻��
		bool CollisionOBB(
			const BoundingBoxOBB& obbA,
			const BoundingBoxOBB& obbB);

		// BOUNDING CYLINDER���쐬����
		BoundingCylinder SetBoundingCylinder(
			SRT transform,		// �~���̎p�����
			Cylinder cylinder);	// �`��p�̉~�����

		// �~�����m�̓����蔻��	
		bool CollisionCylinder(
			const BoundingCylinder& Cylinder1,
			const BoundingCylinder& Cylinder2);

		// ������Ɏw�肵���_�����݂��邩�ǂ������`�F�b�N����
		bool isPointOnSegment(
			const Segment& seg,
			const Vector3& point,
			float epsilon = 1e-6f);

		// �������m�̍ŒZ���������߂�
		float ClosestDistanceBetweenLines(
			const Line& line1,
			const Line& line2,
			float& s, 			// line1 �p�����[�^�i�������j
			float& t,			// line2 �p�����[�^�i�������j
			Vector3& p1, 		// line1 �ŋߐړ_
			Vector3& p2);		// line2 �ŋߐړ_

		// �~�����m�̓����蔻�萸����
		bool PreciseCylinderCollision(
			const BoundingCylinder& Cylinder1,
			const BoundingCylinder& Cylinder2);

		// �~�����m�̓����蔻��(����)
		bool CylinderSideIntersect(
			const BoundingCylinder& cylinderSide,
			const BoundingCylinder& other);

		// �o�E���f�B���O�J�v�Z�����쐬����
		BoundingCapsule SetBoundingCapsule(
			SRT transform,		// �J�v�Z���̎p�����
			float radius,		// �J�v�Z���̔��a
			float height);		// �J�v�Z���̍���

	} // namespace Collision
}}// namespace GM31::GE::Collision