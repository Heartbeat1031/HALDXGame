#pragma once
#include	<cinttypes>

constexpr uint32_t BOSSWIDTH= 40;
constexpr uint32_t BOSSHEIGHT=10;

constexpr uint32_t TentacleNum =8;


void initboss();
void disposeboss();
void updateboss();
void drawboss();
