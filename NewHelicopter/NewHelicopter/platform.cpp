
#pragma once
#include "platform.h"
#include "Collider.h"
#include "GameObject.h"
#include "Player.h"

void Platform::Move(float deltaTime) {
	m_pos = m_pos;
}

void Platform::Update (float deltatime) {
	if (myCollider->isPlayerIntersect && bumpedPlayer != nullptr) {
		//두 y값의 합이 같을 때 까지 위쪽으로 밀어내기
	}
}

Platform::~Platform() {
	bumpedPlayer = nullptr;
}