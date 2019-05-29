#include "SelectScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "GameScene.h"
#include "../ResourceManager.h"
#include "../Particle/Bubble.h"
#include <string>
#include "../Stage.h"
#include "../Player.h"
#include "../Camera.h"
#include "../Boss/Octopus.h"
#include "../Boss/Crab.h"


void SelectScene::FadeIn(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	auto a = 255 - 255 * (float)(flame) / WAITFRAME;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 255 * (float)(flame) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (flame >= WAITFRAME) 
	{
		if (!CheckHandleASyncLoad(BGM)) 
		{
			PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
			_updater = &SelectScene::Wait;
		}
	}
}

void SelectScene::FadeOut(const Input & p)
{
	auto s = Game::GetInstance().GetScreenSize();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	Draw();
	SetDrawBlendMode(DX_BLENDMODE_MULA, 255 * (float)(flame) / WAITFRAME);
	DrawBox(0, 0, s.x, s.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (flame >= WAITFRAME) {
		Game::GetInstance().ChangeScene(new GameScene(Select));
	}
	else {
		(*FadeBubble).Create();
	}
}

void SelectScene::Wait(const Input & p)
{
	Draw();

	_updater = &SelectScene::Run;
}

void SelectScene::Run(const Input & p)
{
	Draw();

	if (p.Trigger(BUTTON::A) || p.IsTrigger(PAD_INPUT_10)) {
		if (!CheckHandleASyncLoad(SE))
		{
			flame = 0;
			PlaySoundMem(SE, DX_PLAYTYPE_BACK);
			StopSoundMem(BGM);
			Stage::GetInstance().LoadStage("../Stage/test"+ std::to_string(Select+1) +".fmf");
			_updater = &SelectScene::FadeOut;
		}
	}
	if (p.TriggerTrigger(TRIGGER::RIGHT)) {
		Select = (Select<2)?++Select:Select;
	}

	if (p.TriggerTrigger(TRIGGER::LEFT)) {
		Select = (Select>0)?--Select:Select;
	}
}

SelectScene::SelectScene()
{
	auto size = Game::GetInstance().GetScreenSize();

	bubble = ResourceManager::GetInstance().LoadImg("../img/Bubble.png");
	background = ResourceManager::GetInstance().LoadImg("../img/selectback.png");

	//ñAÇÃÉVÉFÅ[É_Å[í∏ì_
	int bubblesizex,bubblesizey;
	GetGraphSize(bubble, &bubblesizex, &bubblesizey);
	for (int i = 0; i < 4; i++)
	{
		bubble_vertex[i].pos = VGet((i % 2)* bubblesizex, (i / 2)*bubblesizey, 0);
		bubble_vertex[i].rhw = 1.0f;
		bubble_vertex[i].dif = GetColorU8(255, 255, 255, 255);
		bubble_vertex[i].spc = GetColorU8(0, 0, 0, 0);
		bubble_vertex[i].u = bubble_vertex[i].su = (float)(i % 2);
		bubble_vertex[i].v = bubble_vertex[i].sv = (float)(i / 2);
	}

  	firstscreen = MakeScreen(size.x, size.y);
	secondscreen = MakeScreen(size.x, size.y);
	_updater = &SelectScene::FadeIn;
	shader_time = 0;
	flame = 0;
	Cnt = 0;
	Select = 0; 

	CoralBubble.push_back(std::make_unique<Bubble>(size.x /5*2, size.y / 5 * 2,150,true,1));
	CoralBubble.push_back(std::make_unique<Bubble>(50, size.y / 10 * 7, 100, true, 1));
	CoralBubble.push_back(std::make_unique<Bubble>(size.x / 5 * 4, size.y / 5 * 4, 125, true, 1));

	BGM = ResourceManager::GetInstance().LoadSound("under-the-sea-1.mp3");
	SE = ResourceManager::GetInstance().LoadSound("se_maoudamashii_effect15.mp3");
}

SelectScene::~SelectScene()
{
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SelectScene::Draw()
{
	auto& game = Game::GetInstance();
	auto size = game.GetScreenSize();

	DrawExtendGraph(0, 0, size.x, size.y, background, true);

	for (auto &b:CoralBubble)
	{
		(*b).Draw();
	}

	auto addx = cos((Cnt % 360)*DX_PI / 180) * 25;
	auto addy = sin((Cnt)*DX_PI / 720) * 100;
	auto addr = sin((Cnt)*DX_PI / 180) * 0.1;

	auto _camera = std::make_shared<Camera>();

	auto pl = std::make_unique<Player>(nullptr);
	auto oct = std::make_shared<Octopus>(_camera);
	auto crab = std::make_shared<Crab>(_camera);

	// ñAÇÃíÜÇÃDraw
	oct->SelectDraw (Vector2(size.x / 4 * 3 + 130 + ((Select != 2) ? addx : 0), size.y / 2 + ((Select != 2) ? addy : 0) - 10), (1 + ((Select != 2) ? addr : addr + 0.5))*0.3f);
	crab->SelectDraw(Vector2(size.x / 4 * 2 + ((Select != 1) ? addx : 0) - 5, size.y / 2 - ((Select != 1) ? addy : 0) + 20), (1 - ((Select != 1) ? addr : addr - 0.5))*0.58f);
	pl->SelectDraw  (Vector2(size.x / 4 - 100 + ((Select != 0) ? addx : 0), size.y / 2 + ((Select != 0) ? addy : 0)), (1 + ((Select != 0) ? addr : addr + 0.5)) * 100);

	// ñAÇÃDraw
	DrawRotaGraph(size.x / 4 - 100 + ((Select != 0) ? addx : 0), size.y / 2 + ((Select != 0) ? addy : 0), 1 + ((Select != 0) ? addr : addr +0.5), 0, bubble, true);
	DrawRotaGraph(size.x / 4 * 2 + ((Select != 1) ? addx : 0), size.y / 2 - ((Select != 1) ? addy : 0), 1 - ((Select != 1) ? addr: addr -0.5), 0, bubble, true);
	DrawRotaGraph(size.x / 4 * 3 + 100 + ((Select != 2) ? addx : 0), size.y / 2 + ((Select != 2) ? addy : 0), 1 +((Select != 2) ? addr : addr +0.5), 0, bubble, true);

	

	DrawString(size.x / 2 - (float)(GetFontSize()) * 4.0f / 2.0f, size.y / 2 + size.y / 4, "Select", 0xa000f0);
}

void SelectScene::Update(const Input & p)
{
	for (auto &b : CoralBubble)
	{
		(*b).Create();
	}
	flame++;
	Cnt++;
	shader_time++;
	(this->*_updater)(p);

	(*FadeBubble).Draw();
}
