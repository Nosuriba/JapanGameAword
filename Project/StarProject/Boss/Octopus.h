#pragma once
#include "Boss.h"
#include <vector>
#include <array>


enum class E_LEG_STATE {
	NORMAL,			//通常
	PUNCH,			//パンチ
	CHASE,			//追いかける
	RE_MOVE,		//元に戻る
	DETH,			//死
};

struct E_Leg {
	Vector2 tip;	//先端座標
	std::vector<Vector2> joint;	//関節座標
	MATRIX mat;		//回転角
	const int T = 12;	//関節数
	E_LEG_STATE state;	//状態
	int angle;		//足の角度
	int cnt;		//足ごとの動き出すタイミング調整用
};

struct Oct {
	float r ;		//足の長さ
	Vector2 center;	//中心座標
	Vector2 movePos;	//回転する向きの座標
	Vector2 hedPos;		//頭の座標
	std::array<Vector2,2> eyePos;		//目の座標
	std::vector<Vector2> root;	//足の根元
	std::vector<E_Leg> legs;	//足
	int helth = 200;		//体力
	int interval = 0;		//無敵時間

};

class Octopus :
	public Boss
{
private:
	bool _returnFlag;		//パンチ用のフラグ
	bool _damageFlag;		//ダメージフラグ
	int _maxAngle;			//通常時の足の最大回転角
	int _idx;				//攻撃を行う足のｲﾝﾃﾞｯｸｽ番号
	int _timer;				//足の攻撃切り替え用

	int extend;
	int setup;

	std::vector<std::shared_ptr<Particle>> _particle;

	void IkCcd(Vector2 pos,int idx,int numMaxItaration);

	void Die();					//死亡処理
	void DieUpdate();			//死亡時の更新
	void Normal(int idx);		//通常状態
	void Punch(int idx);		//パンチ
	void OctInk();				//タコ墨
	void Chase(int idx);		//追跡
	void ReMove(int idx);		//元に戻す

	void LegMove(E_Leg& leg, int idx);	//遅れてついてくる
	//void Move();
	void HitUpd();				//当たり判定更新

	void NeturalUpdate();		//戦闘時の更新

	void DebugDraw();

	void (Octopus::*_updater)();
	Oct _oct;
public:
	Octopus(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos = Vector2(1000, 600));
	~Octopus();
	void OnDamage();			//ダメージ処理
	void Draw();
	void ShadowDraw();
	void SelectDraw(const Vector2 p, const float s);
	void Update();
	void HitBlock();			//ブロック衝突
};

