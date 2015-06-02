/*
*	3Dモデル読み込み
*
*	2015/05/19		Y.Ozawa
*/

#pragma once

class Model
{
public:

	LPD3DXMESH mesh;		//	メッシュ
	LPD3DXBUFFER buffer;	//	頂点バッファ
	DWORD numMaterial;		//	マテリアルの数

public:

	//	デフォルトコンストラクタ
	Model();

	//	コンストラクタ
	Model(const TCHAR *meshFileName);

	//	デストラクタ
	~Model();

	//	メッシュをロードする
	bool LoadMesh(const TCHAR *meshFileName);


};