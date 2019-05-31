#pragma once
#include "Scene.h"

#include <DxLib.h>

#include <memory>
#include <vector>
#include <list>

class Input;
class Player;
class Enemy;
class Boss;
class Collision;
class Camera;
class Hart;

class Obstacle;
class DestroyableObject;
class PredatoryObject;
class ImmortalObject;
class GoalObject;

struct ScoreInfo {
	int enemy;
	int bite;
	int breakobj;
	int time;
	ScoreInfo() :enemy(0), bite(0), breakobj(0), time(0) {};
	ScoreInfo(const int& enemy, const int& bite, const int & breakobj, const float& time) {
		this->enemy = enemy;
		this->bite = bite;
		this->breakobj = breakobj;
		this->time = time;
	}
};

struct CutScreenInfo {
	int top;
	int bottom;
	int left;
	int right;
};

struct LevelUIInfo {
	Position2 circlePos;
	int circle_r;
	int backCircle_r;
};

class GameScene : public Scene
{
private:
	void (GameScene::*_updater)(const Input &p);
	void LoadStageUpdate(const Input& p);
	void LoadResourceUpdate(const Input& p);
	void FadeIn(const Input& p);
	void FadeOut(const Input& p);
	void Wait(const Input &p);
	void Run(const Input& p);
	void CutinUpdate(const Input& p);
	void nlDraw();
	void LoadResource();

	std::vector <std::shared_ptr<Hart>> _Harts;
	std::shared_ptr<Player> _pl;
	std::shared_ptr<Collision> _col;
	std::shared_ptr<Camera> _camera;

	std::list<std::shared_ptr<Enemy>>	_enemies;
	std::list<std::shared_ptr<Boss>>	_bosses;

	std::list<std::shared_ptr<DestroyableObject>>	_destroyObj;
	std::list<std::shared_ptr<PredatoryObject>>		_predatoryObj;
	std::vector<std::shared_ptr<ImmortalObject>>	_immortalObj;
	std::vector<std::shared_ptr<GoalObject>>		_goalObject;

	//スクリーン
	int firstscreen;
	int secondscreen;
	int thirdscreen;
	int _4thscreen;
	int uiscreen;

	int shader_time;

	VERTEX2DSHADER wave_vertex[4];
	VERTEX2DSHADER shadow_vertex[4];

	//イメージ
	int sea;
	int sea_effect;
	int beach;
	int gameclear;
	int gameover;


	//カウント系
	int fadewait;

	int waitNum;//ゲーム開始までのカウント
	int waitCnt;

	int gameCnt;
	int nlCnt,nlpl;

	//カットイン
	int cutinCnt;

	//時間
	int time;
	int totaltime;
	int flame;


	//UI等
	ScoreInfo score;

	//当たり用スクリーンのサイズの保管
	std::vector<CutScreenInfo> _cutAreaScreen;
	CutScreenInfo cutscr;

	int stageNum;

	//UI
	LevelUIInfo leveluiInfo;

	int guage,Lvimg,Numimg,cgauge;

public:
	GameScene(const int& stagenum);
	~GameScene();

	void Draw();
	void Update(const Input& p);
};

