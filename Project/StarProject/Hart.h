#pragma once
#include "Processing/Geometry.h"

struct H_ELEMENT{

	Vector2 _pos;
	Vector2 _devec;
	int _ground_y;
};

class Hart
{
private:
	const int pieceSize;
	const int num;
	const float _g;
	Vector2 _vec;
	int _breakCnt;
	int _Cnt;
	bool _isbreak,_isdel;
	int img;
	H_ELEMENT _piece[25];
	int Rand(int Max);
public:
	Hart(Vector2 pos = {0,0},int Num=0);
	~Hart();
	void UpDate();
	void Draw();
	void _ReCnt() { _Cnt = 0; };
	bool isDelete() { return _isdel; };
	void Break() { _isbreak = true; };
};

