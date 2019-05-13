#pragma once
#include "../Processing/Input.h"
#include "../Bubble.h"

// ﾃﾞﾊﾞｯｸﾞの時長いから短くする
#ifdef _DEBUG
const int WAITFRAME = 60;
#else
const int WAITFRAME = 120;
#endif // DEBUG


class Scene
{
public:
	Scene();
	virtual ~Scene();

	//更新
	virtual void Update(const Input& p) = 0;
};

