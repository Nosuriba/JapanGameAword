#include "ResultScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "TitleScene.h"

void ResultScene::Wait(const Input & p)
{
	if (p.IsTrigger(PAD_INPUT_10)) {
		Game::GetInstance().ChangeScene(new TitleScene());
	}
	Draw();
}

ResultScene::ResultScene()
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

	_updater = &ResultScene::Wait;
}

ResultScene::~ResultScene()
{
}

void ResultScene::Update(const Input & p)
{
	(this->*_updater)(p);
}

void ResultScene::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();
	DrawString(size.x / 2 - GetFontSize() * 2, size.y / 2, "Result", 0xff00ff);
}
