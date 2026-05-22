#pragma once
#include "pch.h"

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
	static MyRender* Instance;
	
public:
	bool InitMyRender(HWND hd);
	void Render(int drawCount, GameObjectBase** drawTargets);
	void DrawCollider(learning::Collider* myCollider);
	void DrawGameObject(int drawCount, GameObjectBase** drawTargets);
	void DrawCircleCollider(learning::ColliderCircle* col);
	void DrawBoxCollider(learning::ColliderBox* col);
	void OnResize(int width, int height);
	void OnClose(HWND hd);
};