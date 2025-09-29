#include "ServerC.h"

void ServerC::Init() {
    if (!Server) {
        ENetAddress address{};
        address.host = ENET_HOST_ANY;
        address.port = Port;

        Server = enet_host_create(&address, MaxClients, Channels, 0, 0);
        if (!Server) {
            std::fprintf(stderr, "[ServerC] Failed to create server on :%u\n", Port);
            return;
        }
        std::printf("[ServerC] Listening on :%u\n", Port);
    }
}

void ServerC::Uninit() {
    if (Server) {
        enet_host_flush(Server);
        enet_host_destroy(Server);
        Server = nullptr;
        Peers.clear();
        std::printf("[ServerC] Stopped\n");
    }
}

void ServerC::Update(float /*dt*/) {
    if (!Server) { return; }
    ENetEvent ev;
    while (enet_host_service(Server, &ev, 0) > 0) {
        HandleEvent(ev);
    }
}

void ServerC::HandleEvent(const ENetEvent& ev) {
    switch (ev.type) {
        case ENET_EVENT_TYPE_CONNECT:
            Peers.push_back(ev.peer);
            if (Events.OnConnect) { Events.OnConnect(ev.peer); }
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            if (Events.OnReceive) { Events.OnReceive(ev.peer, ev.packet); }
            enet_packet_destroy(ev.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT: {
            auto it = std::find(Peers.begin(), Peers.end(), ev.peer);
            if (it != Peers.end()) { Peers.erase(it); }
            if (Events.OnDisconnect) { Events.OnDisconnect(ev.peer); }
            break;
        }
        default: break;
    }
}

void ServerC::Broadcast(const void* data, size_t size, uint8_t channel, NetReliability r) {
    if (!Server) { return; }
    ENetPacket* pkt = MakePacket(data, size, r);
    enet_host_broadcast(Server, channel, pkt);
    enet_host_flush(Server);
}

void ServerC::SendTo(ENetPeer* peer, const void* data, size_t size, uint8_t channel, NetReliability r) {
    if (!peer) { return; }
    ENetPacket* pkt = MakePacket(data, size, r);
    enet_peer_send(peer, channel, pkt);
    enet_host_flush(peer->host);
}
