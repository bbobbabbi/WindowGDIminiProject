#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Utillity.h"
#include "MyRender.h"
#include "RenderHelp.h"
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
	UpdateFrame(deltaTime,m_speed);
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
	std::cout << accel <<"   "<< m_speed << std::endl;
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


void Player::SetBitmapInfo(renderHelp::BitmapInfo* bitmapInfo) {
	m_pBitmapInfo = bitmapInfo;
	// 스프라이트 정보는 일단은 하드코딩해요. 
  // 일단, 프레임 크기와 시간이 같다고 가정합니다.
	m_frameWidth = m_pBitmapInfo->GetWidth() / 12;
	m_frameHeight = m_pBitmapInfo->GetHeight() / 10;
	m_frameIndex = 0;
	for (int i = 0; i < 60; ++i)
	{
		m_frameXY[i].x = (i % 10) * 200;
		m_frameXY[i].y = (i / 10) * 200;
	}
}


void Player::Render(MyRender& render) {
	render.DrawCollider(myCollider);
	render.DrawCollider(detector);
	render.DrawBitmap(m_pBitmapInfo,m_width,m_height,m_pos,m_frameXY,m_frameIndex,m_frameWidth,m_frameHeight);
}

// 가져오는 그림 처리 
void Player::UpdateFrame(float deltaTime, float speed)
{
	const float minSpeed = -0.2f;
	const float maxSpeed = 0.5f;        // 최고 속도 기준으로 수정

	const float maxFrameDuration = 50.f; // 느릴 때
	const float minFrameDuration = 10.f; // 빠를 때

	float t = (speed - minSpeed) / (maxSpeed - minSpeed);

	if (t < 0.0f)
		t = 0.0f;
	else if (t > 1.0f)
		t = 1.0f;

	m_frameDuration =
		maxFrameDuration +
		(minFrameDuration - maxFrameDuration) * t;

	if (speed <= 0)
	{
		m_frameIndex = m_startframeIndex;
		m_frameTime = 0.0f;
		return;
	}

	m_frameTime += deltaTime;

	if (m_frameTime >= m_frameDuration)
	{
		m_frameTime = 0.0f;

		int localFrame =
			(m_frameIndex - m_startframeIndex + 1) % m_frameCount;

		m_frameIndex = m_startframeIndex + localFrame;
	}
}

Player::~Player() {
	delete detector;
	detector = nullptr;
}