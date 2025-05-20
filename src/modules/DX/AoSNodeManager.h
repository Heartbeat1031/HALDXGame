//
// Created by lclcl on 25-5-20.
//
#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>
#include <cassert>

struct Node {
    virtual ~Node() = default;
    virtual void Update() = 0;
};

class AoSNodeManager {
public:
    using Handle = std::uint32_t;

    Handle Add(std::unique_ptr<Node> node);
    void Remove(Handle handle);
    void ForEachActive(const std::function<void(Node&)>& fn);
    Node* Get(Handle handle);
    size_t ActiveCount() const;

private:
    std::vector<std::unique_ptr<Node>> pool;
    std::vector<bool> actives;
    std::vector<Handle> freeIndices;
};
