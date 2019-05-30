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
	img = ResourceManager::GetInstance().LoadImg("../img/hart.png");
}


Hart::~Hart()
{
	for (auto &i :imgBuff)
	{
		DeleteGraph(i);
	}
}

void Hart::UpDate()
{
	_Cnt++;
	if (_Cnt>(60+num*60))
	{
		_vec.y = 25;
	}

	_pos.y += _vec.y;

	if (_isbreak)
	{
		_pos += _vec;
	}
	else
	{
		if (_pos.y > _ground_y)
		{
			_pos.y = _ground_y;
		}
	}
}

void Hart::Draw()
{
	//DrawGraph(_pos.x, _pos.y,img, true);
	for (auto y = 0; y < 5; y++)
	{
		for (auto i = 0; i < 5; i++)
		{
			DrawGraph(_pos.x + i * 10, _pos.y + i * 10, imgBuff[y*5+i], true);
		}
	}
}
