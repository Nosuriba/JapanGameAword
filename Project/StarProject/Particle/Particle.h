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

	// ���W
	float x, y;

	// �߰è�ٖ{�̂̔z��
	std::vector<Element> particle;

	// �گ�ޗp�ϐ�
	std::thread p_thread;
	int rota;
	// �ړ��p�֐�
	virtual void Move() = 0;

public:
	// �`��p�֐�
	virtual void Draw() = 0;

	void SetRota(int _rota) { rota = _rota; };
	void SetPos(int _x, int _y) { x = _x, y = _y; };

	// �����p�֐�
	virtual void Create() = 0;

	// �������p�֐�
	virtual void Init() = 0;

	~Particle();
};
