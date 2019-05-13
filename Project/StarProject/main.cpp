#include "Game.h"
#include <DxLib.h>

int main()
{
	Game::GetInstance().Init();
	Game::GetInstance().Run();
	Game::GetInstance().End();

	return 0;
}