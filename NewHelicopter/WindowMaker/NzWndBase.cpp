#include "pch.h"
#include <iostream>


//윈도우 프로시저
LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{

	//[CHECK] #3-Ask NzWndBase의 protected 멤버변수에 접근 가능한 이유는?
	switch (msg)
	{
	case WM_SIZE:
	{
		NzWndBase* pNzWnd = (NzWndBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (pNzWnd)pNzWnd->OnResize(LOWORD(lparam), HIWORD(lparam));
		break;
	}

	case WM_CLOSE:
	{
		NzWndBase* pNzWnd = (NzWndBase*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (pNzWnd) pNzWnd->OnClose();
		PostQuitMessage(0);
		break;
	}


	default:
		return::DefWindowProc(hwnd, msg, wparam, lparam);
	}//switch

	return NULL;
}


// 윈도우 생성 함수
bool NzWndBase::Create(const wchar_t* className, const wchar_t* windowName, int width, int height)
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = className;
	wc.lpfnWndProc = NzWndProc; // //[CHECK] #5. 윈도우 프로시저(함수)의 포인터 등록


	ATOM classId = 0;
	if (!GetClassInfoEx(HINSTANCE(), className, &wc))
	{
		classId = RegisterClassEx(&wc);

		if (0 == classId) return false;
	}
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	m_width = screenWidth / 2;
	m_height = screenHeight;

	int posX = (screenWidth - m_width) / 2;
	int posY = (screenHeight - m_height) / 2;


	RECT rc = { 0, 0, m_width, m_height };
	//AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	//[2025-04-22] 리사이즈/최대화 막음
	AdjustWindowRect(&rc,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);

	//[CHECK] AdjustWindowRect()의 의미는?
	m_hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, posX, posY,
		rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);



	if (NULL == m_hWnd) return false;

	::SetWindowText((HWND)m_hWnd, windowName);


	SetWindowLongPtr((HWND)m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow((HWND)m_hWnd, SW_SHOW);
	UpdateWindow((HWND)m_hWnd);

	return true;
}

//윈도우 삭제 + 핸들 반환
void NzWndBase::Destroy()
{
	if (NULL != m_hWnd)
	{
		DestroyWindow((HWND)m_hWnd);
		m_hWnd = NULL;
	}
}

//사이즈 재정의
void NzWndBase::OnResize(int width, int height)
{
	m_width = width;
	m_height = height;
}
