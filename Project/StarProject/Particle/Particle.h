#pragma once
#include <DxLib.h>
#include <vector>
#include <random>
#include <thread>

#ifdef _DEBUG
#else
#include "AMP.h"
#endif

// ﾊﾟｰﾃｨｸﾙ用の構造体
struct Element
{
	/// position
	float x=0;
	float y=0;

	/// velocity
	float vx=0;
	float vy=0;

	/// Life
	float bright=0;

	/// Radius
	float radius = 1;
};

// ﾊﾟｰﾃｨｸﾙの仮想関数
class Particle
{
protected:
	// 要素数
	int ElementNum;

	// 座標
	float x, y;

	// ﾊﾟｰﾃｨｸﾙ本体の配列
	std::vector<Element> particle;

	// ｽﾚｯﾄﾞ用変数
	std::thread p_thread;

	// 移動用関数
	virtual void Move() = 0;

public:
	// 描画用関数
	virtual void Draw() = 0;

	// 生成用関数
	virtual void Create() = 0;

	// 初期化用関数
	virtual void Init() = 0;

	~Particle() {};
};

