#include "Game.h"

#include <windows.h>

void
Citadel::Castle::Game::SetWindowTitle(const wchar_t* title) {
	Citadel::Keep::SPtr<HWND> hwnd = Citadel::Keep::SPtrFromHandle<HWND>(impl);
	SetWindowText(*hwnd.get(), title);
}