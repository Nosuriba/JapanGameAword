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
	if (_stages.find(str) != _stages.end()) return;

	int fmf_h = DxLib::FileRead_open(str.c_str(), false);
	if (fmf_h == -1) return;

	DxLib::FileRead_read(&_stages[str].fmf, sizeof(FMF), fmf_h);

	auto fmf = _stages[str].fmf;
	std::vector<char> tmp;
	tmp.resize(fmf.mapWidth * fmf.mapHeight);
	DxLib::FileRead_read(&tmp[0], tmp.size(), fmf_h);

	DxLib::FileRead_close(fmf_h);

	_stages[str].data.resize(tmp.size());
	for (int i = 0; i < fmf.mapHeight; i++) 
	{
		for (int j = 0; j < fmf.mapWidth; j++) 
		{
			_stages[str].data[j*fmf.mapHeight + i] = tmp[i*fmf.mapWidth + j];
		}
	}
	_stages[str].readX = 0;
}

std::vector<char> Stage::GetStageData(std::string str)
{
	if (_stages.find(str) == _stages.end())
		return std::vector<char>();
	return _stages[str].data;
}

std::vector<char> Stage::GetStageData(std::string str, int minX, int maxX)
{
	if (_stages.find(str) == _stages.end())
		return std::vector<char>();

	auto fmf = _stages[str].fmf;

	int L = max(minX / (_stages[str].fmf.chipW * 2), _stages[str].readX);
	int R = maxX / (_stages[str].fmf.chipW * 2) + 1;
	if (R <= _stages[str].readX) 
		return std::vector<char>();

	auto idxL = L * _stages[str].fmf.mapHeight;
	auto idxR = min(R * _stages[str].fmf.mapHeight, _stages[str].data.size());
	auto begin = _stages[str].data.begin() + idxL;
	auto end = min(_stages[str].data.begin() + idxR, _stages[str].data.end());
	_stages[str].readX = R;
	return std::vector<char>(begin, end);
}
