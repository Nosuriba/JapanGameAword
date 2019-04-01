#include "Model.h"
#include <DxLib.h>
#include <cassert>

Model::Model()
{
	_chara.emplace_back(Load("../Model/Earth/earth_100.pmx"));
	_chara.emplace_back(Load("../Model/Moon/Moon_100.pmx"));
}

int Model::Load(const char * handle)
{
	//���łɂ���Ε�������
	if (_handle.find(handle) != _handle.end()) {
		return DxLib::MV1DuplicateModel(_handle[handle]);
	}
	else {
		//��x���ǂݍ���ł��Ȃ����߃��[�h����
		_handle[handle] = DxLib::MV1LoadModel(handle);

		if (_handle[handle] == -1) {
			//�ǂݍ��݂Ɏ��s�����B
			std::string path = handle;
			std::string message = "�p�X���Ԉ���Ă��邩�A�Ή����Ă��Ȃ��`���ł��B\n[" + path + "]";
			MessageBox(nullptr, message.c_str(), "�x��", MB_OK);
			assert(_handle[handle]);
			return -1;
		}

		return _handle[handle];
	}
}

const std::vector<int> Model::GetChara() const
{
	std::vector<int> c;
	for (auto h : _chara) {
		c.push_back(MV1DuplicateModel(h));
	}

	return c;
}

Model::~Model()
{
}
