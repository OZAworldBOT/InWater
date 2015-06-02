/*
*	爆弾のクラス
*	
*	2015/05/30		Y.Ozawa
*/

#pragma once


class Texture;
class Graphic;

//粒子のデーター
struct bomb_point
{
	D3DXVECTOR3 pos;			//	弾丸の座標
};

class Bomb
{
private:

	Texture *texture;
	Graphic *bomb;
	bomb_point *bombPoint;

public:

	//	デフォルトコンストラクタ
	Bomb();

	//	デストラクタ
	~Bomb();

	//	描画する
	void Draw(D3DXVECTOR3 Pos[], float Size, bool Alpha);
};













