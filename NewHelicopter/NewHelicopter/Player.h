#pragma once

#include "GameObject.h"
#include "RenderHelp.h"
#include "MyRender.h"
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
    void SetBitmapInfo(renderHelp::BitmapInfo * bitmapInfo);
    // 가져오는 그림 처리 
    void UpdateFrame(float deltaTime, float speed);

private:
    learning::Vector2f UpDir;
    float accel=0;
    float elapsedTime = 0.0f;
    const float MaxSpeed = 0.5f;
    const float MaxAccel= 0.001f;
    const float Gravity= 0.0005f;
    float m_verticalSpeed = 0.0f;
    learning::ColliderCircle* detector = nullptr;

    int m_width = 200;
    int m_height = 200;

    MyRender::FrameFPos m_frameXY[60] ={};
   
    int m_frameWidth = 200;
    int m_frameHeight = 200;
    int m_frameIndex = 1;
    int m_startframeIndex = 0;
    int m_frameCount = 59; // 프레임 수

    float m_frameTime = 0.0f;
    float m_frameDuration = 10.0f; // 임의 설정
};
