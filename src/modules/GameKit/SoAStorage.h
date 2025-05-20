//
// Created by lclcl on 25-5-20.
//
#pragma once
#include <vector>
#include <tuple>
#include <optional>
#include <cstdint>
#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>

using SoAHandle = std::uint32_t;

template<typename... ComponentTypes>
class SoAStorage {
    // 每个组件使用 unique_ptr 存储，确保自动释放
    using ComponentStorage = std::tuple<std::vector<std::unique_ptr<ComponentTypes> >...>;

    ComponentStorage dataColumns;
    std::vector<bool> actives;
    std::vector<SoAHandle> freeIndices;

    // 获取某个字段下的 unique_ptr<T>&
    template<typename T>
    std::unique_ptr<T> &getAt(SoAHandle index) {
        return std::get<std::vector<std::unique_ptr<T> > >(dataColumns)[index];
    }

    // 设置某个索引下的所有字段
    void setTupleAt(SoAHandle index, std::unique_ptr<ComponentTypes> &&... components) {
        ((std::get<std::vector<std::unique_ptr<ComponentTypes> > >(dataColumns)[index] = std::move(components)), ...);
    }

public:
    // 添加节点并返回句柄，必须传入指针（或 std::make_unique）
    SoAHandle Add(std::unique_ptr<ComponentTypes> &&... components) {
        SoAHandle index;

        if (!freeIndices.empty()) {
            index = freeIndices.back();
            freeIndices.pop_back();
            setTupleAt(index, std::move(components)...);
            actives[index] = true;
        } else {
            index = static_cast<SoAHandle>(std::get<0>(dataColumns).size());
            (std::get<std::vector<std::unique_ptr<ComponentTypes> > >(dataColumns).emplace_back(std::move(components)),
                ...);
            actives.push_back(true);
        }

        return index;
    }

    // 删除节点，自动释放所有 unique_ptr 管理的对象
    void Remove(SoAHandle handle) {
        assert(handle < actives.size());
        actives[handle] = false;
        (std::get<std::vector<std::unique_ptr<ComponentTypes> > >(dataColumns)[handle].reset(), ...);
        freeIndices.push_back(handle);
    }

    // 遍历所有活跃节点，传入每个字段的裸引用
    void ForEachActive(const std::function<void(SoAHandle, ComponentTypes &...)> &fn) {
        for (SoAHandle i = 0; i < actives.size(); ++i) {
            if (!actives[i]) continue;
            fn(i, *getAt<ComponentTypes>(i)...);
        }
    }

    // 获取某个字段的引用（解引用 unique_ptr）
    template<typename T>
    T &Get(SoAHandle handle) {
        assert(handle < actives.size() && actives[handle]);
        return *std::get<std::vector<std::unique_ptr<T> > >(dataColumns)[handle];
    }

    // 获取某个字段的 const 引用
    template<typename T>
    const T &GetConst(SoAHandle handle) const {
        return *std::get<std::vector<std::unique_ptr<T> > >(dataColumns)[handle];
    }

    // 清空所有数据但保留已分配的内存容量
    void Clear() {
        for (SoAHandle i = 0; i < actives.size(); ++i) {
            (std::get<std::vector<std::unique_ptr<ComponentTypes>>>(dataColumns)[i].reset(), ...);
        }
        actives.clear();
        freeIndices.clear();
    }

    // 清空所有数据并释放内存
    void Reset() {
        (std::get<std::vector<std::unique_ptr<ComponentTypes>>>(dataColumns).clear(), ...);
        actives.clear();
        actives.shrink_to_fit();
        freeIndices.clear();
        freeIndices.shrink_to_fit();
    }
};
