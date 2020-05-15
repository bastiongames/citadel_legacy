#include <Windows.h>
#include "castle/Application.h"

using namespace Citadel::Castle;

_Use_decl_annotations_
int main() {
	Game* game = GameMaker::Get();
	return Application::Run(game);
}