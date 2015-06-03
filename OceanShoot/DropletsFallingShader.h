#pragma once

class DROPLETS_FALLING_SHADER : public D3D2DSQUARE
{
private:
	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_pTechnique, m_pTexOffset, m_pAddDropletsPos, m_pDistortion;
	LPDIRECT3DDEVICE9 m_d3dDevice;

	//�H�}�b�v
	LPDIRECT3DTEXTURE9 m_pDropletsTexture[2];
	LPDIRECT3DSURFACE9 m_pDropletsSurface[2];

	//�H�̋O�Ճ}�b�v
	LPDIRECT3DTEXTURE9 m_pDropletsLocusTexture[2];
	LPDIRECT3DSURFACE9 m_pDropletsLocusSurface[2];

	//�u���[�K��
	LPDIRECT3DTEXTURE9 m_pBlurTexture;
	LPDIRECT3DSURFACE9 m_pBlurSurface;

	//�@���}�b�v
	LPDIRECT3DTEXTURE9 m_pNormalTexture;
	LPDIRECT3DSURFACE9 m_pNormalSurface;

	//�H�}�b�v�̎Q�ƃC���f�b�N�X
	int m_RenderTargetIndex;

	//�K���X�̖��C�W���e�N�X�`���[
	LPDIRECT3DTEXTURE9 m_pNoiseTexture;

	//�H��K������
	LPDIRECT3DTEXTURE9 UpdateDroplets(D3DPRESENT_PARAMETERS* pd3dParameters, LPDIRECT3DTEXTURE9 pBackBufferTexture, UINT Pass);

public:
	DROPLETS_FALLING_SHADER(LPDIRECT3DDEVICE9 pd3dDevice, D3DPRESENT_PARAMETERS* pd3dParameters);
	DROPLETS_FALLING_SHADER(LPDIRECT3DDEVICE9 pd3dDevice, UINT Width, UINT Height);
	~DROPLETS_FALLING_SHADER();
	void Invalidate();
	void Restore();
	HRESULT Load(TCHAR* pNoiseTextureName);
	//�H��ǉ�����
	LPDIRECT3DTEXTURE9 Render(D3DPRESENT_PARAMETERS* pd3dParameters, LPDIRECT3DTEXTURE9 pBackBufferTexture, float AddDropletsPosX, float AddDropletsPosY, float Distortion);
	//�H��ǉ����Ȃ�
	LPDIRECT3DTEXTURE9 Render(D3DPRESENT_PARAMETERS* pd3dParameters, LPDIRECT3DTEXTURE9 pBackBufferTexture, float Distortion);
	BOOL IsOK();
	LPD3DXEFFECT GetEffect(){ return m_pEffect; };
};