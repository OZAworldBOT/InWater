/*
*	ステージのクラス　ステージないにはポイントスプライトで泡を表示
*
*	2015/05/27		Y.Ozawa
*/
#pragma once


class Graphic;
class Texture;

//粒子のデーター
struct b_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};


class Stage
{
	D3DXVECTOR3 MinRange;
	D3DXVECTOR3 MaxRange;

private:
	Texture *water;

	Graphic *stage;

	b_point *bubble;
public:

	//デフォルトコンストラクタ
	Stage();

	//デストラクタ
	~Stage();

	void Move();

	//撮影する
	void View();

	//	初期化
	void initStage();

};