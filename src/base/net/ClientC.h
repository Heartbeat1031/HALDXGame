#pragma once
#include "NetC.h"
#include "Component.h"

class ClientC : public Component {

private:
    ENetHost* Client = nullptr;
    ENetPeer* Peer   = nullptr;
    bool      Connected = false;

    void HandleEvent(const ENetEvent& ev);

public:
    std::string Hostname = "127.0.0.1";
    uint16_t    Port     = 12345;
    size_t      Channels = 2;
    NetEvents   Events;

    ClientC() = default;
    ~ClientC() override = default;

    void Init() override;           // 创建 ENet 客户端并连接
    void Uninit() override;         // 断开并销毁
    void Update(float dt) override; // 轮询事件

    bool IsConnected() const { return Connected; }

    void Send(const void* data, size_t size, uint8_t channel = 0, NetReliability r = NetReliability::Reliable);
    void Disconnect(uint32_t data = 0);
};
