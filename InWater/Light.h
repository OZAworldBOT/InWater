/*
*	光源を扱うクラス
*
*	2015/05/18		Y.Ozawa
*/

#pragma once

//	光源
class Light
{
public:
	D3DLIGHT9 light;		//	ライト

public:

	//	デフォルトコンストラクタ
	Light();

	//	デストラクタ
	~Light();
};
