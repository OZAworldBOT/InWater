/*
*	弾丸のクラス
*
*	2015/05/27		Y.Ozawa
*/

#pragma once

class Texture;
class Graphic;

//粒子のデーター
struct bullet_point
{
	D3DXVECTOR3 pos;			//	弾丸の座標
};

class Bullet
{	
private:

	Texture *texture;
	Graphic *bullet;
	bullet_point *bulletPoint;

public:

	//	デフォルトコンストラクタ
	Bullet(const TCHAR *FileName, int Num);

	//	デストラクタ
	~Bullet();

	//	描画する
	void Draw(D3DXVECTOR3 Pos[], float Size, int Num);
};