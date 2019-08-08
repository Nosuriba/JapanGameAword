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
	img = ResourceManager::GetInstance().LoadImg("../img/hart.png");
	for (int n = 0; n < 25; n++)
	{
		auto Theta = Rand(360)*DX_PI_F / 180;
		_piece[n]._devec = Vector2(cos(Theta)*(Rand(5)+1), sin(Theta)*(Rand(5)+1));
		_piece[n]._pos = (pos + Vector2(pieceSize*(n%5), pieceSize * (n / 5)));
		_piece[n]._ground_y = _piece[n]._pos.y;
	}
	_Cnt;
	_breakCnt=0,_Cnt = 0;
	_isbreak = false, _isdel = false;
	_vec = Vector2();

}


Hart::~Hart()
{

}

void Hart::UpDate()
{
	Size s;
	GetDrawScreenSize(&s.width, &s.height);
	_Cnt++;
	_vec.y += 3;
	if (_Cnt>(60+num)&&(!_isbreak))
	{
		_vec.y = -12;
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
		DrawRectRotaGraph(_piece[n]._pos.x + pieceSize / 2, _piece[n]._pos.y + pieceSize / 2,
			n%5*10,n/5*10,10,10,1, _isbreak ? _Cnt : 0,img,true);
	}
}

bool Hart::isDelete() 
{
	return _isdel; 
};
