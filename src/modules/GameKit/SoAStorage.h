//
// Created by lclcl on 25-5-20.
//
#pragma once
#include <vector>
#include <optional>
#include <cstdint>
#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>

using UID = std::uint32_t;

// SoAStorage是一个可以支持快速添加、快速删除和快速遍历的多态存储容器.
// SoAStorageは、迅速な追加、削除、および多態的な走査をサポートするストレージコンテナです。
template<typename T>
class SoAStorage {
    std::vector<std::unique_ptr<T> > dataColumn;
    std::vector<bool> actives;
    std::vector<UID> freeIndices;

public:
    UID Add(std::unique_ptr<T> component) {
        UID index;
        if (!freeIndices.empty()) {
            index = freeIndices.back();
            freeIndices.pop_back();
            dataColumn[index] = std::move(component);
            actives[index] = true;
        } else {
            index = static_cast<UID>(dataColumn.size());
            dataColumn.emplace_back(std::move(component));
            actives.push_back(true);
        }
        return index;
    }

    void Remove(UID handle) {
        assert(handle < actives.size());
        actives[handle] = false;
        dataColumn[handle].reset();
        freeIndices.push_back(handle);
    }

    T &Get(UID handle) {
        assert(handle < actives.size() && actives[handle]);
        return *dataColumn[handle];
    }

    const T &GetConst(UID handle) const {
        assert(handle < actives.size() && actives[handle]);
        return *dataColumn[handle];
    }

    void ForEachActive(const std::function<void(UID, T &)> &fn) {
        for (UID i = 0; i < actives.size(); ++i) {
            if (!actives[i]) continue;
            fn(i, *dataColumn[i]);
        }
    }

    void Clear() {
        for (auto &ptr: dataColumn) {
            ptr.reset();
        }
        actives.clear();
        freeIndices.clear();
    }

    void Reset() {
        dataColumn.clear();
        actives.clear();
        actives.shrink_to_fit();
        freeIndices.clear();
        freeIndices.shrink_to_fit();
    }
};
