#pragma once
#include <string>
#include "../Geometry.h"

class Object
{
public:
	Object();
	~Object();
	void Init(std::string fileName, const Vector3& pos);
	virtual void Update();
	virtual void Draw();
protected:
	std::string fileName;
	VECTOR		startPos, endPos, hitPos2D, hitPos3D, modelPos3D;
	Vector3		pos;
	Vector3		catchPos;
	int			mouseX, mouseY;
	int			handle;
	bool dieFlag, hitFlag, catchFlag;
	void DebugDraw();
private:
	/// プレイヤー側に持っていく予定
	void CheckHitModel();			// レイがモデルと当たったかの判定用
	void MoveModel();				// モデルの操作用
};

