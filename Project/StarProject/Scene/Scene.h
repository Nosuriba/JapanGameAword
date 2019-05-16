#pragma once
#include "../Processing/Input.h"
#include "../Particle/Particle.h"

// ���ޯ�ނ̎���������Z������
#ifdef _DEBUG
const int WAITFRAME = 60;
#else
const int WAITFRAME = 120;
#endif // DEBUG

class Particle;
class Scene
{
protected:
	// ̪��ނŎg���o�u������߰è�ق̃|�C���^
	static std::unique_ptr<Particle> FadeBubble;
	static std::unique_ptr<Particle> BuckBubble;
public:
	Scene();
	virtual ~Scene();

	//�X�V
	virtual void Update(const Input& p) = 0;
};

