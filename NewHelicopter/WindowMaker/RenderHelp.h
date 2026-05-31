#pragma once
#include "Windows.h"


namespace renderHelp
{
    struct WICInitializer;
    //이미지는 png지만 그릴려면 비트맵 구조로 변환해줘야 함
    class BitmapInfo
    {
    public:
        HBITMAP GetBitmapHandle() const { return m_hBitmap; }
        int GetWidth() const { return m_width; }
        int GetHeight() const { return m_height; }

    private:
        friend struct WICInitializer;

        BitmapInfo() = default;

        BitmapInfo(HBITMAP hBitmap)
        {
            m_hBitmap = hBitmap;
            BITMAP bitmap;
            GetObject(hBitmap, sizeof(BITMAP), &bitmap);
            m_width = bitmap.bmWidth;
            m_height = bitmap.bmHeight;
        }

        ~BitmapInfo()
        {
            if (m_hBitmap)
            {
                DeleteObject(m_hBitmap);
                m_hBitmap = nullptr;
            }
        }

    private:
        HBITMAP m_hBitmap = nullptr;

        int m_width = 0;
        int m_height = 0;

        BitmapInfo(const BitmapInfo&) = delete;
        BitmapInfo& operator=(const BitmapInfo&) = delete;
    };

    struct Camera
    {
        learning::Vector2f pos{ 0,0 };
        int width = 0;
        int height = 0;

        // 기존 기준점
        float focusX = 0;
        float focusY = 0;

        // 데드존 크기
        float deadZoneW = 400.0f;
        float deadZoneH = 400.0f;

        // 현재 플레이어가 화면에서 보이는 위치
        float targetScreenX = 0;
        float targetScreenY = 0;

        float deadLeft = 0;
        float deadRight = 0;
        float deadTop = 0;
        float deadBottom = 0;

        void Follow(const learning::Vector2f& targetPos, float screenSizeX, float screenSizeY)
        {
            width = screenSizeX;
            height = screenSizeY;

            focusX = width / 2.0f;
            focusY = height * 2.5f / 4.0f;

            targetScreenX = targetPos.x - pos.x;
            targetScreenY = targetPos.y - pos.y;

            deadLeft = focusX - deadZoneW / 2.0f;
            deadRight = focusX + deadZoneW / 2.0f;
            deadTop = focusY - deadZoneH / 2.0f;
            deadBottom = focusY + deadZoneH / 2.0f;

            if (targetScreenX < deadLeft)
            {
                pos.x = targetPos.x - deadLeft;
            }
            else if (targetScreenX > deadRight)
            {
                pos.x = targetPos.x - deadRight;
            }

            if (targetScreenY < deadTop)
            {
                pos.y = targetPos.y - deadTop;
            }
            else if (targetScreenY > deadBottom)
            {
                pos.y = targetPos.y - deadBottom;
            }
        }
    };
    //파일명으로 비트맵info 클래스 만들기
    BitmapInfo* CreateBitmapInfo(LPCWSTR filename);
}
