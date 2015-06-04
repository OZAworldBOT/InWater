/*
*	�w�b�_�[�t�@�C���̃C���N���[�h
*	���C�u�����̃����N
*
*	2015/05/17		Y.Ozawa
*/

#pragma once

//----------------------------------------------------
//	define��`
//----------------------------------------------------
#define BUBBLE_MAX 400			//	�A�̍ő��
#define POINT_MAX 100			//	�^�C�g����ʂ̖A�̍ő��
#define ENEMY_MAX 30			//	�G�̍ő��
#define EXPLOSION_MAX 80		//	�����̍ő��
#define BULLET_MAX 30			//	�v���C���[�̒e�̍ő��
#define BOMB_MAX 50				//	�v���C���[�̔��e�̍ő��
#define RAZER_MAX 150			//	�v���C���[�̃��[�U�[�̍ő��

//----------------------------------------------------
//	�C���N���[�h�t�@�C��
//----------------------------------------------------

#include <Windows.h>

//	DirectX�֌W
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
#include "UPrimitive.h"
#include "DropletsFallingShader.h"

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
//	���C�u����
//----------------------------------------------------

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")


//----------------------------------------------------
//	�}�N���̒�`
//----------------------------------------------------

//�R���\�[�����J��
#define OpenConsole() {\
	AllocConsole();\
	FILE *fp;\
	freopen_s(&fp, "CONOUT$", "w", stdout);\
	freopen_s(&fp, "CONIN$", "r", stdin);\
}
//�R���\�[�������
#define CloseConsole() {\
	FreeConsole();\
}
//�|�C���^�������[�X����
#define SafeRelease(p) {\
	if (p)\
			{\
		p->Release();\
		p = nullptr;\
			}\
}

