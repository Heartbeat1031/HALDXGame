//
// Created by clcc777 on 25-9-30.
//
#include "ServerManagerC.h"

#include <iostream>

#include "PacketIO.h"

void ServerManagerC::Init() {
}

void ServerManagerC::Uninit() {
    Stop();
}

void ServerManagerC::Update(float dt) {
    if (!Server) { return; }
    ENetEvent ev;
    int handled = 0;
    while (handled < MAX_EVENTS_PER_FRAME && enet_host_service(Server, &ev, 0) > 0) {
        HandleEvent(ev);
        ++handled;
    }
}

void ServerManagerC::Start() {
    if (Server) {
        std::cout << "[ServerC] Server already running on :" << Port << std::endl;
        return;
    }
    ENetAddress address{};
    address.host = ENET_HOST_ANY;
    address.port = Port;

    Server = enet_host_create(&address, MaxClients, Channels, 0, 0);
    if (!Server) {
        std::cout << "[ServerC] Failed to create server on :" << Port << std::endl;
        return;
    }
    std::cout << "[ServerC] Listening on :" << Port << " succeed" << std::endl;
}

void ServerManagerC::Stop() {
    if (Server == nullptr) {
        return;
    }
    enet_host_flush(Server);
    enet_host_destroy(Server);
    Server = nullptr;
    Peers.clear();
    std::cout << "[ServerC] Stopped" << std::endl;
}

void ServerManagerC::HandleEvent(const ENetEvent &ev) {
    switch (ev.type) {
        case ENET_EVENT_TYPE_CONNECT:
            OnConnect(ev.peer);
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            OnReceive(ev.peer, ev.packet);
            enet_packet_destroy(ev.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            OnDisconnect(ev.peer);
            break;
        default: break;
    }
}

void ServerManagerC::Broadcast(PacketWriter &pw, uint8_t channel, NetReliability r) {
    if (!Server) { return; }

    ENetPacket *pkt = MakePacket(pw.data(), pw.size(), r);
    enet_host_broadcast(Server, channel, pkt);
    enet_host_flush(Server);
}

void ServerManagerC::SendTo(ENetPeer *peer, PacketWriter &pw, uint8_t channel, NetReliability r) {
    if (!peer) { return; }
    ENetPacket *pkt = MakePacket(pw.data(), pw.size(), r);
    enet_peer_send(peer, channel, pkt);
    enet_host_flush(peer->host);
}

void ServerManagerC::OnConnect(ENetPeer *peer) {
    Peers.push_back(peer);
    if (Events.OnConnect) {
        Events.OnConnect(peer);
    }
}

void ServerManagerC::OnReceive(ENetPeer *peer, ENetPacket *packet) {
    if (Events.OnReceive) {
        Events.OnReceive(peer,packet);
    }
    PacketReader r(packet->data, packet->dataLength);
    std::string msg = r.readString();
    std::cout << "[ServerC] Received message from peer " << peer->address.host << ":" << peer->address.port << " - " << msg << std::endl;
}

void ServerManagerC::OnDisconnect(ENetPeer *peer) {
    auto it = std::find(Peers.begin(), Peers.end(), peer);
    if (it != Peers.end()) { Peers.erase(it); }
    if (Events.OnDisconnect) {
        Events.OnDisconnect(peer);
    }
}
