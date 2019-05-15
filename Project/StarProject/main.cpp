#include "Game.h"
#include <DxLib.h>

#ifdef _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
#endif
{
	Game::GetInstance().Init();
	Game::GetInstance().Run();
	Game::GetInstance().End();

	return 0;
}