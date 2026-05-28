#pragma once

#include "GameObject.h"

namespace learning {
    struct Vector2f;
}

class Player : public GameObject{
public: 
    Player(const GameObject&) = delete;
    Player(ObjectType type) : GameObject(type) {}
    ~Player() override;
    learning::Vector2f GetUpDir();
    void Update(float deltaTime);
    void Render(MyRender& render) override;
    void Move(float deltaTime) override;
    void Accelat(float deltaTime);
    void DeAccelat(float deltaTime);
    void SetDetector(float radius);
    learning::ColliderCircle* GetDetector();
private:
    learning::Vector2f UpDir;
    float accel=0;
    float elapsedTime = 0.0f;
    const float MaxSpeed = 0.5f;
    const float MaxAccel= 0.001f;
    const float Gravity= 0.0005f;
    float m_verticalSpeed = 0.0f;
    learning::ColliderCircle* detector = nullptr;
};

