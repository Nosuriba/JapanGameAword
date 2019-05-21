#include "ResultScene.h"
#include "../Processing/Input.h"
#include "../ResourceManager.h"
#include "../Game.h"
#include "TitleScene.h"

void ResultScene::FadeIn(const Input & p)
{
	if (flame >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		_updater = &ResultScene::Wait;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (float)(flame) / WAITFRAME);
	Draw();

}

void ResultScene::FadeOut(const Input & p)
{
	if (flame >= WAITFRAME) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		(*FadeBubble).Draw();
		Game::GetInstance().ChangeScene(new TitleScene());
	}
	else {
		(*FadeBubble).Create();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 255 * (float)(flame) / WAITFRAME);
		Draw();
	}
}

void ResultScene::Wait(const Input & p)
{
	Draw();
	_updater = &ResultScene::Run;
}

void ResultScene::Run(const Input & p)
{
	auto size = Game::GetInstance().GetScreenSize();
	ResultCnt++;
	Draw();
	if (p.IsTrigger(PAD_INPUT_10)) {
		ResultCnt = (ResultCnt+40);
		if (isEnd)
		{
			flame = 0;
			_updater = &ResultScene::FadeOut;
		}
	}
}

ResultScene::ResultScene(const int& enemy, const int& bite, const int & breakobj, const int& time)
{
	_updater = &ResultScene::FadeIn;

	flame = 0;

	//フォントのロード
	LPCSTR font = "CP_Revenge.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("チェックポイント★リベンジ", DX_CHARSET_DEFAULT);
	
	ResultStr = { "たおしたてき　:%3d ×100" ,"たべたかず　　:%3d ×100" , "こわしたかず　　:%3d ×10","のこりタイム　:%3d ×1000","","　　総合点　　:%6d", };

	imgbuff = ResourceManager::GetInstance().LoadImg("../img/selectback.png");
	ResultCnt = 0;

	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::enemy]		= 6)*100;
	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::bite]		= 3)*100;
	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::breakobj]	= 20)*10;
	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::time]		= time)*1000;
	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::blank]		= 0);

}

ResultScene::~ResultScene()
{
}

void ResultScene::Update(const Input & p)
{
	flame++;
	(this->*_updater)(p);
}

void ResultScene::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();
	int mode, palam;
	GetDrawBlendMode(&mode,&palam);

	DrawExtendGraph(0, 0, size.x, size.y, imgbuff, true);
	DrawString(0, 0, "^p^", 0xff00ff);
	ChangeFont("チェックポイント★リベンジ", DX_CHARSET_DEFAULT);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(size.x / 2 - GetFontSize()*8, size.y / 10,
		size.x / 2 + GetFontSize() * 8, size.y /5*4, 0x000000,true);
	SetDrawBlendMode(mode, palam);

	DrawString(size.x / 2 - GetFontSize() * 2, 5, "リザルト", 0xff8c00);
	DrawString(size.x / 2 - GetFontSize() * 2.05, 5.05, "リザルト", 0);

	int Cnt = 0;
	for (auto str: ResultStr)
	{
		if (ResultCnt > Cnt * 40)
		{
			if (ResultData[1][Cnt]< ResultData[0][Cnt])
			{
				Cnt!=5?ResultData[1][Cnt]++: ResultData[1][Cnt]+=1000;
			}
			else
			{
				ResultData[1][Cnt] = ResultData[0][Cnt];

				if (str == ResultStr[ResultStr.size() - 1])
				{
					isEnd = true;
				}
			}
			DrawFormatString(size.x / 2 - GetFontSize() * 7.55f, size.y / 10 * (2 + Cnt) + 0.05f, 0, str.c_str(), ResultData[1][Cnt]);
			DrawFormatString(size.x / 2 - GetFontSize() * 7.5f, size.y / 10 * (2 + Cnt), 0xff8c00, str.c_str(), ResultData[1][Cnt]);
			if (ResultData[1][Cnt] == ResultData[0][Cnt])Cnt++;
			else break;
		}
	}

	(*FadeBubble).Draw();

	if (isEnd)
	{
		DrawBox(0, size.y / 10*8.5f, size.x, size.y, 0x000000, true);
		ChangeFont("Rainy Days", DX_CHARSET_DEFAULT);
		SetFontSize(96);
		DrawString((size.x - (float)(GetFontSize()) *1.5f) / 2.0f, size.y / 10 * 8.7f, "A", 0x33ff33);
		SetFontSize(64);
		ChangeFont("チェックポイント★リベンジ", DX_CHARSET_DEFAULT);
		DrawString((size.x - (float)(GetFontSize()) * 2) / 2.0f, size.y / 10 * 9, "button to Title", 0xffffff);
	}

	ChangeFont("Rainy Days", DX_CHARSET_DEFAULT);
}
