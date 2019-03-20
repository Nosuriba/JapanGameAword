#include "Object.h"

Object::Object()
{
	fileName = "";
	pos		 = Vector3(0, 0, 0);
	catchPos = Vector3(0, 0, 0);
	startPos = VGet(0, 0, 0);
	endPos	 = VGet(0, 0, 0);
	hitPos2D = VGet(0, 0, 0);
	hitPos3D = VGet(0, 0, 0);
	modelPos3D = VGet(0, 0, 0);
	handle	 = 0;
	dieFlag  = catchFlag = hitFlag = false;
}

Object::~Object()
{
}

void Object::Init(std::string fileName, const Vector3& pos)
{
	this->pos	   = pos;
	this->fileName = fileName;
	this->handle   = DxLib::MV1LoadModel(fileName.c_str());		/// ���f���̓ǂݍ���

	/// ���̃J�����ݒ�
	DxLib::SetCameraNearFar(0.1f, 1000.0f);
	DxLib::SetCameraPositionAndTarget_UpVecY(VGet(0, 20, -20), VGet(0.f, 10.f, 0.f));
}

void Object::Update()
{
	int mouseX, mouseY = 0;
	DxLib::GetMousePoint(&mouseX, &mouseY);
	
	/// ���f���̓���m�F(���ݒ�)
	if (!catchFlag)
	{
		if (DxLib::GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			VECTOR scrPos, scrPos2, worldPos, worldPos2;

			scrPos  = VGet((float)mouseX, (float)mouseY, 0);
			scrPos2 = VGet((float)mouseX, (float)mouseY, 1.f);

			worldPos  = VGet(scrPos.x, scrPos.y, scrPos.z);
			worldPos2 = VGet(scrPos2.x, scrPos2.y, scrPos2.z);

			/// �}�E�X�J�[�\���̈ʒu����^���������C���΂��Ă���
			startPos = ConvScreenPosToWorldPos(worldPos);
			endPos	 = ConvScreenPosToWorldPos(worldPos2);

			/// �ǂ̃��f���Ɠ����蔻�����邩�̐ݒ�
			DxLib::MV1RefreshCollInfo(handle, -1);

			/// ���C�����f���ɓ����������̔���
			auto rayHitCheck = DxLib::MV1CollCheck_Line(handle, -1, startPos, endPos);

			if (rayHitCheck.HitFlag)
			{
				catchFlag = true;
				catchPos = Vector3(mouseX, mouseY, 0);

				modelPos3D = DxLib::MV1GetPosition(handle);

				hitPos3D = rayHitCheck.HitPosition;
				hitPos2D = ConvWorldPosToScreenPos(hitPos3D);
			}
		}
	}
	else
	{
		if (!(DxLib::GetMouseInput() & MOUSE_INPUT_LEFT))
		{
			catchFlag = false;
		}

		Vector3 movePos;
		VECTOR nowHitPos2D;
		VECTOR nowHitPos3D;
		VECTOR nowModelPos3D;

		movePos = Vector3((float)(mouseX - catchPos.x),
						  (float)(mouseY - catchPos.y),
						   0);
		
		nowHitPos2D	  = VGet(hitPos2D.x + movePos.x,
						     hitPos2D.y + movePos.y,
						     hitPos2D.z);

		nowHitPos3D	  = ConvScreenPosToWorldPos(nowHitPos2D);

		nowModelPos3D = VGet(modelPos3D.x + nowHitPos3D.x - hitPos3D.x,
							 modelPos3D.y + nowHitPos3D.y - hitPos3D.y,
							 modelPos3D.z + nowHitPos3D.z - hitPos3D.z);

		pos = Vector3(nowModelPos3D.x, nowModelPos3D.y, 0);
	}
	
	
}

void Object::Draw()
{
	/// ���N���X
	DxLib::MV1SetPosition(handle, VGet(pos.x, pos.y, pos.z));	/// ���f�����W�̐ݒ�
	DxLib::MV1DrawModel(handle);
	DebugDraw();
}

void Object::DebugDraw()
{
	/// ���W�����܂��n���Ă��Ȃ��̂ŁA�l��0�̂܂܂ɂȂ��Ă���
	auto dbg = 20;
	DxLib::DrawFormatString(0, 0, 0xffffff, "�n���̍��W(X)  %d", (int)(pos.x));
	DxLib::DrawFormatString(0, dbg * 1, 0xffffff, "�n���̍��W(Y)  %d", (int)(pos.y));
}
