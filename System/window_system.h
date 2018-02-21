#pragma once

#include <ecs.h>

namespace window
{
	constexpr char * name_ = "GameTitle";
	namespace size
	{
		template<class T> constexpr T width_ = 1280;
		template<class T> constexpr T height_ = 720;
	}
}

float operator""w(long double n)
{
	return static_cast<float>(n * window::size::width_<float>);
}
float operator""h(long double n)
{
	return static_cast<float>(n * window::size::height_<float>);
}

class WindowSystem final: public System
{
public:
	HWND hwnd_;

public:
	WindowSystem(void)
	{
		WNDCLASSEX  wc;
		memset(&wc, 0, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = GetModuleHandleA(nullptr);
		wc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
		wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wc.lpszClassName = window::name_;
		wc.hIconSm = LoadIconA(nullptr, IDI_APPLICATION);
		RegisterClassExA(&wc);

		RECT rc = { 0, 0, window::size::width_<long>, window::size::height_<long> };
		AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, true, 0);

		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;

		RECT rc_desk;
		GetWindowRect(GetDesktopWindow(), &rc_desk);

		int x = rc_desk.right / 2 - w / 2;
		int y = rc_desk.bottom / 2 - h / 2;

		this->hwnd_ = CreateWindowExA(0, window::name_,
			window::name_, WS_OVERLAPPEDWINDOW,
			x, y, w, h, 0, 0, GetModuleHandleA(nullptr), 0);

		ShowWindow(this->hwnd_, SW_SHOW);
		UpdateWindow(this->hwnd_);
	}

public:
	static LRESULT __stdcall WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (iMsg)
		{
		case WM_KEYDOWN:if (wParam == VK_ESCAPE) PostQuitMessage(0); break;
		case WM_DESTROY:PostQuitMessage(0); break;
		}
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}

private:
	void $Begin(void) override {}
	void $Run(void) override
	{
		MSG msg = { 0 };
		memset(&msg, 0, sizeof(msg));

		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
			if (msg.message == WM_QUIT) Game::ShutDown();
		}
	}
	void $End(void) override {}
};