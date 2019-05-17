#include <DxLib.h>
#include "Game.h"
#include "Processing/Input.h"
#include "Scene/TitleScene.h"

constexpr int screen_x = 1200;
constexpr int screen_y = 800;

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

const std::vector<int> Game::GetShaderHandle() const
{
	return _handle;
}

void Game::Init()
{
	DxLib::SetGraphMode(screen_x, screen_y, 32);
	DxLib::ChangeWindowMode(true);

	SetUseDirect3DVersion(DX_DIRECT3D_9EX);

	DxLib::SetWindowText("名称未設定だぞ(´･ω･`)");
	if (DxLib_Init() == -1)
	{
		return;
	}

	SetUseASyncLoadFlag(true);

	//ピクセルシェーダ読み込み
	wavehandle = LoadPixelShader("Shader/Wave.pso");
	shadowhandle = LoadPixelShader("Shader/Shadow.pso");

	_handle.push_back(wavehandle);
	_handle.push_back(shadowhandle);

	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	SetTextureAddressModeUV(DX_TEXADDRESS_BORDER, DX_TEXADDRESS_BORDER);

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
