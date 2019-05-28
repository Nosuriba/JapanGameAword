#pragma once
#include <DxLib.h>
#include <vector>
#include <random>
#include <thread>

#ifdef _DEBUG
#else
#include "AMP.h"
#endif


// �߰è�ٗp�̍\����
struct Element
{
	/// Position
	float x=0;
	float y=0;

	/// Velocity
	float vx=0;
	float vy=0;

	/// AddVelocity
	float avx = 0;
	float avy = 0;

	/// Life
	float bright=0;

	/// Radius
	float radius = 1;
};

// �߰è�ق̉��z�֐�
class Particle
{
protected:
	// �v�f��
	int ElementNum;

	// ������[��T�C�Y
	static const int screen_x;
	static const int screen_y;


	// ���W
	float x, y;
	int rota;
	int vel;
	// �߰è�ٖ{�̂̔z��
	std::vector<Element> particle;

	// �گ�ޗp�ϐ�
	std::thread p_thread;

	// �ړ��p�֐�
	virtual void Move() = 0;
public:
	// �`��p�֐�
	virtual void Draw() = 0;
	virtual void Draw(int color) { };

	void SetRota(int _rota) { rota = _rota; };
	void SetVelocity(int _vel) { vel = _vel; };
	void SetPos(int _x, int _y) { x = _x, y = _y; };

	// �����p�֐�
	virtual void Create() = 0;

	// �������p�֐�
	virtual void Init() = 0;

	~Particle();
};

