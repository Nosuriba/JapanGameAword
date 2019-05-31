#include "Hart.h"
#include "ResourceManager.h"
#include<random>

int Hart::Rand(int Max)
{
	std::random_device rd;
	std::mt19937 Rand(rd());
	return Rand()%Max;
}

Hart::Hart(Vector2 pos,int Num):_g(0.98f), num(Num), pieceSize(10)
{
	int buff[25];
	LoadDivGraph("../img/hart.png", 25, 5, 5, pieceSize, pieceSize, buff, true);

	for (int n = 0; n < 25; n++)
	{
		auto Theta = Rand(360)*DX_PI_F / 180;
		_piece[n].imgBuff = buff[n];
		_piece[n]._devec = Vector2(cos(Theta)*(Rand(5)+1), sin(Theta)*(Rand(5)+1));
		_piece[n]._pos = (pos + Vector2(pieceSize*(n%5), pieceSize * (n / 5)));
		_piece[n]._ground_y = _piece[n]._pos.y;
	}

	
	_breakCnt=0,_Cnt = 0;
	_isbreak = false, _isdel = false;
	_vec = Vector2();

}


Hart::~Hart()
{
	for (int num = 0; num < 25; num++)
	{
		DeleteGraph(_piece[num].imgBuff);
	}
}

void Hart::UpDate()
{
	Size s;
	GetDrawScreenSize(&s.width, &s.height);
	_Cnt++;
	_vec.y += 4*_g;
	if (_Cnt>(60+num)&&(!_isbreak))
	{
		_vec.y = -10;
		_Cnt = num;
	}
	for (int n = 0; n < 25; n++)
	{
		if (_isbreak)
		{
			if (_Cnt > (60 + num))
			{
				_isdel = true;
			}
			if ((((_piece[n]._pos - Vector2(s.width+pieceSize, s.height+pieceSize)).Magnitude()))&&
				(((_piece[n]._pos - Vector2()).Magnitude())))
			{
				_piece[n]._pos += _piece[n]._devec;
				_piece[n]._devec.y += _g;
			}
		}
		else
		{
			_piece[n]._pos.y += _vec.y;
			if (_piece[n]._pos.y > _piece[n]._ground_y)
			{
				_piece[n]._pos.y = _piece[n]._ground_y;
				_vec.y = 0;
			}
		}
	}
}

void Hart::Draw()
{
	for (auto n = 0; n < 25; n++)
	{
		DrawRotaGraph(_piece[n]._pos.x+ pieceSize/2, _piece[n]._pos.y+ pieceSize/2,
			1, _isbreak? _Cnt:0,_piece[n].imgBuff, true);
	}
}
