#pragma once

class D3D2DSQUARE
{
private:
	//���_�t�H�[�}�b�g
	DWORD D3D2DFVF;

	typedef struct _D3D2DVERTEX
	{
		float x, y, z, rhw;   //���_���W
		DWORD color;          //���_�J���[
		float tu, tv;         //�e�N�Z��
	}D3D2DVERTEX;

	LPDIRECT3DVERTEXBUFFER9 m_pd3d2DVertex;
	LPDIRECT3DDEVICE9 m_d3dDevice;

	//�Q�c�|���S���̃T�C�Y
	UINT m_Width, m_Height;

public:
	D3D2DSQUARE(LPDIRECT3DDEVICE9 pd3dDevice, D3DPRESENT_PARAMETERS* pd3dParameters);
	D3D2DSQUARE(LPDIRECT3DDEVICE9 pd3dDevice, UINT Width, UINT Height);
	~D3D2DSQUARE();
	void Invalidate();
	//2D�|���S���̃T�C�Y���擾
	UINT GetWidth();
	UINT GetHeight();
	//2D�|���S���̃T�C�Y��ύX
	HRESULT Resize(UINT Width, UINT Height);
	HRESULT Resize(D3DPRESENT_PARAMETERS* pd3dParameters);
	//������
	HRESULT Load();
	//2D�|���S���������_�����O
	void Render();
};