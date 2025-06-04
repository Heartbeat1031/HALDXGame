//
// Created by clcc777 on 25-6-4.
//

#pragma once
#include <DirectXMath.h>
#include <Jolt/Jolt.h>
#include <wrl/client.h>

#include "Buffer.h"
#include "Jolt/Renderer/DebugRendererSimple.h"

class HalDebugRenderer : public JPH::DebugRendererSimple {
public:
    HalDebugRenderer();
    void DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) override;
	void DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow) override;
    void DrawText3D(JPH::RVec3Arg inPosition, const std::string_view &inString, JPH::ColorArg inColor, float inHeight) override;
};
