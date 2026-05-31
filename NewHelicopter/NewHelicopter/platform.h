#pragma once

#include "GameObject.h"
#include "Collider.h"

class Player;

class Platform : public GameObject {
public:
    Platform(const GameObject&) = delete;
    Platform(ObjectType type) : GameObject(type) { }
    ~Platform() override;
    void Move(float deltaTime) override;
    void Update(float deltaTime) {};
    void Push();
    bool CheckIsAbove(Player* player, learning::ColliderBox* playerBox);
    Player* bumpedPlayer = nullptr;
    learning::ColliderBox* platfCol = nullptr;
    learning::ColliderBox* playerfCol = nullptr;
};