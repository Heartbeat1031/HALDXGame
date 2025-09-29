#pragma once
#include "NetC.h"
#include "Component.h"

class ServerC : public Component {

private:
    ENetHost* Server = nullptr;
    std::vector<ENetPeer*> Peers;

    void HandleEvent(const ENetEvent& ev);
public:
    uint16_t Port = 12345;
    size_t   MaxClients = 32;
    size_t   Channels   = 2;
    NetEvents Events;

public:
    ServerC() = default;
    ~ServerC() override = default;

    void Init() override;           // 创建 ENetHost（监听）
    void Uninit() override;         // 销毁 ENetHost
    void Update(float dt) override; // 轮询 ENet 事件

    void Broadcast(const void* data, size_t size, uint8_t channel = 0, NetReliability r = NetReliability::Reliable);
    void SendTo(ENetPeer* peer, const void* data, size_t size, uint8_t channel = 0, NetReliability r = NetReliability::Reliable);

    const std::vector<ENetPeer*>& GetPeers() const { return Peers; }
};
