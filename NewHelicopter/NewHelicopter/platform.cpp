
#pragma once
#include "platform.h"
#include "Collider.h"
#include "GameObject.h"
#include "Player.h"
#include "Collider.h"
#include "iostream"

void Platform::Move(float deltaTime) {
	m_pos = m_pos;
}

void Platform::Update (float deltatime) {
    if (!myCollider->isPlayerIntersect || bumpedPlayer == nullptr)
        return;

    auto* pb = playerfCol;
    auto* tb = platfCol;

    if (pb == nullptr || tb == nullptr)
    {
        std::cout << "사각형 밀어내기 콜라이더 변환 실패" << std::endl;
        return;
    }

    learning::Vector2f pPos = bumpedPlayer->GetPosition();
    learning::Vector2f tPos = m_pos;

    float playerBottom = pPos.y + pb->halfSize.y;
    float platformTop = tPos.y - tb->halfSize.y;

    float penetration = playerBottom - platformTop;

    if (penetration > 0.0f)
    {
        bumpedPlayer->SetPosition(pPos.x, pPos.y - penetration);
        bumpedPlayer->SetSpeed(0);
    }
}

Platform::~Platform() {
	bumpedPlayer = nullptr;
    playerfCol = nullptr;
    platfCol = nullptr;
}