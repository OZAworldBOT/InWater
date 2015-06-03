#include "Lib.h"

D3D2DSQUARE::D3D2DSQUARE(LPDIRECT3DDEVICE9 pd3dDevice, D3DPRESENT_PARAMETERS* pd3dParameters)
{
	m_d3dDevice = pd3dDevice;

	//バックバッファのサイズで格納する
	m_Width = pd3dParameters->BackBufferWidth;
	m_Height = pd3dParameters->BackBufferHeight;

	D3D2DFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	m_pd3d2DVertex = NULL;
}

D3D2DSQUARE::D3D2DSQUARE(LPDIRECT3DDEVICE9 pd3dDevice, UINT Width, UINT Height)
{
	m_d3dDevice = pd3dDevice;

	m_Width = Width;
	m_Height = Height;

	D3D2DFVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	m_pd3d2DVertex = NULL;
}

D3D2DSQUARE::~D3D2DSQUARE()
{
	Invalidate();
}

void D3D2DSQUARE::Invalidate()
{
	SafeRelease(m_pd3d2DVertex);
}

HRESULT D3D2DSQUARE::Load()
{
	HRESULT hr = -1;

	Invalidate();

	//２Ｄポリゴンの作成
	hr = m_d3dDevice->CreateVertexBuffer(sizeof(D3D2DVERTEX) * 4, D3DUSAGE_WRITEONLY, D3D2DFVF, D3DPOOL_MANAGED, &m_pd3d2DVertex, NULL);
	if (FAILED(hr))
		return -1;

	//ポリゴンの頂点の座標を更新する
	hr = Resize(m_Width, m_Height);

	return hr;
}

UINT D3D2DSQUARE::GetWidth()
{
	return m_Width;
}

UINT D3D2DSQUARE::GetHeight()
{
	return m_Height;
}

HRESULT D3D2DSQUARE::Resize(D3DPRESENT_PARAMETERS* pd3dParameters)
{
	HRESULT hr = -1;

	m_Width = pd3dParameters->BackBufferWidth;
	m_Height = pd3dParameters->BackBufferHeight;

	hr = Resize(m_Width, m_Height);

	return hr;
}

HRESULT D3D2DSQUARE::Resize(UINT Width, UINT Height)
{
	HRESULT hr = -1;

	m_Width = Width;
	m_Height = Height;

	D3D2DVERTEX* vtx;
	hr = m_pd3d2DVertex->Lock(0, 0, (void**)&vtx, 0);
	if (FAILED(hr))
		return -3;

	//頂点の座標
	vtx[0].x = 0.0f;           vtx[0].y = 0.0f;            vtx[0].z = 0.0f; vtx[0].rhw = 1.0f;
	vtx[1].x = (float)m_Width; vtx[1].y = 0.0f;            vtx[1].z = 0.0f; vtx[1].rhw = 1.0f;
	vtx[2].x = 0.0f;           vtx[2].y = (float)m_Height; vtx[2].z = 0.0f; vtx[2].rhw = 1.0f;
	vtx[3].x = (float)m_Width; vtx[3].y = (float)m_Height; vtx[3].z = 0.0f; vtx[3].rhw = 1.0f;

	//テクセル
	vtx[0].tu = 0.0f + 0.5f / (float)m_Width; vtx[0].tv = 0.0f + 0.5f / (float)m_Height;
	vtx[1].tu = 1.0f + 0.5f / (float)m_Width; vtx[1].tv = 0.0f + 0.5f / (float)m_Height;
	vtx[2].tu = 0.0f + 0.5f / (float)m_Width; vtx[2].tv = 1.0f + 0.5f / (float)m_Height;
	vtx[3].tu = 1.0f + 0.5f / (float)m_Width; vtx[3].tv = 1.0f + 0.5f / (float)m_Height;

	//頂点カラー
	vtx[0].color = 0xFFFFFFFF;
	vtx[1].color = 0xFFFFFFFF;
	vtx[2].color = 0xFFFFFFFF;
	vtx[3].color = 0xFFFFFFFF;

	m_pd3d2DVertex->Unlock();

	hr = S_OK;

	return hr;
}

//２Ｄポリゴンをレンダリングする
void D3D2DSQUARE::Render()
{
	m_d3dDevice->SetFVF(D3D2DFVF);
	m_d3dDevice->SetStreamSource(0, m_pd3d2DVertex, 0, sizeof(D3D2DVERTEX));
	//▲ポリゴンを２枚使用
	m_d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}