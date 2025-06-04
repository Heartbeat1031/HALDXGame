#include "HalDebugRenderer.h"

#include "Global.h"
#include <imgui.h>

using namespace DirectX;

ImVec2 WorldToScreen(const JPH::RVec3 &p) {
    if (halgame == nullptr || halgame->m_pCamera == nullptr)
        return ImVec2();

    XMMATRIX vp = halgame->m_pCamera->GetViewProjMatrixXM();
    D3D11_VIEWPORT view = halgame->m_pCamera->GetViewPort();

    XMVECTOR pos = XMVectorSet((float)p.GetX(), (float)p.GetY(), (float)p.GetZ(), 1.0f);
    pos = XMVector3TransformCoord(pos, vp);


    float x = XMVectorGetX(pos) * 0.5f + 0.5f;
    float y = XMVectorGetY(pos) * 0.5f + 0.5f;

    ImVec2 result;
    result.x = view.TopLeftX + x * view.Width;
    result.y = view.TopLeftY + (1.0f - y) * view.Height;
    return result;
}

HalDebugRenderer::HalDebugRenderer() {
}
struct ScreenProjectResult {
    bool valid;
    ImVec2 screen;
};

ScreenProjectResult ProjectToScreen(const JPH::RVec3& p) {
    if (halgame == nullptr || halgame->m_pCamera == nullptr)
        return { false, {} };

    XMMATRIX vp = halgame->m_pCamera->GetViewProjMatrixXM();
    D3D11_VIEWPORT view = halgame->m_pCamera->GetViewPort();

    XMVECTOR pos = XMVectorSet((float)p.GetX(), (float)p.GetY(), (float)p.GetZ(), 1.0f);
    XMVECTOR clip = XMVector4Transform(pos, vp);

    float cx = XMVectorGetX(clip);
    float cy = XMVectorGetY(clip);
    float cz = XMVectorGetZ(clip);
    float cw = XMVectorGetW(clip);

    // 检查是否在可见范围
    if (cw <= 0.0f) return { false, {} };
    if (cx < -cw || cx > cw || cy < -cw || cy > cw || cz < 0.0f || cz > cw)
        return { false, {} };

    // NDC
    float x = cx / cw * 0.5f + 0.5f;
    float y = cy / cw * 0.5f + 0.5f;

    ImVec2 result;
    result.x = view.TopLeftX + x * view.Width;
    result.y = view.TopLeftY + (1.0f - y) * view.Height;
    return { true, result };
}

// 用于绘线
void HalDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) {
    auto p1 = ProjectToScreen(inFrom);
    auto p2 = ProjectToScreen(inTo);

    if (p1.valid && p2.valid) {
        ImU32 color = IM_COL32(inColor.r, inColor.g, inColor.b, inColor.a);
        ImGui::GetBackgroundDrawList()->AddLine(p1.screen, p2.screen, color);
    }
}

void HalDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow)
{
    ImVec2 p1 = WorldToScreen(inV1);
    ImVec2 p2 = WorldToScreen(inV2);
    ImVec2 p3 = WorldToScreen(inV3);
    ImU32 color = IM_COL32(inColor.r, inColor.g, inColor.b, inColor.a);
    ImGui::GetBackgroundDrawList()->AddTriangle(p1, p2, p3, color);
}


void HalDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const std::string_view &inString, JPH::ColorArg inColor, float inHeight) {
    ImVec2 p = WorldToScreen(inPosition);
    ImU32 color = IM_COL32(inColor.r, inColor.g, inColor.b, inColor.a);
    ImGui::GetBackgroundDrawList()->AddText(p, color, std::string(inString).c_str());
}
