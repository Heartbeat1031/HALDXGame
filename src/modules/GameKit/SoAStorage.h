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
    // 添加节点 并返回其唯一标识符
    // コンポーネントを追加し、その一意の識別子を返します
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

    // 删除节点
    // コンポーネントを削除します
    void Remove(UID handle) {
        assert(handle < actives.size());
        actives[handle] = false;
        dataColumn[handle].reset();
        freeIndices.push_back(handle);
    }

    // 获取节点的引用
    // コンポーネントを取得します
    T &Get(UID handle) {
        assert(handle < actives.size() && actives[handle]);
        return *dataColumn[handle];
    }

    // 获取节点的常量引用
    // コンポーネントの定数参照を取得します
    const T &GetConst(UID handle) const {
        assert(handle < actives.size() && actives[handle]);
        return *dataColumn[handle];
    }

    // 遍历节点
    // アクティブなコンポーネントを走査します
    void ForEachActive(const std::function<void(UID, T &)> &fn) {
        for (UID i = 0; i < actives.size(); ++i) {
            if (!actives[i]) continue;
            fn(i, *dataColumn[i]);
        }
    }

    // 清空存储 不释放内存空间
    // ストレージをクリアしますが、メモリは解放しません
    void Clear() {
        for (auto &ptr: dataColumn) {
            ptr.reset();
        }
        actives.clear();
        freeIndices.clear();
    }

    // 清空存储 并释放内存空间
    // ストレージをクリアし、メモリを解放します
    void Reset() {
        dataColumn.clear();
        actives.clear();
        actives.shrink_to_fit();
        freeIndices.clear();
        freeIndices.shrink_to_fit();
    }
};
