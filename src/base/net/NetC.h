#pragma once

#include <enet/enet.h>      // ENet 头
#include <cstdint>
#include <functional>

// 发送可靠性选项
enum class NetReliability : uint8_t {
    Unreliable = 0,
    Reliable   = 1,
};

// 简单的事件回调接口（可选）
struct NetEvents {
    std::function<void(ENetPeer*)>              OnConnect;
    std::function<void(ENetPeer*, ENetPacket*)> OnReceive;
    std::function<void(ENetPeer*)>              OnDisconnect;
};

// 工具：创建 ENetPacket
inline ENetPacket* MakePacket(const void* data, size_t size, NetReliability r) {
    const enet_uint32 flags = (r == NetReliability::Reliable) ? ENET_PACKET_FLAG_RELIABLE : 0;
    return enet_packet_create(data, size, flags);
}
