#include "Hart.h"
#include "ResourceManager.h"
int Hart::Num = 0;

Hart::Hart(Vector2 pos):_g(0.98), num(++Num)
{
	_pos = pos;
	_ground_y = _pos.y;
	_breakCnt=0, _toJamp=0,_Cnt = 0;
	_isbreak = false;
	_vec = Vector2();
	LoadDivGraph("../img/hart.png",25,5,5,10,10, imgBuff,true);
}


Hart::~Hart()
{
	/*for (auto &i :imgBuff)
	{
		DeleteGraph(i);
	}*/
}

void Hart::UpDate()
{
	_Cnt++;
	if (_Cnt>(60+num*60)&&(_vec.y==0))
	{
		_vec.y = -10;
		_Cnt = 0;
	}

	_pos.y += _vec.y;
	if (_isbreak)
	{
		_pos += _vec;
	}
	else
	{
		_vec.y += 2;
		if (_pos.y > _ground_y)
		{
			_pos.y = _ground_y;
			_vec.y = 0;
		}
	}
}

void Hart::Draw()
{
	for (auto y = 0; y < 25; y++)
	{
		DrawGraph(_pos.x + y%5 * 10, _pos.y+y/5*10, imgBuff[y], true);
	}
}
