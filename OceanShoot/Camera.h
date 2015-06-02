/*
*	カメラクラス
*
*	2015/05/19		Y.Ozawa
*/

#pragma once

// 3Dを撮影するためのクラス
class Camera
{
private:

	D3DVIEWPORT9 viewport;		// ビューポート
	float aspect;				// アスペクト比

	D3DXMATRIX m_view;			// ビュー行列
	D3DXMATRIX m_projection;	// 射影行列

public:

	//デフォルトコンストラクタ
	Camera();

	//デストラクタ
	~Camera();

	//カメラの初期化
	void Initialize();

	//カメラの解放
	void Release();

	//カメラから撮影する
	void View(D3DXVECTOR3 pos, D3DXVECTOR3 rota);

private:

	//座標変換
	void TransForm(D3DXVECTOR3 pos, D3DXVECTOR3 rota);

};
