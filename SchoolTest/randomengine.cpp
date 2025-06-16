#include "randomengine.h"

RandomEngine::RandomEngine(unsigned int seed) : m_engine(seed) {}

int RandomEngine::GetInt(int min, int max) {
    return std::uniform_int_distribution<int>(min, max)(m_engine);
}

float RandomEngine::GetFloat(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(m_engine);
}

int RandomEngine::RollDice() {
    return m_diceDist(m_engine);
}

int RandomEngine::RandomDirection() {
    return m_dirDist(m_engine);
}

bool RandomEngine::IsCriticalHit(float chance) {
    return GetFloat(0.0f, 1.0f) < chance;
}
