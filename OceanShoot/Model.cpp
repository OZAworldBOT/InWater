/*
*	3Dモデル読み込み
*
*	2015/05/19		Y.Ozawa
*/

#include "Lib.h"

extern LPDIRECT3DDEVICE9 d3dDevice;

//	デフォルトコンストラクタ
Model::Model()
{
	mesh = nullptr;
	buffer = nullptr;
	numMaterial = 0;
}

//	コンストラクタ
Model::Model(const TCHAR *meshFileName)
{
	LoadMesh(meshFileName);
}

//	デストラクタ
Model::~Model()
{
	SafeRelease(buffer);
	SafeRelease(mesh);
}

//	メッシュを読み込む
bool Model::LoadMesh(const TCHAR *meshFileName)
{
	if (FAILED(D3DXLoadMeshFromX(meshFileName, D3DXMESH_SYSTEMMEM, d3dDevice, nullptr, &buffer, nullptr, &numMaterial, &mesh)))
	{
		DebugAlert("%sを\n読み込めませんでした。\nファイル名やディレクトリを間違えていないかご確認ください。", meshFileName);
		return false;
	}

	return true;
}
















