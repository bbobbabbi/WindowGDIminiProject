#pragma once
#include "pch.h"
#include "MyRender.h"
#include "renderHelp.h"
#include "Utillity.h"
#include "GameObject.h"


MyRender* MyRender::Instance = nullptr;

bool MyRender::InitMyRender(HWND hd) {
	RECT rcClient = {};
	GetClientRect(hd, &rcClient);
	m_width = rcClient.right - rcClient.left;
	m_height = rcClient.bottom - rcClient.top;

	m_hFrontDC = GetDC(hd);
	m_hBackDC = CreateCompatibleDC(m_hFrontDC);
	m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

	m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

	if (m_hFrontDC == nullptr || m_hBackDC == nullptr || m_hBackBitmap == nullptr || m_hDefaultBitmap == nullptr)
		return false;
	else return true;
}

void MyRender::Render(int drawCount, GameObjectBase** drawTargets) {
	::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

	//메모리 DC에 그리기
	DrawGameObject(drawCount, drawTargets);


	//메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
	BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

void MyRender::DrawGameObject(int drawCount, GameObjectBase** drawTargets){
	for (int i = 0; i < drawCount; i++)
	{
		if (drawTargets[i])
		{
			drawTargets[i]->Render(*Instance);
		}
		else {
			break;
		}
	}
}



void MyRender::DrawBitmap(renderHelp::BitmapInfo* m_pBitmapInfo,float m_width,float m_height , learning::Vector2f m_pos, MyRender::FrameFPos* m_frameXY,int m_frameIndex,int m_frameWidth,int m_frameHeight)
{
	if (m_pBitmapInfo == nullptr) return;
	if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;

	HDC hBitmapDC = CreateCompatibleDC(m_hBackDC);

	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
	// BLENDFUNCTION 설정 (알파 채널 처리)
	BLENDFUNCTION blend = { 0 };
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;  // 원본 알파 채널 그대로 사용
	blend.AlphaFormat = AC_SRC_ALPHA;


	const int x = m_pos.x - m_width / 2;
	const int y = m_pos.y - m_height / 2;

	const int srcX = m_frameXY[m_frameIndex].x;
	const int srcY = m_frameXY[m_frameIndex].y;

	//실제 비트맵에 그림
	AlphaBlend(m_hBackDC, x, y, m_width, m_height,
		hBitmapDC, srcX, srcY, m_frameWidth, m_frameHeight, blend);

	// 비트맵 핸들 복원
	SelectObject(hBitmapDC, hOldBitmap);
	DeleteDC(hBitmapDC);

}

void MyRender::DrawCollider(learning::Collider* myCollider)
{
	HPEN hPenR = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HPEN hPenB = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	HPEN hOldPen = (HPEN)SelectObject(m_hBackDC, hPenR); // 빨간팬 선택 기존 팬 들어있음
	HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hBackDC, GetStockObject(NULL_BRUSH));

	if (myCollider->isPlayerIntersect) {
		SelectObject(m_hBackDC, hPenB);
	}
	else {
		SelectObject(m_hBackDC, hPenR);
	}

	myCollider->Draw(*Instance);

	// 이전 객체 복원 및 펜 삭제
	SelectObject(m_hBackDC, hOldPen);
	SelectObject(m_hBackDC, hOldBrush);
	DeleteObject(hPenR);
	DeleteObject(hPenB);
}

void MyRender::DrawCircleCollider(learning::ColliderCircle* col) {
	Ellipse(m_hBackDC, col->center.x - col->radius,
		col->center.y - col->radius,
		col->center.x + col->radius,
		col->center.y + col->radius);
}

void MyRender::DrawBoxCollider(learning::ColliderBox* col) {
	Rectangle(m_hBackDC, col->center.x - col->halfSize.x,
		col->center.y - col->halfSize.y,
		col->center.x + col->halfSize.x,
		col->center.y + col->halfSize.y);
}


void MyRender::OnResize(int width, int height)
{
	m_width = width;
	m_height = height;
	m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

	HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

	DeleteObject(hPrevBitmap);
}

void MyRender::OnClose(HWND hd)
{

	SelectObject(m_hBackDC, m_hDefaultBitmap);

	DeleteObject(m_hBackBitmap);
	DeleteDC(m_hBackDC);

	ReleaseDC(hd, m_hFrontDC);
}


