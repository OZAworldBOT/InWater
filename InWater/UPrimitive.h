#pragma once

class D3D2DSQUARE
{
private:
	//頂点フォーマット
	DWORD D3D2DFVF;

	typedef struct _D3D2DVERTEX
	{
		float x, y, z, rhw;   //頂点座標
		DWORD color;          //頂点カラー
		float tu, tv;         //テクセル
	}D3D2DVERTEX;

	LPDIRECT3DVERTEXBUFFER9 m_pd3d2DVertex;
	LPDIRECT3DDEVICE9 m_d3dDevice;

	//２Ｄポリゴンのサイズ
	UINT m_Width, m_Height;

public:
	D3D2DSQUARE(LPDIRECT3DDEVICE9 pd3dDevice, D3DPRESENT_PARAMETERS* pd3dParameters);
	D3D2DSQUARE(LPDIRECT3DDEVICE9 pd3dDevice, UINT Width, UINT Height);
	~D3D2DSQUARE();
	void Invalidate();
	//2Dポリゴンのサイズを取得
	UINT GetWidth();
	UINT GetHeight();
	//2Dポリゴンのサイズを変更
	HRESULT Resize(UINT Width, UINT Height);
	HRESULT Resize(D3DPRESENT_PARAMETERS* pd3dParameters);
	//初期化
	HRESULT Load();
	//2Dポリゴンをレンダリング
	void Render();
};