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

using UID = std::uint32_t;

template<typename... ComponentTypes>
class SoAStorage {
    // 各コンポーネントは unique_ptr で管理し、自動的に解放されるようにします。
    using ComponentStorage = std::tuple<std::vector<std::unique_ptr<ComponentTypes> >...>;

    ComponentStorage dataColumns;
    std::vector<bool> actives;
    std::vector<UID> freeIndices;

    // 特定のメンバ変数の unique_ptr<T>& を取得する
    template<typename T>
    std::unique_ptr<T> &getAt(UID index) {
        return std::get<std::vector<std::unique_ptr<T> > >(dataColumns)[index];
    }

    // 特定のインデックスに対応するすべてのフィールドを設定する
    void setTupleAt(UID index, std::unique_ptr<ComponentTypes> &&... components) {
        ((std::get<std::vector<std::unique_ptr<ComponentTypes> > >(dataColumns)[index] = std::move(components)), ...);
    }

public:
    // ノードを追加してハンドルを返します。引数にはポインタ（または std::make_unique）で渡す必要があります。
    UID Add(std::unique_ptr<ComponentTypes> &&... components) {
        UID index;

        if (!freeIndices.empty()) {
            index = freeIndices.back();
            freeIndices.pop_back();
            setTupleAt(index, std::move(components)...);
            actives[index] = true;
        } else {
            index = static_cast<UID>(std::get<0>(dataColumns).size());
            (std::get<std::vector<std::unique_ptr<ComponentTypes> > >(dataColumns).emplace_back(std::move(components)),
                ...);
            actives.push_back(true);
        }

        return index;
    }

    // 指定ノードを削除し、それに関連付けられた unique_ptr による全オブジェクトを自動的に解放します。
    void Remove(UID handle) {
        assert(handle < actives.size());
        actives[handle] = false;
        (std::get<std::vector<std::unique_ptr<ComponentTypes> > >(dataColumns)[handle].reset(), ...);
        freeIndices.push_back(handle);
    }

    // アクティブなすべてのノードを対象に、各フィールド（T& 型）への生の参照をコールバック関数に渡します。
    void ForEachActive(const std::function<void(UID, ComponentTypes &...)> &fn) {
        for (UID i = 0; i < actives.size(); ++i) {
            if (!actives[i]) continue;
            fn(i, *getAt<ComponentTypes>(i)...);
        }
    }

    // unique_ptr をデリファレンスして、特定のフィールドの参照（T&）を取得する
    template<typename T>
    T &Get(UID handle) {
        assert(handle < actives.size() && actives[handle]);
        return *std::get<std::vector<std::unique_ptr<T> > >(dataColumns)[handle];
    }

    // フィールドの const 参照を取得
    template<typename T>
    const T &GetConst(UID handle) const {
        return *std::get<std::vector<std::unique_ptr<T> > >(dataColumns)[handle];
    }

    // 全データを削除しますが、確保済みのメモリ容量（capacity）はそのまま保持されます。
    void Clear() {
        for (UID i = 0; i < actives.size(); ++i) {
            (std::get<std::vector<std::unique_ptr<ComponentTypes> > >(dataColumns)[i].reset(), ...);
        }
        actives.clear();
        freeIndices.clear();
    }

    // 保持している全データを削除し、確保していたメモリも解放します。
    void Reset() {
        (std::get<std::vector<std::unique_ptr<ComponentTypes> > >(dataColumns).clear(), ...);
        actives.clear();
        actives.shrink_to_fit();
        freeIndices.clear();
        freeIndices.shrink_to_fit();
    }
};
