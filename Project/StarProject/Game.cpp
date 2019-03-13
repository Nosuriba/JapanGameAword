#include <DxLib.h>
#include "Game.h"

std::unique_ptr<Game, Game::GameDeleter> Game::s_Instance(new Game());

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	DxLib::ChangeWindowMode(TRUE);
	DxLib::SetGraphMode(640, 640, 32);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("ñºèÃñ¢ê›íËÇæÇº(ÅL•É÷•`)");
	if (DxLib_Init() == -1)
	{
		return;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);

}

void Game::Run()
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

	}
}

void Game::End()
{
	DxLib_End();
}
