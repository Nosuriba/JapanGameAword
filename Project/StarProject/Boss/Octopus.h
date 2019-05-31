#pragma once
#include "Boss.h"
#include <vector>
#include <array>


enum class E_LEG_STATE {
	NORMAL,			//�ʏ�
	PUNCH,			//�p���`
	CHASE,			//�ǂ�������
	RE_MOVE,		//���ɖ߂�
	DETH,			//��
};

struct E_Leg {
	Vector2 tip;	//��[���W
	std::vector<Vector2> joint;	//�֐ߍ��W
	MATRIX mat;		//��]�p
	const int T = 12;	//�֐ߐ�
	E_LEG_STATE state;	//���
	int angle;		//���̊p�x
	int cnt;		//�����Ƃ̓����o���^�C�~���O�����p
};

struct Oct {
	float r ;		//���̒���
	Vector2 center;	//���S���W
	Vector2 movePos;	//��]��������̍��W
	Vector2 hedPos;		//���̍��W
	std::array<Vector2,2> eyePos;		//�ڂ̍��W
	std::vector<Vector2> root;	//���̍���
	std::vector<E_Leg> legs;	//��
	int helth = 200;		//�̗�
	int interval = 0;		//���G����

};

class Octopus :
	public Boss
{
private:
	bool _returnFlag;		//�p���`�p�̃t���O
	bool _damageFlag;		//�_���[�W�t���O
	int _maxAngle;			//�ʏ펞�̑��̍ő��]�p
	int _idx;				//�U�����s�����̲��ޯ���ԍ�
	int _timer;				//���̍U���؂�ւ��p

	int extend;
	int setup;

	std::vector<std::shared_ptr<Particle>> _particle;

	void IkCcd(Vector2 pos,int idx,int numMaxItaration);

	void Die();					//���S����
	void DieUpdate();			//���S���̍X�V
	void Normal(int idx);		//�ʏ���
	void Punch(int idx);		//�p���`
	void OctInk();				//�^�R�n
	void Chase(int idx);		//�ǐ�
	void ReMove(int idx);		//���ɖ߂�

	void LegMove(E_Leg& leg, int idx);	//�x��Ă��Ă���
	//void Move();
	void HitUpd();				//�����蔻��X�V

	void NeturalUpdate();		//�퓬���̍X�V

	void DebugDraw();

	void (Octopus::*_updater)();
	Oct _oct;
public:
	Octopus(const std::shared_ptr<Camera>& c, const std::shared_ptr<Player>& p, const Vector2& pos = Vector2(1000, 600));
	~Octopus();
	void OnDamage();			//�_���[�W����
	void Draw();
	void ShadowDraw();
	void SelectDraw(const Vector2 p, const float s);
	void Update();
	void HitBlock();			//�u���b�N�Փ�
};

