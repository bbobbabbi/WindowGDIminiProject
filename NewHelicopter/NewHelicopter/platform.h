#pragma once

#include "GameObject.h"

class Player;

class Platform : public GameObject {
public:
    Platform(const GameObject&) = delete;
    Platform(ObjectType type) : GameObject(type) {}
    ~Platform() override;
    void Move(float deltaTime) override;
    void Update(float deltaTime) override;
    Player* bumpedPlayer = nullptr;
    learning::ColliderBox* platfCol = nullptr;
    learning::ColliderBox* playerfCol = nullptr;
    bool isPlatformDetected = false;
};