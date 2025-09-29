#include "ClientC.h"

void ClientC::Init() {
    if (!Client) {
        Client = enet_host_create(nullptr, 1, Channels, 0, 0);
        if (!Client) {
            std::fprintf(stderr, "[ClientC] Failed to create client host\n");
            return;
        }
        ENetAddress addr{};
        if (enet_address_set_host(&addr, Hostname.c_str()) != 0) {
            std::fprintf(stderr, "[ClientC] Invalid host: %s\n", Hostname.c_str());
            return;
        }
        addr.port = Port;

        Peer = enet_host_connect(Client, &addr, Channels, 0);
        if (!Peer) {
            std::fprintf(stderr, "[ClientC] No available peers for connection\n");
            return;
        }
        std::printf("[ClientC] Connecting to %s:%u ...\n", Hostname.c_str(), Port);
    }
}

void ClientC::Uninit() {
    if (Peer) {
        enet_peer_disconnect(Peer, 0);
        ENetEvent ev;
        // 等待优雅断开，超时后强制重置
        while (enet_host_service(Client, &ev, 100) > 0) {
            if (ev.type == ENET_EVENT_TYPE_DISCONNECT) { break; }
        }
        enet_peer_reset(Peer);
        Peer = nullptr;
        Connected = false;
    }
    if (Client) {
        enet_host_destroy(Client);
        Client = nullptr;
        std::printf("[ClientC] Stopped\n");
    }
}

void ClientC::Update(float /*dt*/) {
    if (!Client) { return; }
    ENetEvent ev;
    while (enet_host_service(Client, &ev, 0) > 0) {
        HandleEvent(ev);
    }
}

void ClientC::HandleEvent(const ENetEvent& ev) {
    switch (ev.type) {
        case ENET_EVENT_TYPE_CONNECT:
            Connected = true;
            if (Events.OnConnect) { Events.OnConnect(ev.peer); }
            std::printf("[ClientC] Connected\n");
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            if (Events.OnReceive) { Events.OnReceive(ev.peer, ev.packet); }
            enet_packet_destroy(ev.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            Connected = false;
            if (Events.OnDisconnect) { Events.OnDisconnect(ev.peer); }
            std::printf("[ClientC] Disconnected\n");
            break;
        default: break;
    }
}

void ClientC::Send(const void* data, size_t size, uint8_t channel, NetReliability r) {
    if (!Peer || !Connected) { return; }
    ENetPacket* pkt = MakePacket(data, size, r);
    enet_peer_send(Peer, channel, pkt);
    enet_host_flush(Client);
}

void ClientC::Disconnect(uint32_t data) {
    if (Peer && Connected) {
        enet_peer_disconnect(Peer, data);
    }
}
