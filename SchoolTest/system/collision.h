#pragma once

#include	"CommonTypes.h"
#include	<algorithm>

// 円柱定義
struct BoundingCylinder {
	Vector3    bottom;						// 底
	Vector3    top;							// 上
	float	radius;							// 半径
};

// 球定義
struct BoundingSphere {
	Vector3	center;
	float	radius;
};

// セグメント（線分）
struct Segment {
	Vector3		startpoint;				// 開始点
	Vector3		endpoint;				// 終了点
};

// 境界ボックスAABB
struct BoundingBoxAABB {
	Vector3 min;
	Vector3 max;
};

// 境界ボックスOBB
struct BoundingBoxOBB : public BoundingBoxAABB {
	Vector3 center;
	Vector3 worldcenter;

	// 中心軸（X軸、Y軸、Z軸）
	Vector3 axisX;
	Vector3 axisY;
	Vector3 axisZ;

	// BOXサイズ
	float	lengthx;
	float	lengthy;
	float	lengthz;
};

// 境界カプセル
struct BoundingCapsule {
	Vector3		startpoint;				// 開始点
	Vector3		endpoint;				// 終了点
	float		radius;					// 半径
};

// 球
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2);

// AABB
bool CollisionAABB(BoundingBoxAABB p1, BoundingBoxAABB p2);

// make AABB
BoundingBoxAABB SetAABB(DirectX::SimpleMath::Vector3 centerposition, float width, float height, float depth);

// 直線と点の距離を求める
float calcPointLineDist(
	const Vector3& point,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t);

// 直線と点の距離を求める
float calcPointLineDist(
	const Vector3& point,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t);

// 線分と直線の長さを求める
float calcPointSegmentDist(
	const Vector3& p,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t);

// 円柱と球の当たり判定
bool CollisionSphereCylinder(
	BoundingSphere sphere, 
	BoundingCylinder cylinder);

// AABBと点の最近接点を求める
void ClosestPtPointAABB(Vector3 p, BoundingBoxAABB aabb, Vector3& q);

// AABBと点の距離を求める
float SqDistPointAABB(Vector3 p, BoundingBoxAABB aabb);

// AABBと球の当たり判定
bool CollisionSphereAABB(
	BoundingSphere sphere,
	BoundingBoxAABB aabb);

// OBBと点のもっと近い点を求める
void ClosestPtPointOBB(Vector3 point, BoundingBoxOBB obb, Vector3& answer);

// OBBと点の距離を求める
float DistPointOBB(Vector3 point, BoundingBoxOBB obb);

BoundingBoxOBB SetOBB(
	Vector3 rot,					// 姿勢（回転角度）
	Vector3 pos,					// 中心座標（ワールド）
	float width,					// 幅
	float height,					// 高さ
	float depth);					// 奥行き

// OBBと球の当たり判定
bool CollisionSphereOBB(
	BoundingSphere sphere,
	BoundingBoxOBB obb);

// カプセル同士の当たり判定
bool CollisionCapsule(BoundingCapsule p1, BoundingCapsule p2);

// 線分と線分の最短距離を求める
float ClosestPtSegmentSegment(
	const Segment& seg1,				// 線分１
	const Segment& seg2, 				// 線分２	
	float& s,							// 線分１の最短距離のパラメータ
	float& t,							// 線分２の最短距離のパラメータ
	Vector3& p1, 						// 線分１の最短距離の点	
	Vector3& p2);						// 線分２の最短距離の点

