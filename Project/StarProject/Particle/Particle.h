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

	// �ړ��p�֐�
	virtual void Move() = 0;

public:
	// �`��p�֐�
	virtual void Draw() = 0;

	// �����p�֐�
	virtual void Create() = 0;

	// �������p�֐�
	virtual void Init() = 0;

	~Particle() {};
};

