#include "SelectScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "GameScene.h"


void SelectScene::Wait(const Input & p)
{
	auto& game = Game::GetInstance();

	Draw();

	if (p.IsTrigger(PAD_INPUT_10)) {
		game.ChangeScene(new GameScene());
	}
}

SelectScene::SelectScene()
{
	//フォントのロード
	LPCSTR font = "H2O-Shadow.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("H2O Shadow", DX_CHARSET_DEFAULT);

	_updater = &SelectScene::Wait;
}

SelectScene::~SelectScene()
{
}

void SelectScene::Draw()
{
	auto& game = Game::GetInstance();
	auto size = game.GetScreenSize();

	DrawString(size.x / 2 - (float)(GetFontSize()) * 3.0f / 2.0f, size.y / 2, "Select", 0xa000f0);
}

void SelectScene::Update(const Input & p)
{
	(this->*_updater)(p);
}
