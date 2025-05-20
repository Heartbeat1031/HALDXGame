//
// Created by lclcl on 25-5-20.
//
#include "AoSNodeManager.h"

AoSNodeManager::Handle AoSNodeManager::Add(std::unique_ptr<Node> node) {
    if (!freeIndices.empty()) {
        Handle idx = freeIndices.back();
        freeIndices.pop_back();
        pool[idx] = std::move(node);
        actives[idx] = true;
        return idx;
    }
    pool.emplace_back(std::move(node));
    actives.push_back(true);
    return static_cast<Handle>(pool.size() - 1);
}

void AoSNodeManager::Remove(Handle handle) {
    assert(handle < actives.size());
    actives[handle] = false;
    pool[handle].reset();
    freeIndices.push_back(handle);
}

void AoSNodeManager::ForEachActive(const std::function<void(Node&)>& fn) {
    for (size_t i = 0; i < pool.size(); ++i) {
        if (actives[i] && pool[i]) {
            fn(*pool[i]);
        }
    }
}

Node* AoSNodeManager::Get(Handle handle) {
    if (handle >= pool.size() || !actives[handle]) {
        return nullptr;
    }
    return pool[handle].get();
}

size_t AoSNodeManager::ActiveCount() const {
    return pool.size() - freeIndices.size();
}
