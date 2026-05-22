#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Utillity.h"
#include <iostream>

learning::Vector2f Player::GetUpDir(){
	

    return (m_pos + learning::Vector2f(0, -1) - m_pos).Normalized();
}


void Player::Move(float deltaTime) {
	m_speed += (accel -Gravity)* deltaTime;

	if (m_speed > MaxSpeed) 
	{
		m_speed = MaxSpeed;
	}

	float distance = m_speed* deltaTime;

	m_pos += m_dir * distance;
}

void Player::Accelat(float deltaTime) {
	
	accel += deltaTime * 0.0001f;

	// 최대값 제한
	if (accel > MaxAccel)
	{
		accel = MaxAccel;
	}
	std::cout << accel << std::endl;
}

void Player::DeAccelat(float deltaTime) {
	accel -= deltaTime * 0.01f;

	if (accel < 0.0f)
	{
		accel = 0.0f;
	}
}