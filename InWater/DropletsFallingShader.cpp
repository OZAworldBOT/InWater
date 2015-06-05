#include "Lib.h"

DROPLETS_FALLING_SHADER::DROPLETS_FALLING_SHADER(LPDIRECT3DDEVICE9 pd3dDevice, D3DPRESENT_PARAMETERS* pd3dParameters) : D3D2DSQUARE(pd3dDevice, pd3dParameters)
{
	m_d3dDevice = pd3dDevice;

	for (int i = 0; i<2; i++)
	{
		m_pDropletsTexture[i] = NULL;
		m_pDropletsSurface[i] = NULL;
		m_pDropletsLocusTexture[i] = NULL;
		m_pDropletsLocusSurface[i] = NULL;
	}

	m_pBlurTexture = NULL;
	m_pBlurSurface = NULL;

	m_pNormalTexture = NULL;
	m_pNormalSurface = NULL;

	m_pNoiseTexture = NULL;

	m_pEffect = NULL;

	m_RenderTargetIndex = 0;
}

DROPLETS_FALLING_SHADER::DROPLETS_FALLING_SHADER(LPDIRECT3DDEVICE9 pd3dDevice, UINT Width, UINT Height) : D3D2DSQUARE(pd3dDevice, Width, Height)
{
	m_d3dDevice = pd3dDevice;

	for (int i = 0; i<2; i++)
	{
		m_pDropletsTexture[i] = NULL;
		m_pDropletsSurface[i] = NULL;
		m_pDropletsLocusTexture[i] = NULL;
		m_pDropletsLocusSurface[i] = NULL;
	}

	m_pBlurTexture = NULL;
	m_pBlurSurface = NULL;

	m_pNormalTexture = NULL;
	m_pNormalSurface = NULL;

	m_pNoiseTexture = NULL;

	m_pEffect = NULL;

	m_RenderTargetIndex = 0;
}

DROPLETS_FALLING_SHADER::~DROPLETS_FALLING_SHADER()
{
	Invalidate();

	SafeRelease(m_pNoiseTexture);

	SafeRelease(m_pEffect);
}

//デバイスがロストしたときにコールする関数
void DROPLETS_FALLING_SHADER::Invalidate()
{
	if (m_pEffect)
		m_pEffect->OnLostDevice();

	for (int i = 0; i<2; i++)
	{
		SafeRelease(m_pDropletsTexture[i]);
		SafeRelease(m_pDropletsSurface[i]);
		SafeRelease(m_pDropletsLocusTexture[i]);
		SafeRelease(m_pDropletsLocusSurface[i]);
	}

	SafeRelease(m_pBlurTexture);
	SafeRelease(m_pBlurSurface);

	SafeRelease(m_pNormalTexture);
	SafeRelease(m_pNormalSurface);

	m_RenderTargetIndex = 0;
}

void DROPLETS_FALLING_SHADER::Restore()
{
	if (m_pEffect)
	{
		m_pEffect->OnResetDevice();


		for (int i = 0; i<2; i++)
		{
			D3DXCreateTexture(m_d3dDevice,
				(DWORD)D3D2DSQUARE::GetWidth(),
				(DWORD)D3D2DSQUARE::GetHeight(),
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				&m_pDropletsTexture[i]);
			m_pDropletsTexture[i]->GetSurfaceLevel(0, &m_pDropletsSurface[i]);

			D3DXCreateTexture(m_d3dDevice,
				(DWORD)D3D2DSQUARE::GetWidth(),
				(DWORD)D3D2DSQUARE::GetHeight(),
				1,
				D3DUSAGE_RENDERTARGET,
				D3DFMT_A8R8G8B8,
				D3DPOOL_DEFAULT,
				&m_pDropletsLocusTexture[i]);
			m_pDropletsLocusTexture[i]->GetSurfaceLevel(0, &m_pDropletsLocusSurface[i]);
		}

		D3DXCreateTexture(m_d3dDevice,
			(DWORD)D3D2DSQUARE::GetWidth(),
			(DWORD)D3D2DSQUARE::GetHeight(),
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pBlurTexture);
		m_pBlurTexture->GetSurfaceLevel(0, &m_pBlurSurface);

		D3DXCreateTexture(m_d3dDevice,
			(DWORD)D3D2DSQUARE::GetWidth(),
			(DWORD)D3D2DSQUARE::GetHeight(),
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_pNormalTexture);
		m_pNormalTexture->GetSurfaceLevel(0, &m_pNormalSurface);

		LPDIRECT3DSURFACE9 OldSurface = NULL;
		m_d3dDevice->GetRenderTarget(0, &OldSurface);

		LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
		m_d3dDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

		//デプスバッファは使用しないので無効にする
		m_d3dDevice->SetDepthStencilSurface(NULL);

		for (int i = 0; i<2; i++)
		{
			m_d3dDevice->SetRenderTarget(0, m_pDropletsSurface[i]);
			m_d3dDevice->SetRenderTarget(1, m_pDropletsLocusSurface[i]);
			m_d3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET, 0x0, 1.0f, 0L);
		}
		m_d3dDevice->SetRenderTarget(0, OldSurface);
		SafeRelease(OldSurface);

		m_d3dDevice->SetDepthStencilSurface(OldDepthStencilSurface);
		SafeRelease(OldDepthStencilSurface);

		m_d3dDevice->SetRenderTarget(1, NULL);
	}
}

HRESULT DROPLETS_FALLING_SHADER::Load(TCHAR* pNoiseTextureName)
{
	D3DCAPS9 caps;
	HRESULT hr;

	m_d3dDevice->GetDeviceCaps(&caps);
	if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1) && caps.PixelShaderVersion >= D3DPS_VERSION(3, 0))
	{
		//シェーダーの初期化
		LPD3DXBUFFER pErr = NULL;
		hr = D3DXCreateEffectFromFile(m_d3dDevice, "Shader/DropletsFallingShader.fx", NULL, NULL, 0, NULL, &m_pEffect, &pErr);
		if (FAILED(hr))
			return -1;

		m_pTechnique = m_pEffect->GetTechniqueByName("TShader");
		m_pTexOffset = m_pEffect->GetParameterByName(NULL, "TexOffset");
		m_pAddDropletsPos = m_pEffect->GetParameterByName(NULL, "AddDropletsPos");
		m_pDistortion = m_pEffect->GetParameterByName(NULL, "Distortion");

		D3DXVECTOR2 Size;
		Size.x = 1.0f / D3D2DSQUARE::GetWidth();
		Size.y = 1.0f / D3D2DSQUARE::GetHeight();
		m_pEffect->SetFloatArray(m_pTexOffset, (float*)&Size, sizeof(D3DXVECTOR2));

		m_pEffect->SetTechnique(m_pTechnique);
	}

	else
	{
		return -2;
	}

	hr = D3D2DSQUARE::Load();
	if (FAILED(hr))
		return -3;

	hr = D3DXCreateTextureFromFileEx(m_d3dDevice,
		pNoiseTextureName,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0x0,
		NULL,
		NULL,
		&m_pNoiseTexture);
	if (FAILED(hr))
		return -4;

	return S_OK;
}

LPDIRECT3DTEXTURE9 DROPLETS_FALLING_SHADER::UpdateDroplets(D3DPRESENT_PARAMETERS* pd3dParameters, LPDIRECT3DTEXTURE9 pBackBufferTexture, UINT Pass)
{
	if (m_pEffect)
	{
		D3DVIEWPORT9 OldViewport, NewViewport;

		//****************************************************************
		// (STEP1)　滴マップと滴の軌跡マップを更新
		//****************************************************************

		//ビューポートを滴マップのサイズに合わせる
		m_d3dDevice->GetViewport(&OldViewport);
		CopyMemory(&NewViewport, &OldViewport, sizeof(D3DVIEWPORT9));
		NewViewport.Width = (DWORD)D3D2DSQUARE::GetWidth();
		NewViewport.Height = (DWORD)D3D2DSQUARE::GetHeight();
		m_d3dDevice->SetViewport(&NewViewport);

		//現在のレンダーターゲットサーフェイスを取得
		LPDIRECT3DSURFACE9 OldSurface = NULL;
		m_d3dDevice->GetRenderTarget(0, &OldSurface);

		m_RenderTargetIndex = 1 - m_RenderTargetIndex;

		//レンダーターゲットをセットする
		m_d3dDevice->SetRenderTarget(0, m_pDropletsSurface[1 - m_RenderTargetIndex]);
		m_d3dDevice->SetRenderTarget(1, m_pDropletsLocusSurface[1 - m_RenderTargetIndex]);

		LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
		m_d3dDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

		//デプスバッファを使用しないので無効にする
		m_d3dDevice->SetDepthStencilSurface(NULL);

		//滴マップをテクスチャーステージ0にセットする
		m_d3dDevice->SetTexture(0, m_pDropletsTexture[m_RenderTargetIndex]);

		//滴の軌跡マップをテクスチャーステージ1にセットする
		m_d3dDevice->SetTexture(1, m_pDropletsLocusTexture[m_RenderTargetIndex]);

		//ノイズマップをステージ2にセットする
		m_d3dDevice->SetTexture(2, m_pNoiseTexture);

		m_d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		m_d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		m_d3dDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		m_d3dDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		m_d3dDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
		m_d3dDevice->SetSamplerState(2, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_d3dDevice->SetSamplerState(2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_d3dDevice->SetSamplerState(2, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		m_d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(Pass);
		D3D2DSQUARE::Render();   //2Dスプライトのレンダリング
		m_pEffect->EndPass();
		m_pEffect->End();

		m_d3dDevice->SetRenderTarget(1, NULL);
		m_d3dDevice->SetTexture(1, NULL);
		m_d3dDevice->SetTexture(2, NULL);

		//****************************************************************
		// (STEP2)　ブラーを適応する
		//****************************************************************

		//ブラーマップをレンダーターゲットにする
		m_d3dDevice->SetRenderTarget(0, m_pBlurSurface);

		//滴の軌跡マップをセットする
		m_d3dDevice->SetTexture(0, m_pDropletsLocusTexture[m_RenderTargetIndex]);

		m_d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		//滴の軌跡マップをぼかす
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(2);
		D3D2DSQUARE::Render();   //2Dスプライトのレンダリング
		m_pEffect->EndPass();
		m_pEffect->End();


		//****************************************************************
		// (STEP3)　法線マップを作成する
		//****************************************************************

		//重いので使用しない
		////法線マップを作成
		//D3DXComputeNormalMap( m_pNormalTexture,
		//                      m_pBlurTexture,
		//                      NULL,
		//                      0,
		//                      D3DX_CHANNEL_RED,
		//                      0.6f
		//                    );

		//法線マップをレンダーターゲットにする
		m_d3dDevice->SetRenderTarget(0, m_pNormalSurface);

		//ブラーマップをセットする
		m_d3dDevice->SetTexture(0, m_pBlurTexture);

		//法線マップを作成する
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(3);
		D3D2DSQUARE::Render();   //2Dスプライトのレンダリング
		m_pEffect->EndPass();
		m_pEffect->End();


		//****************************************************************
		// (STEP4)　バックバッファのイメージをゆがませる
		//****************************************************************

		//バックバッファに戻す
		m_d3dDevice->SetRenderTarget(0, OldSurface);
		SafeRelease(OldSurface);

		//ビューポートを戻す
		m_d3dDevice->SetViewport(&OldViewport);

		//法線マップをセットする
		m_d3dDevice->SetTexture(0, m_pNormalTexture);

		//バックバッファのイメージをセットする
		m_d3dDevice->SetTexture(1, pBackBufferTexture);

		//2Dスプライトの解像度を退避する
		UINT OldWidth, OldHeight;
		OldWidth = D3D2DSQUARE::GetWidth();
		OldHeight = D3D2DSQUARE::GetHeight();

		//2Dスプライトのサイズをバックバッファの解像度にリサイズ
		D3D2DSQUARE::Resize(pd3dParameters);

		m_d3dDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_d3dDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

		//ゆがみを適応
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(4);
		D3D2DSQUARE::Render();   //2Dスプライトのレンダリング
		m_pEffect->EndPass();
		m_pEffect->End();

		//2Dスプライトを各種マップの更新に使用している解像度にリサイズ
		D3D2DSQUARE::Resize(OldWidth, OldHeight);

		//デプスバッファを戻す
		m_d3dDevice->SetDepthStencilSurface(OldDepthStencilSurface);
		SafeRelease(OldDepthStencilSurface);

		m_d3dDevice->SetTexture(1, NULL);

		m_d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_d3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		m_d3dDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_d3dDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	}

	//スクリーンショットをとるためにテクスチャーを返す
	//   return m_pDropletsTexture[m_RenderTargetIndex];
	//   return m_pDropletsLocusTexture[m_RenderTargetIndex];
	//   return m_pBlurTexture;
	return m_pNormalTexture;
}

//滴を追加する
LPDIRECT3DTEXTURE9 DROPLETS_FALLING_SHADER::Render(D3DPRESENT_PARAMETERS* pd3dParameters, LPDIRECT3DTEXTURE9 pBackBufferTexture, float AddDropletsPosX, float AddDropletsPosY, float Distortion)
{
	if (m_pEffect)
	{
		D3DXVECTOR2 Dropletspos = D3DXVECTOR2(AddDropletsPosX, AddDropletsPosY);
		m_pEffect->SetValue(m_pAddDropletsPos, &Dropletspos, sizeof(D3DXVECTOR2));

		m_pEffect->SetFloat(m_pDistortion, Distortion);

		return UpdateDroplets(pd3dParameters, pBackBufferTexture, 0);
	}

	return NULL;
}

//滴を追加しない
LPDIRECT3DTEXTURE9 DROPLETS_FALLING_SHADER::Render(D3DPRESENT_PARAMETERS* pd3dParameters, LPDIRECT3DTEXTURE9 pBackBufferTexture, float Distortion)
{
	if (m_pEffect)
	{
		m_pEffect->SetFloat(m_pDistortion, Distortion);

		return UpdateDroplets(pd3dParameters, pBackBufferTexture, 1);
	}

	return NULL;
}

BOOL DROPLETS_FALLING_SHADER::IsOK()
{
	if (m_pEffect)
		return TRUE;

	return FALSE;
}
