#pragma once

class DROPLETS_FALLING_SHADER : public D3D2DSQUARE
{
private:
	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_pTechnique, m_pTexOffset, m_pAddDropletsPos, m_pDistortion;
	LPDIRECT3DDEVICE9 m_d3dDevice;

	//滴マップ
	LPDIRECT3DTEXTURE9 m_pDropletsTexture[2];
	LPDIRECT3DSURFACE9 m_pDropletsSurface[2];

	//滴の軌跡マップ
	LPDIRECT3DTEXTURE9 m_pDropletsLocusTexture[2];
	LPDIRECT3DSURFACE9 m_pDropletsLocusSurface[2];

	//ブラー適応
	LPDIRECT3DTEXTURE9 m_pBlurTexture;
	LPDIRECT3DSURFACE9 m_pBlurSurface;

	//法線マップ
	LPDIRECT3DTEXTURE9 m_pNormalTexture;
	LPDIRECT3DSURFACE9 m_pNormalSurface;

	//滴マップの参照インデックス
	int m_RenderTargetIndex;

	//ガラスの摩擦係数テクスチャー
	LPDIRECT3DTEXTURE9 m_pNoiseTexture;

	//滴を適応する
	LPDIRECT3DTEXTURE9 UpdateDroplets(D3DPRESENT_PARAMETERS* pd3dParameters, LPDIRECT3DTEXTURE9 pBackBufferTexture, UINT Pass);

public:
	DROPLETS_FALLING_SHADER(LPDIRECT3DDEVICE9 pd3dDevice, D3DPRESENT_PARAMETERS* pd3dParameters);
	DROPLETS_FALLING_SHADER(LPDIRECT3DDEVICE9 pd3dDevice, UINT Width, UINT Height);
	~DROPLETS_FALLING_SHADER();
	void Invalidate();
	void Restore();
	HRESULT Load(TCHAR* pNoiseTextureName);
	//滴を追加する
	LPDIRECT3DTEXTURE9 Render(D3DPRESENT_PARAMETERS* pd3dParameters, LPDIRECT3DTEXTURE9 pBackBufferTexture, float AddDropletsPosX, float AddDropletsPosY, float Distortion);
	//滴を追加しない
	LPDIRECT3DTEXTURE9 Render(D3DPRESENT_PARAMETERS* pd3dParameters, LPDIRECT3DTEXTURE9 pBackBufferTexture, float Distortion);
	BOOL IsOK();
	LPD3DXEFFECT GetEffect(){ return m_pEffect; };
};