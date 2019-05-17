#include "Stage.h"
#include <DxLib.h>
Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::LoadStage(std::string str)
{
	name = str;
	count = 0;

	if (_stages.find(str) != _stages.end()) return;

	SetUseASyncLoadFlag(true);

	fmf_h = DxLib::FileRead_open(str.c_str(), false);

	SetUseASyncLoadFlag(false);
}

const bool Stage::LoadCheck()
{
	if (count == 0)
	{
		if (_stages.find(name) != _stages.end()) return true;
		if (CheckHandleASyncLoad(fmf_h)) return false;
		if (fmf_h == -1) return false;

		DxLib::FileRead_read(&_stages[name].fmf, sizeof(_stages[name].fmf), fmf_h);
		_stages[name].data.resize(_stages[name].fmf.mapWidth * _stages[name].fmf.mapHeight);
	}
	auto fmf = _stages[name].fmf;
	std::vector<char> tmp;
	tmp.resize(fmf.mapWidth);
	DxLib::FileRead_read(&tmp[0], tmp.size(), fmf_h);

	for (int j = 0; j < fmf.mapWidth; j++)
	{
		_stages[name].data[j * fmf.mapHeight + count].no = tmp[j];
		_stages[name].data[j * fmf.mapHeight + count].x = j * _stages[name].fmf.chipW + _stages[name].fmf.chipW / 2;
		_stages[name].data[j * fmf.mapHeight + count].y = count * _stages[name].fmf.chipH + _stages[name].fmf.chipH / 2;
	}

	++count;
	if (count == fmf.mapHeight)
	{
		count = 0;
		_stages[name].readX = 0;
		DxLib::FileRead_close(fmf_h);
		return true;
	}
	return false;
}

std::vector<ChipInfo> Stage::GetStageData()
{
	if (_stages.find(name) == _stages.end())
		return std::vector<ChipInfo>();
	return _stages[name].data;
}

std::vector<ChipInfo> Stage::GetStageData(int minX, int maxX)
{
	if (_stages.find(name) == _stages.end())
		return std::vector<ChipInfo>();

	auto fmf = _stages[name].fmf;

	int L = max(minX / (_stages[name].fmf.chipW * 2), _stages[name].readX);
	int R = maxX / (_stages[name].fmf.chipW * 2) + 1;
	if (R <= _stages[name].readX) 
		return std::vector<ChipInfo>();

	auto idxL = L * _stages[name].fmf.mapHeight;
	auto idxR = min(R * _stages[name].fmf.mapHeight, _stages[name].data.size());
	auto begin = _stages[name].data.begin() + idxL;
	auto end = min(_stages[name].data.begin() + idxR, _stages[name].data.end());
	_stages[name].readX = R;
	return std::vector<ChipInfo>(begin, end);
}
