/*
*	カメラクラス
*
*	2015/05/19		Y.Ozawa
*/

#include "Lib.h"

extern LPDIRECT3DDEVICE9 d3dDevice;

// デフォルトコンストラクタ
Camera::Camera()
{
	DebugLog("カメラを生成しました。\n");

	Initialize();
}

// デストラクタ
Camera::~Camera()
{
	DebugLog("カメラを破棄しました。\n");

	Release();
}

// カメラの初期化
void Camera::Initialize()
{
	d3dDevice->GetViewport(&viewport);
	d3dDevice->SetViewport(&viewport);

	// アスペクト比の計算
	aspect = (float)viewport.Width / (float)viewport.Height;

	DebugLog("カメラを初期化しました。\n");
}

// カメラの破棄
void Camera::Release()
{

}

// カメラから撮影する
void Camera::View(D3DXVECTOR3 pos, D3DXVECTOR3 rota)
{
	// 座標計算
	TransForm(pos, rota);

	// カメラのビュー行列を計算
	D3DXMATRIX *m_temp = new D3DXMATRIX;
	D3DXMatrixIdentity(m_temp);
	D3DXMatrixInverse(m_temp, nullptr, &m_view);	// カメラの逆行列を求める

	// ビュー行列を設定
	d3dDevice->SetTransform(D3DTS_VIEW, m_temp);

	// 射影行列の初期化
	D3DXMatrixIdentity(&m_projection);

	// 射影行列を作成
	D3DXMatrixPerspectiveFovLH(&m_projection, D3DXToRadian(45.0f), aspect, 1.0f, 100000.0f);

	// 射影行列をパイプラインに設定
	d3dDevice->SetTransform(D3DTS_PROJECTION, &m_projection);

	delete m_temp;
}

// 座標変換
void Camera::TransForm(D3DXVECTOR3 pos, D3DXVECTOR3 rota)
{
	D3DXMATRIX *m_temp = new D3DXMATRIX;
	D3DXMatrixIdentity(&m_view);
	D3DXMatrixIdentity(m_temp);

	// 行列を回転する
	D3DXMatrixRotationYawPitchRoll(m_temp, rota.y, rota.x, rota.z);
	m_view *= *m_temp;

	// 行列を平行移動させる
	D3DXMatrixTranslation(m_temp, pos.x, pos.y, pos.z);
	m_view *= *m_temp;

	delete m_temp;
}
