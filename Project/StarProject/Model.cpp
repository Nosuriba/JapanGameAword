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
	//すでにあれば複製する
	if (_handle.find(handle) != _handle.end()) {
		return DxLib::MV1DuplicateModel(_handle[handle]);
	}
	else {
		//一度も読み込んでいないためロードする
		_handle[handle] = DxLib::MV1LoadModel(handle);

		if (_handle[handle] == -1) {
			//読み込みに失敗した。
			std::string path = handle;
			std::string message = "パスが間違っているか、対応していない形式です。\n[" + path + "]";
			MessageBox(nullptr, message.c_str(), "警告", MB_OK);
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
