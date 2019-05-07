#include <DxLib.h>
#include "Game.h"
#include "Processing/Input.h"
#include "Scene/TitleScene.h"

constexpr int screen_x = 1280;
constexpr int screen_y = 740;

std::unique_ptr<Game, Game::GameDeleter> Game::s_Instance(new Game());

Game::Game()
{
}

Game::~Game()
{
}

const Vector2 Game::GetScreenSize() const
{
	return Vector2(screen_x, screen_y);
}

void Game::Init()
{
	DxLib::SetGraphMode(screen_x, screen_y, 32);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("–¼Ì–¢Ý’è‚¾‚¼(L¥ƒÖ¥`)");
	if (DxLib_Init() == -1)
	{
		return;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	ChangeScene(new TitleScene());
}

void Game::Run()
{
	Input input;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

		ClsDrawScreen();

		input.Update();

		_scene->Update(input);

		ScreenFlip();
	}
}

void Game::End()
{
	DxLib_End();
}

void Game::ChangeScene(Scene * scene)
{
	_scene.reset(scene);
}
