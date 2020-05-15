#include "Application.h"

#include <Windows.h>
#include "keep/Types.h"

using namespace Citadel::Castle;
using Citadel::Keep::MakeSPtr;
using Citadel::Keep::SPtr;

handle Citadel::Castle::Application::impl= nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Game* game = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (message) {
		case WM_CREATE: {
			LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
		} return 0;

		case WM_KEYDOWN: {

		} return 0;

		case WM_PAINT: {
			game->Update();
			game->Render();
		} return 0;

		case WM_DESTROY: {
			PostQuitMessage(0);
		} return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

int
Citadel::Castle::Application::Run(Game* game) {
	//FreeConsole();
	HINSTANCE instance = GetModuleHandle(0);

	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = static_cast<HINSTANCE>(instance);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = L"CitadelClass";
	RegisterClassEx(&windowClass);

	RECT windowRect = { 0, 0, 1280, 720 };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowEx(
		0L,
		windowClass.lpszClassName,
		L"Citadel",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		windowClass.hInstance,
		game
		);
	impl = MakeSPtr<HWND>(hwnd);

	game->Setup(impl);

	ShowWindow(hwnd, SW_SHOW);

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	game->Shutdown();

	delete game;

	return static_cast<char>(msg.wParam);
}