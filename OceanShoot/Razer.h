/*
*	レーザーのクラス
*
*	2015/05/27		Y.Ozawa
*/

#pragma once

class Texture;
class Graphic;

//粒子のデーター
struct razer_point
{
	D3DXVECTOR3 pos;			//	レーザーの座標
};

class Razer
{
private:

	Texture *texture;
	Graphic *razer;
	razer_point *razerPoint;

public:

	//	デフォルトコンストラクタ
	Razer();

	//	デストラクタ
	~Razer();

	//	描画する
	void Draw(D3DXVECTOR3 Pos[]);
};
