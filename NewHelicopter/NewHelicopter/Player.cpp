#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Utillity.h"
#include "MyRender.h"
#include <iostream>


learning::Vector2f Player::GetUpDir(){
	

    return (m_pos + learning::Vector2f(0, -1) - m_pos).Normalized();
}


void Player::Move(float deltaTime) {
	m_speed += (accel - Gravity) * deltaTime; 

	if (m_speed > MaxSpeed) { m_speed = MaxSpeed; }

	float distance = m_speed * deltaTime; 
	
	if (m_dir.y == 0) m_dir = learning::Vector2f(0,-1);
	m_pos += m_dir * distance;
}

void Player::Update(float deltaTime)
{
	Move(deltaTime);

	if (myCollider)
	{
		myCollider->center = m_pos;
	}
	if (detector) {
		detector->center = m_pos;
	}
}

void Player::Accelat(float deltaTime) {
	
	accel += deltaTime * 0.01f;

	// 최대값 제한
	if (accel > MaxAccel)
	{
		accel = MaxAccel;
	}
	std::cout << accel << std::endl;
}

void Player::DeAccelat(float deltaTime) {
	accel -= deltaTime * 0.001f;

	if (accel < 0.0f)
	{
		accel = 0.0f;
	}
}

void Player::SetDetector(float radius)
{
	if (detector)
	{
		delete detector;
		detector = nullptr;
	}

	learning::ColliderCircle* circleP = new learning::ColliderCircle;
	detector = dynamic_cast<learning::ColliderCircle*>(circleP);

	circleP->center = m_pos;
	circleP->radius = radius;
}

learning::ColliderCircle* Player::GetDetector()
{
	return detector;
}



void Player::Render(MyRender& render) {
	render.DrawCollider(myCollider);
	render.DrawCollider(detector);
}

Player::~Player() {
	delete detector;
	detector = nullptr;
}