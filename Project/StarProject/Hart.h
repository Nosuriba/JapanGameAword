#pragma once
#include "Processing/Geometry.h"
class Hart
{
private:
	static int Num;
	const int num;
	Vector2 _vec;
	int _ground_y;
	const float _g;
	Vector2 _pos;
	int _breakCnt,_toJamp,_Cnt;
	bool _isbreak;
	int imgBuff[4];
	int img;
public:
	Hart(Vector2 pos = {0,0});
	~Hart();
	void UpDate();
	void Draw();
};

