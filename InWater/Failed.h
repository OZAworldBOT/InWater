/*
*	タイトル画面のクラス
*
*	2015/06/03		Y.Ozawa
*/

#pragma once


class Graphic;
class Texture;
class Camera;

//粒子のデーター
struct f_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};


class Failed
{
	D3DXVECTOR3 MinRange;
	D3DXVECTOR3 MaxRange;
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Rot;
	D3DXVECTOR3 Scale;
	D3DXVECTOR3 camera_Pos;
	D3DXVECTOR3 camera_Rot;

private:
	Texture *texture;

	Graphic *graphic;

	f_point *point;

	Camera *camera;
public:

	//デフォルトコンストラクタ
	Failed();

	//デストラクタ
	~Failed();

	//撮影する
	void View();

	//	初期化
	void Init();

	//	解放
	void Release();
};