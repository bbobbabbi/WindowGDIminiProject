
#pragma once
#include "platform.h"
#include "Collider.h"
#include "GameObject.h"
#include "Player.h"
#include "Collider.h"
#include "iostream"

void Platform::Move(float deltaTime) {
	m_Wpos = m_Wpos;
}

void Platform::Update (float deltatime) {
    if (!myCollider->isPlayerIntersect || bumpedPlayer == nullptr)
        return;

    if (bumpedPlayer->GetSpeed() > 0) {
        return;
    }

    auto* pb = playerfCol;
    auto* tb = platfCol;
    
    if (pb == nullptr || tb == nullptr)
    {
        std::cout << "사각형 밀어내기 콜라이더 변환 실패" << std::endl;
        return;
    }

    learning::Vector2f pPos = bumpedPlayer->GetWPosition();
    learning::Vector2f tPos = m_Wpos;

    float playerBottom = pPos.y + pb->halfSize.y;
    float platformTop = tPos.y - tb->halfSize.y;

    float penetration = playerBottom - platformTop;

    if (penetration > 0.0f)
    {
        bumpedPlayer->ZeroReset();
        bumpedPlayer->SetWPosition(pPos.x, pPos.y - penetration);
    }   
}


Platform::~Platform() {
	bumpedPlayer = nullptr;
    playerfCol = nullptr;
    platfCol = nullptr;
}