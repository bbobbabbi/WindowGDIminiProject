
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

void Platform::Push()
{
    if (!platfCol->isPlayerIntersect || bumpedPlayer == nullptr)
        return;

    if (bumpedPlayer->GetSpeed() > 0)
        return;
    
    if (playerfCol == nullptr || platfCol == nullptr)
    {
        std::cout << "사각형 밀어내기 콜라이더 변환 실패" << std::endl;
        return;
    }

    learning::Vector2f pPos = bumpedPlayer->GetWPosition();
    learning::Vector2f tPos = m_Wpos;

    float platformTop = tPos.y - platfCol->halfSize.y;

    //완전 밀착 방지 -1
    float newPlayerY = platformTop - playerfCol->halfSize.y-1.f;

    bumpedPlayer->SetWPosition(pPos.x, newPlayerY);
    bumpedPlayer->ZeroReset();

}

bool Platform::CheckIsAbove(Player* player,learning::ColliderBox* playerBox)
{
    if (player == nullptr || playerBox == nullptr  || platfCol == nullptr)
        return false;

    if (player->GetSpeed() > 0.0f)
        return false;

    //플랫폼 정보 주입
    bumpedPlayer = player;
    playerfCol = playerBox;

    learning::Vector2f prevPos = bumpedPlayer->GetPrevWPosition();
    learning::Vector2f currPos = bumpedPlayer->GetWPosition();
    learning::Vector2f platPos = m_Wpos;


    float prevBottom = prevPos.y + playerfCol->halfSize.y;
    float currBottom = currPos.y + playerfCol->halfSize.y;

    float platTop = platPos.y - platfCol->halfSize.y;

    float playerLeft = currPos.x - playerfCol->halfSize.x;
    float playerRight = currPos.x + playerfCol->halfSize.x;

    float platLeft = platPos.x - platfCol->halfSize.x;
    float platRight = platPos.x + platfCol->halfSize.x;

    bool overlapX = playerRight > platLeft && playerLeft < platRight;

    bool crossedTop = prevBottom <= platTop && currBottom >= platTop;

    return overlapX && crossedTop;
}


Platform::~Platform() {
	bumpedPlayer = nullptr;
    playerfCol = nullptr;
    platfCol = nullptr;
}