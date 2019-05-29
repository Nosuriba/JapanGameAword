#pragma once
#include "Enemy.h"

// ����_�p
struct CtlInfo
{
	Vector2 _pos;
	bool	_isTurn;

	CtlInfo() : _pos(0, 0), _isTurn(false) {};
	CtlInfo(const Vector2& p, bool f)
	{
		this->_pos = p;
		this->_isTurn = f;
	}
};

class Fish :
	public Enemy
{
private:
	// �j��
	void SwimUpdate();
	// ����
	void TurnUpdate();
	// �ǐ�
	void TrackUpdate();
	// ������
	void EscapeUpdate();
	// ���S
	void DieUpdate();

	void (Fish::*_updater)();

	std::vector<Vector2> midPoints;			// �޼ު�Ȑ��̒��ԓ_
	std::vector<CtlInfo> cPoints;			// �޼ު�Ȑ��̐���_

	Vector2 _target;		// �ǐՑΏ�
	int		_escapeTime;	// �����o�ߎ���

	Vector2 _axis;	// ����̎�
	float	_angle;	// 1�t���[���̐���p�x

	void Search();		// ���G
	void CalBezier();	// �޼ު�Ȑ��̌v�Z�p(3���޼ު)
	void LookAt(const Vector2& v);

public:
	Fish(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos);
	~Fish();

	void Update();

	void Draw();
	void DebugDraw();

	void OnDamage();

	//////////// ����Ȃ��q //////////////

	//////////////////////////////////////
};

