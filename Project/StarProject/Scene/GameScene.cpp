#include "GameScene.h"
#include "../Processing/Input.h"
#include "../Game.h"
#include "ResultScene.h"
#include "../Player.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/Fish.h"
#include "../Enemy/Diodon.h"
#include "../Processing/Collision.h"
#include "../Camera.h"
#include "../Object/Obstacle.h"
#include "../Object/DestroyableObject.h"
#include "../Object/PredatoryObject.h"
#include "../Object/ImmortalObject.h"

void GameScene::FadeIn(const Input & p)
{
	if (wait >= 60) {
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		_updater = &GameScene::Wait;
	}
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * (float)wait / 60.0f);
	Draw();
}

void GameScene::FadeOut(const Input & p)
{
	if (wait >= 180) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		BubbleDraw();
		Game::GetInstance().ChangeScene(new ResultScene());
	}
	else {
		BubbleCreate();
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - 255 * (float)wait / 180.0f);
		Draw();
	}
}

void GameScene::Wait(const Input & p)
{
	Draw();
	_updater = &GameScene::Run;
}

void GameScene::Run(const Input & p)
{
	Draw();
	if (p.IsTrigger(PAD_INPUT_10)) {
		wait = 0;
		_updater = &GameScene::FadeOut;
	}
	else if (totaltime == 0) {
		wait = 0;
		_updater = &GameScene::FadeOut;
	}
}

GameScene::GameScene()
{
	auto& size = Game::GetInstance().GetScreenSize();
	_camera.reset(new Camera());

	_pl.reset(new Player(_camera));

	_col.reset(new Collision());

	_destroy.reset(new DestroyableObject(_camera));

	_predatory.reset(new PredatoryObject(_camera));

	_immortal.reset(new ImmortalObject(_camera));

	firstscreen = MakeScreen(size.x * 2, size.y);
	secondscreen = MakeScreen(size.x * 2, size.y);
	thirdscreen = MakeScreen(size.x * 2, size.y);

	for (int i = 0; i < 2; ++i)
	{
		if (i <= 0)
		{
			_enemies.push_back(std::make_shared<Fish>(_camera));
		}
		else
		{
			_enemies.push_back(std::make_shared<Diodon>(_camera));
		}
	}

	//フォントのロード
	LPCSTR font = "H2O-Shadow.ttf";
	if (AddFontResourceEx(font, FR_PRIVATE, nullptr) > 0) {
	}
	else {
		MessageBox(nullptr, "失敗", "", MB_OK);
	}

	SetFontSize(64);

	ChangeFont("H2O Shadow", DX_CHARSET_DEFAULT);

	//画像の読み込み
	sea = LoadGraph("../img/sea.png");
	sea_effect = LoadGraph("../img/sea2.png");
	beach = LoadGraph("../img/砂浜.png");

	//頂点の設定

	for (int i = 0; i < 4; i++)
	{
		vertex[i].pos = VGet((i % 2)* (size.x * 2 - 200), (i / 2)*size.y, 0);
		vertex[i].rhw = 1.0f;
		vertex[i].dif = GetColorU8(255, 255, 255, 255);
		vertex[i].spc = GetColorU8(0, 0, 0, 0);
		vertex[i].u = vertex[i].su = (float)(i % 2);
		vertex[i].v = vertex[i].sv = (float)(i / 2);
	}

	//オブジェクトの生成
	_destroyObj.emplace_back(std::make_shared<DestroyableObject>(_camera));
	_predatoryObj.emplace_back(std::make_shared<PredatoryObject>(_camera));
	_immortalObj.emplace_back(std::make_shared<ImmortalObject>(_camera));

	flame = 0;
	wait = 0;
	time = 60;
	totaltime = 60;

	shader_time = 0;

	_updater = &GameScene::FadeIn;
}

GameScene::~GameScene()
{
	
}

void GameScene::Draw()
{
	int sizex, sizey;
	DxLib::GetWindowSize(&sizex, &sizey);


	//firstスクリーン
	SetDrawScreen(firstscreen);

	ClearDrawScreen();

	DrawExtendGraph(0 - _camera->CameraCorrection().x, 0 - _camera->CameraCorrection().y,
		sizex - _camera->CameraCorrection().x, sizey - _camera->CameraCorrection().y, beach, true);

	_pl->Draw();

	for (auto itr : _enemies)
	{
		itr->Draw();
	}

	for (auto &destroy : _destroyObj) {
		destroy->Draw();
	}
	for (auto &predatory : _predatoryObj) {
		predatory->Draw();
	}
	for (auto &immortal : _immortalObj) {
		immortal->Draw();
	}

	auto one = totaltime % 10;
	auto ten = totaltime / 10;

	DrawFormatString(sizex / 2, GetFontSize() / 2, 0xff00ff, "%d", one);
	DrawFormatString(sizex / 2 - GetFontSize(), GetFontSize() / 2, 0xff00ff, "%d", ten);
	BubbleDraw();

	//secondスクリーン
	SetDrawScreen(secondscreen);

	ClearDrawScreen();

	DrawExtendGraph(0, 0 , sizex * 2, sizey, sea_effect, true);

	//シェーダで使うテクスチャは先ほど作った描画可能画像
	SetUseTextureToShader(0, secondscreen);

	//シェーダーに情報を渡す
	SetPSConstSF(0, shader_time / 100.0f);

	//ピクセルシェーダのセット
	SetUsePixelShader(Game::GetInstance().GetShaderHandle());

	DrawPrimitive2DToShader(vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP);


	//thirdスクリーン
	SetDrawScreen(thirdscreen);

	ClearDrawScreen();

	DrawExtendGraph(0, 0, sizex * 2, sizey, sea, true);


	//バック描画
	SetDrawScreen(DX_SCREEN_BACK);

	ClearDrawScreen();

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawGraph(0, 0, firstscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 100);

	DrawGraph(0 - 200, 0, secondscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 60);

	DrawGraph(0 - 200, 0, thirdscreen, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void GameScene::Update(const Input & p)
{
	flame++; wait++; shader_time++;

	_pl->Update(p);

	for (auto itr : _enemies)
	{
		itr->Update();
	}

	for (int i = 0; i < _enemies.size(); ++i)
	{
		/// 敵の死亡処理
		if (_enemies[i]->GetInfo()._dieFlag)
		{
			_enemies.erase(_enemies.begin() + i);
			continue;
		}

		auto sVec = _pl->GetShot();
		/// ﾌﾟﾚｲﾔｰｼｮｯﾄと敵の当たり判定
		for (int p = 0; p < _pl->GetShot().size(); ++p)
		{
			if (_col->WaterToSqr(_pl->GetInfo().legs[p].tip, sVec[p],_enemies[i]->GetInfo()._rect))
			{
				auto vec = _enemies[i]->GetInfo()._pos - _pl->GetInfo().legs[p].tip;
				vec.Normalize();

				_enemies[i]->CalEscapeDir(vec);
				break;
			}
			_enemies[i]->CalTrackVel(_pl->GetInfo().center, _col->TriToTri(_pl->GetInfo().legs, _enemies[i]->GetInfo()._searchVert));
		}
		
		/// ﾌﾟﾚｲﾔｰと敵ｼｮｯﾄの当たり判定
		for (int s = 0; s < _enemies[i]->GetShotInfo().size(); ++s)
		{
			if (_col->TriToSqr(_pl->GetInfo().legs, _enemies[i]->GetShotInfo()[s]._pos, _enemies[i]->GetShotInfo()[s]._size))
			{
				_enemies[i]->ChangeShotColor(s);		/// ﾌﾟﾚｲﾔｰに当たった弾の色を変えている。
			}
		}
	}

	//破壊可能オブジェクト
	for (auto &destroy : _destroyObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().legs[i].tip, sVec[i], destroy->GetInfo()._rect))
			{
				destroy->Break();
			}
		}
	}

	//破壊不可オブジェクト
	for (auto &predatry : _predatoryObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().legs[i].tip, sVec[i], predatry->GetInfo()._rect))
			{
				predatry->Break();
			}
		}
	}

	//捕食対象
	for (auto &immortal : _immortalObj) {
		auto sVec = _pl->GetShot();
		for (int i = 0; i < sVec.size(); ++i) {
			if (_col->WaterToSqr(_pl->GetInfo().legs[i].tip, sVec[i], immortal->GetInfo()._rect))
			{
				immortal->Break();
			}
		}
	}

	if (_enemies.size() <= 0)
	{
		DrawString(0, 0, "Not Enemy", 0xffffff);
	}

	totaltime = time - (flame / 60);

	_camera->Update(_pl->GetInfo().center);

	(this->*_updater)(p);
}