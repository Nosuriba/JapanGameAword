#include "ResultScene.h"
#include "../Processing/Input.h"
#include "../ResourceManager.h"
#include "../Game.h"
#include "TitleScene.h"

void ResultScene::FadeIn(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 - 255 * (float)(flame) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (flame >= WAITFRAME) {
		_updater = &ResultScene::Wait;
	}
}

void ResultScene::FadeOut(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 * (float)(flame) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (flame >= WAITFRAME) {
		Game::GetInstance().ChangeScene(new TitleScene());
	}
	else {
		(*FadeBubble).Create();
	}
}

void ResultScene::Wait(const Input & p)
{
	Draw();
	if (!CheckSoundMem(BGM)) {
		PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
	}
	_updater = &ResultScene::Run;
}

void ResultScene::Run(const Input & p)
{
	auto size = Game::GetInstance().GetScreenSize();
	ResultCnt++;
	Draw();
	if (p.Trigger(BUTTON::A) || p.IsTrigger(PAD_INPUT_10)) {
		ResultData[1][(ResultCnt/40)% (int)R_Data::max] = ResultData[0][(ResultCnt/40) % (int)R_Data::max];
		ResultCnt = (ResultCnt+40);
		if (isEnd&&(StampCnt>=192))
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
	StampCnt = 0;
	//フォントのロード
	LPCSTR font = "CP_Revenge.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("チェックポイント★リベンジ", DX_CHARSET_DEFAULT);
	
	ResultStr = { "たおしたてき　:%02d x 100" ,"たべたかず　　:%02d x 100" , "こわしたかず　:%02d x 10","のこりタイム　:%02d x 1000","","　　総合点　　:%6d", };

	imgbuff = ResourceManager::GetInstance().LoadImg("../img/selectback.png");
	BGM = ResourceManager::GetInstance().LoadSound("System/result.mp3");
	SE= ResourceManager::GetInstance().LoadSound("System/stamp.mp3");
	ResultCnt = 0;

	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::enemy]		= enemy		)*100;
	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::bite]		= bite		)*100;
	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::breakobj]	= breakobj	)*10;
	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::time]		= time		)*1000;
	ResultData[0][(int)R_Data::total] += (ResultData[0][(int)R_Data::blank]		= 0			);
}

ResultScene::~ResultScene()
{
	if (CheckSoundMem(BGM)) {
		StopSoundMem(BGM);
	}
}

void ResultScene::Update(const Input & p)
{
	flame++;
	
	(this->*_updater)(p);

	(*FadeBubble).Draw();
}

void ResultScene::Draw()
{
	auto size = Game::GetInstance().GetScreenSize();
	int mode, palam;
	GetDrawBlendMode(&mode,&palam);
#ifdef _DEBUG
	std::string s[4] = { "C","B","A","猿" };
#else
	std::string s[4] = { "C","B","A","S" };
#endif // _DEBUG

	DrawExtendGraph(0, 0, size.x, size.y, imgbuff, true);
	ChangeFont("チェックポイント★リベンジ", DX_CHARSET_DEFAULT);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,128);
	DrawBox(size.x / 2 - GetFontSize() * 2 - 5, 6,
		size.x / 2 + GetFontSize() * 2 + 5, GetFontSize() + 5, 0x003377, true);
	DrawBox(size.x / 2 - GetFontSize()*8, size.y / 10,
		size.x / 2 + GetFontSize() * 8, size.y /5*4, 0x003377,true);
	SetDrawBlendMode(mode, palam);

	DrawString(size.x / 2 - GetFontSize() * 2.05, 5.05, "リザルト", 0);
	DrawString(size.x / 2 - GetFontSize() * 2, 5, "リザルト", 0xffdd00);

	int Cnt = 0;
	for (auto str: ResultStr)
	{
		if (ResultCnt > Cnt * 40)
		{
			if (ResultData[1][Cnt]< ResultData[0][Cnt])
			{
				(Cnt!=(int)R_Data::total)?ResultData[1][Cnt]++: ResultData[1][Cnt]+=200;
			}
			else
			{
				ResultData[1][Cnt] = ResultData[0][Cnt];

				if (str == ResultStr[ResultStr.size() - 1])	isEnd = true;
			} 
			DrawFormatString(size.x / 2 - GetFontSize() * 7.55f, size.y / 10 * (2 + Cnt) + 0.1f, 0, str.c_str(), ResultData[1][Cnt]);
			DrawFormatString(size.x / 2 - GetFontSize() * 7.5f, size.y / 10 * (2 + Cnt), 0xffdd00, str.c_str(), ResultData[1][Cnt]);
			if (ResultData[1][Cnt] == ResultData[0][Cnt])Cnt++;
			else break;
		}
	}

	if (isEnd)
	{
		if (StampCnt >30)
		{
			if (192 > StampCnt)
			{
				StampCnt+=3;
				if (192<= StampCnt) {
					PlaySoundMem(SE, DX_PLAYTYPE_BACK);
				}
			}
			else
			{
				
				DrawBox(0, size.y / 10 * 8.5f, size.x, size.y, 0x000000, true);
				ChangeFont("Rainy Days", DX_CHARSET_DEFAULT);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, (abs(ResultCnt % 512 - 255)*2));
				SetFontSize(96);
				DrawString((size.x - (float)(GetFontSize()) *3.f) / 2.0f, size.y / 10 * 8.7f, "A", 0x33ff33);
				SetFontSize(64);
				ChangeFont("チェックポイント★リベンジ", DX_CHARSET_DEFAULT);
				DrawString((size.x - (float)(GetFontSize()) * 2) / 2.0f, size.y / 10 * 9, "button to Title", 0xffffff);
				SetDrawBlendMode(mode, palam);
			}

			SetFontSize(582-StampCnt*2);
			DrawCircle((size.x + (float)(194 * 4.5f)) / 2, (size.y - (float)(194)) / 10 * 9, GetFontSize()-96, 0xff3333, 0, 10);
			DrawRotaString((size.x + (float)(194 *3.6f)) / 2+ 194 / 2, (size.y - (float)(194)) / 10 * 7.35f+ 194 / 2,1,1,
				GetFontSize()/2-2.78f, GetFontSize()/2-0.1f,(StampCnt*2 %360)*DX_PI_F/180,
				0xff3333, 0xff3333,false,s[ResultData[0][(int)R_Data::total] / 25000].c_str());
		}
		else
		{
			++StampCnt;
		}
	}

	SetFontSize(64);
	ChangeFont("Rainy Days", DX_CHARSET_DEFAULT);
}
