#pragma once
#include "../Processing/Input.h"
#include "../Particle/Particle.h"

// ﾃﾞﾊﾞｯｸﾞの時長いから短くする
#ifdef _DEBUG
const int WAITFRAME = 60;
#else
const int WAITFRAME = 120;
#endif // DEBUG

class Particle;
class Scene
{
protected:
	// ﾌｪｰﾄﾞで使うバブル状のﾊﾟｰﾃｨｸﾙのポインタ
	static std::unique_ptr<Particle> FadeBubble;
	static std::unique_ptr<Particle> BuckBubble;
public:
	Scene();
	virtual ~Scene();

	//更新
	virtual void Update(const Input& p) = 0;
};

