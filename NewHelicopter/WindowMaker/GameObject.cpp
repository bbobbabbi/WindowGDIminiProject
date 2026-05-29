#pragma once
#include "pch.h"
#include "MyRender.h"
#include "renderHelp.h"
#include "GameObject.h"
#include <assert.h>


GameObject::~GameObject()
{
    if (myCollider)
    {
        delete myCollider;
        myCollider = nullptr;
    }
}

void GameObject::Update(float deltaTime)
{
    Move(deltaTime);
    //UpdateFrame(deltaTime);
    // Collider 업데이트
    if (myCollider)
    {
        myCollider->center = m_pos;
    }
}

void GameObject::Render(MyRender& render)
{
    render.DrawCollider(myCollider);
}


void GameObject::SetColliderCircle(float radius)
{
    if (myCollider)
    {
        delete myCollider;
        myCollider = nullptr;
    }

    learning::ColliderCircle* circleP = new learning::ColliderCircle;
    myCollider = dynamic_cast<learning::Collider*>(circleP);

    assert(circleP != nullptr && "Failed to create ColliderCircle!");
    assert(myCollider != nullptr && "Failed to create ColliderCircle!");

    circleP->radius = radius;
    circleP->center = m_pos;
}



void GameObject::SetColliderBox(float width, float height)
{
    if (myCollider)
    {
        delete myCollider;
        myCollider = nullptr;
    }

    learning::ColliderBox* boxP = new learning::ColliderBox;
    myCollider = dynamic_cast<learning::Collider*>(boxP);

    assert(boxP != nullptr && "Failed to create ColliderBox!");
    assert(myCollider != nullptr && "Failed to create ColliderBox!");

    boxP->center = m_pos;
    boxP->halfSize.x = width / 2.0f;
    boxP->halfSize.y = height / 2.0f;
}

//
//
//void GameObject::SetBitmapInfo(BitmapInfo* bitmapInfo) {
//    assert(m_pBitmapInfo == nullptr && "BitmapInfo must be null!");
//
//    m_pBitmapInfo = bitmapInfo;
//    // 스프라이트 정보는 일단은 하드코딩해요. 
//  // 일단, 프레임 크기와 시간이 같다고 가정합니다.
//    m_frameWidth = m_pBitmapInfo->GetWidth() / 5;
//    m_frameHeight = m_pBitmapInfo->GetHeight() / 3;
//    m_frameIndex = 0;
//
//    for (int i = 0; i < 5; ++i)
//    {
//        m_frameXY[i].x = i * m_frameWidth;
//        m_frameXY[i].y = 0;
//    }
//
//    for (int i = 0; i < 5; ++i)
//    {
//        m_frameXY[i + 5].x = i * m_frameWidth;
//        m_frameXY[i + 5].y = m_frameHeight;
//    }
//
//    for (int i = 0; i < 4; ++i)
//    {
//        m_frameXY[i + 10].x = i * m_frameWidth;
//        m_frameXY[i + 10].y = m_frameHeight * 2;
//    }
//}
//
//void GameObject::DrawBitmap(HDC hdc)
//{
//    if (m_pBitmapInfo == nullptr) return;
//    if (m_pBitmapInfo->GetBitmapHandle() == nullptr) return;
//
//    HDC hBitmapDC = CreateCompatibleDC(hdc);
//
//    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, m_pBitmapInfo->GetBitmapHandle());
//    // BLENDFUNCTION 설정 (알파 채널 처리)
//    BLENDFUNCTION blend = { 0 };
//    blend.BlendOp = AC_SRC_OVER;
//    blend.SourceConstantAlpha = 255;  // 원본 알파 채널 그대로 사용
//    blend.AlphaFormat = AC_SRC_ALPHA;
//
//    SetWidthAndHeight(1.1f, 1.1f);
//
//    const int x = m_pos.x - m_width / 2;
//    const int y = m_pos.y - m_height / 2;
//
//    const int srcX = m_frameXY[m_frameIndex].x;
//    const int srcY = m_frameXY[m_frameIndex].y;
//
//    //실제 비트맵에 그림
//    AlphaBlend(hdc, x, y, m_width, m_height,
//        hBitmapDC, srcX, srcY, m_frameWidth, m_frameHeight, blend);
//
//    // 비트맵 핸들 복원
//    SelectObject(hBitmapDC, hOldBitmap);
//    DeleteDC(hBitmapDC);
//
//}
//
//// 가져오는 그림 처리 
//void GameObject::UpdateFrame(float deltaTime)
//{
//    m_frameTime += deltaTime;
//    if (m_frameTime >= m_frameDuration)
//    {
//        m_frameTime = 0.0f;
//        m_frameIndex = (m_frameIndex + 1) % (m_frameCount);
//    }
//}



void GameObject::SetWidthAndHeight(float width, float height) {
    GameObject::m_width = 100 * width;
    GameObject::m_height = 100 * height;
}


learning::Collider* GameObject::GetCollider() {

    if (myCollider) {
        return myCollider;
    }
}


void  GameObject::Move(float deltaTime) 
{
    GameObjectBase::Move(deltaTime);
}

void GameObjectBase::SetName(const char* name)
{
    if (name == nullptr) return;

    strncpy_s(m_name, name, OBJECT_NAME_LEN_MAX - 1);
    m_name[OBJECT_NAME_LEN_MAX - 1] = '\0';
}