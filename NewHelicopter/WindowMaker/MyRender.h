#pragma once
#include "pch.h"
#include "RenderHelp.h"
#include <vector>

class GameObjectBase;
namespace learning {
	struct Vector2f;
	struct Collider;
	struct ColliderCircle;
	struct ColliderBox;
}



class MyRender
{

public:
	struct FrameFPos
	{
		int x;
		int y;
	};
	MyRender() {
		Instance = this;
	};
	~MyRender(){
		if (Instance != nullptr) {
			Instance = nullptr;
		}

	}
	HWND mhWnd;
	HDC m_hFrontDC = nullptr;
	HDC m_hBackDC = nullptr;
	HBITMAP m_hBackBitmap = nullptr;
	HBITMAP m_hDefaultBitmap = nullptr;
	int m_width, m_height;
	renderHelp::Camera m_camera;
	static MyRender* Instance;
	
public:
	bool InitMyRender(HWND hd);
	void Render(int drawCount, std::vector<GameObjectBase*> drawTargets);
	void DrawCollider(learning::Collider* myCollider);
	void DrawGameObject(int drawCount, std::vector<GameObjectBase*> drawTargets);
	void DrawCircleCollider(learning::ColliderCircle* col);
	void DrawBoxCollider(learning::ColliderBox* col);
	void OnResize(int width, int height);
	void DrawBitmap(renderHelp::BitmapInfo* m_pBitmapInfo, float m_width, float m_height, learning::Vector2f m_Wpos, FrameFPos* m_frameXY, int m_frameIndex, int m_frameWidth, int m_frameHeight);
	void OnClose(HWND hd);
	learning::Vector2f GetCameraPos();
};