/*
*	ヘッダーファイルのインクルード
*	ライブラリのリンク
*
*	2015/05/17		Y.Ozawa
*/

#pragma once

//----------------------------------------------------
//	define定義
//----------------------------------------------------
#define BUBBLE_MAX 600			//	泡の最大個数
#define ENEMY_MAX 30			//	敵の最大個数
#define EXPLOSION_MAX 30		//	爆発の最大個数
#define BULLET_MAX 30			//	プレイヤーの弾の最大個数
#define BOMB_MAX 100			//	プレイヤーの爆弾の最大個数
#define RAZER_MAX 200			//	プレイヤーのレーザーの最大個数

//----------------------------------------------------
//	インクルードファイル
//----------------------------------------------------

#include <Windows.h>

//	DirectX関係
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

//	Application
#include "Application.h"
//	Graphic
#include "Graphic.h"
#include "Model.h"
#include "Texture.h"
#include "Light.h"
#include "Camera.h"
//	Shader


//	Object
#include "Bullet.h"
#include "Bomb.h"
#include "Razer.h"
#include "Explosion.h"

//	Common
#include "Common.h"
//	Debug
#include "Debug.h"

//----------------------------------------------------
//	ライブラリ
//----------------------------------------------------

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


//----------------------------------------------------
//	マクロの定義
//----------------------------------------------------

//コンソールを開く
#define OpenConsole() {\
	AllocConsole();\
	FILE *fp;\
	freopen_s(&fp, "CONOUT$", "w", stdout);\
	freopen_s(&fp, "CONIN$", "r", stdin);\
}
//コンソールを閉じる
#define CloseConsole() {\
	FreeConsole();\
}
//ポインタをリリースする
#define SafeRelease(p) {\
	if (p)\
			{\
		p->Release();\
		p = nullptr;\
			}\
}

