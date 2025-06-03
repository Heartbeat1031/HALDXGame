//
// Created by clcc777 on 25-6-4.
//
#include "HalDebugRenderer.h"

#include "Global.h"

HalDebugRenderer::HalDebugRenderer() {
}

void HalDebugRenderer::DrawLine(JPH::RVec3Arg inFrom, JPH::RVec3Arg inTo, JPH::ColorArg inColor) {
    DebugVertex vertices[2];
    vertices[0].position = { (float)inFrom.GetX(), (float)inFrom.GetY(), (float)inFrom.GetZ() };
    vertices[1].position = { (float)inTo.GetX(),   (float)inTo.GetY(),   (float)inTo.GetZ() };

    JPH::Color color = inColor;
    DirectX::XMFLOAT4 dxColor = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };
    vertices[0].color = dxColor;
    vertices[1].color = dxColor;

    auto context = halgame->GetDeviceContext();

    // Update buffer
    D3D11_MAPPED_SUBRESOURCE mappedData;
    context->Map(m_LineVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
    memcpy(mappedData.pData, vertices, sizeof(vertices));
    context->Unmap(m_LineVertexBuffer.Get(), 0);

    UINT stride = sizeof(DebugVertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_LineVertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    context->IASetInputLayout(m_InputLayout.Get());
    context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
    context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
    context->Draw(2, 0);
}

void HalDebugRenderer::DrawTriangle(JPH::RVec3Arg inV1, JPH::RVec3Arg inV2, JPH::RVec3Arg inV3, JPH::ColorArg inColor, ECastShadow inCastShadow)
{
    DebugVertex vertices[3];
    vertices[0].position = { (float)inV1.GetX(), (float)inV1.GetY(), (float)inV1.GetZ() };
    vertices[1].position = { (float)inV2.GetX(), (float)inV2.GetY(), (float)inV2.GetZ() };
    vertices[2].position = { (float)inV3.GetX(), (float)inV3.GetY(), (float)inV3.GetZ() };

    JPH::Color color = inColor;
    DirectX::XMFLOAT4 dxColor = {
        color.r / 255.0f,
        color.g / 255.0f,
        color.b / 255.0f,
        color.a / 255.0f
    };
    for (int i = 0; i < 3; ++i)
        vertices[i].color = dxColor;

    auto context = halgame->GetDeviceContext();

    // Update buffer
    D3D11_MAPPED_SUBRESOURCE mappedData;
    context->Map(m_LineVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
    memcpy(mappedData.pData, vertices, sizeof(vertices));
    context->Unmap(m_LineVertexBuffer.Get(), 0);

    UINT stride = sizeof(DebugVertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, m_LineVertexBuffer.GetAddressOf(), &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(m_InputLayout.Get());
    context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
    context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
    context->Draw(3, 0);
}


void HalDebugRenderer::DrawText3D(JPH::RVec3Arg inPosition, const std::string_view &inString, JPH::ColorArg inColor, float inHeight) {

}
