/*
*	テクスチャ
*
*	2015/05/19		Y.Ozawa
*/

#include "Lib.h"

extern LPDIRECT3DDEVICE9 d3dDevice;

//デフォルトコンストラクタ
Texture::Texture()
{
	texture = nullptr;
}

//コンストラクタ
Texture::Texture(const TCHAR *textureFileName)
{
	d3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	d3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	LoadTexture(textureFileName);
}

//デストラクタ
Texture::~Texture()
{
	SafeRelease(texture);
}

//メッシュをロードする
bool Texture::LoadTexture(const TCHAR *textureFileName)
{

	if (FAILED(D3DXCreateTextureFromFileEx(d3dDevice, textureFileName, 0, 0, 0, 0, D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED, D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR,
		D3DCOLOR_ARGB(255, 0, 0, 0), nullptr, nullptr, &texture)))
	{
		DebugAlert("%sを\n読み込めませんでした。\nファイル名やディレクトリを間違えていないかご確認ください。", textureFileName);
		return false;
	}

	return true;
}
