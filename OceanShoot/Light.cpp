/*
*	光源を扱うクラス
*
*	2015/05/18		Y.Ozawa
*/

#include "Lib.h"

extern LPDIRECT3DDEVICE9 d3dDevice;

//	デフォルトコンストラクタ
Light::Light()
{
	//	ライトの設定
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	light.Ambient.r = 1.0f;
	light.Ambient.g = 1.0f;
	light.Ambient.b = 1.0f;
	light.Specular.r = 1.0f;
	light.Specular.g = 1.0f;
	light.Specular.b = 1.0f;

	D3DXVECTOR3 lightAngle;
	lightAngle = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &lightAngle);

	d3dDevice->SetLight(0, &light);
	d3dDevice->LightEnable(0, true);
	d3dDevice->SetRenderState(D3DRS_LIGHTING, true);
	d3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00111111);
	d3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
}

//	デストラクタ
Light::~Light()
{

}



