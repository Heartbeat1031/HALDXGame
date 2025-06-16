#pragma once
#include <random>

class RandomEngine final {
private:
    std::mt19937 m_engine;
    std::uniform_int_distribution<int> m_dirDist{-1, 1};
    std::uniform_int_distribution<int> m_diceDist{1, 6};
public:
    explicit RandomEngine(unsigned int seed = std::random_device{}());

    [[nodiscard]] int GetInt(int min, int max);
    [[nodiscard]] float GetFloat(float min, float max);
    [[nodiscard]] int RollDice();
    [[nodiscard]] int RandomDirection();
    [[nodiscard]] bool IsCriticalHit(float chance);
};
