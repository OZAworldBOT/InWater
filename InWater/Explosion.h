/*
*	爆発のクラス
*
*	2015/06/02		Y.Ozawa
*/

#pragma once

class Texture;
class Graphic;

//粒子のデーター
struct exp_point
{
	D3DXVECTOR3 pos;			//	爆発の座標
};


class Explosion
{
private:
	Texture *texture;
	Graphic *explosion;
	exp_point *expPoint;

public:
	//	コンストラクタ
	Explosion();

	//	デストラクタ
	~Explosion();

	//	描画する
	void Draw(D3DXVECTOR3 Pos[]);
};







