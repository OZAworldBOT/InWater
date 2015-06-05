/*
*	�A�v���P�[�V�����̏��������s���N���X
*
*	2015/05/17		Y.Ozawa
*/

#include "Lib.h"
#include <crtdbg.h>
#include <cassert>

LPDIRECT3DDEVICE9		d3dDevice = 0;
LPDIRECTINPUTDEVICE8	dinputDevice = 0;
D3DPRESENT_PARAMETERS	presentParam;

DWORD FloatToDWORD(float val){ return *((DWORD *)&val); }

//�f�t�H���g�R���X�g���N�^
Application::Application()
{
	DebugLog("�A�v���P�[�V�����𐶐����܂���\n");

	//���������[�N�����o����
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//�A�v���P�[�V�����̏�����
	Initialize(win_title, Rect(0, 0, 640, 480), false, nullptr, 0);
}

//�R���X�g���N�^
Application::Application(char* win_title, RECT win_rect, bool win_fullscreen, HINSTANCE hInst, int cmdShow)
{
	DebugLog("�A�v���P�[�V�����𐶐����܂���\n");

	//���������[�N�����o����
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//�A�v���P�[�V�����̏�����
	Initialize(win_title, win_rect, win_fullscreen, hInst, cmdShow);

	//	�����^�C�����[�h�̃`�F�b�N
#ifndef _DEBUG
	Check();
#endif
}

//�f�X�g���N�^
Application::~Application()
{
	DebugLog("�A�v���P�[�V������������܂���\n");

	//�A�v���P�[�V�����̏I������
	Release();
}

void Application::Initialize(char* win_title, RECT win_rect, bool win_fullscreen, HINSTANCE hInst, int cmdShow)
{
	//	�����������o�[�ϐ��ɕۑ�����
	Application::win_title = win_title;
	Application::win_rect = win_rect;
	Application::win_fullscreen = win_fullscreen;

	//�E�B���h�E�̏�����
	if (!InitWindow(hInst, cmdShow))
		DebugAlert("�E�B���h�E��\n���傫���ł��܂���ł���");

	//Direct3D�̏�����
	if (!InitDirect3d())
		DebugAlert("Direct3D��\n�������ł��܂���ł����B");

	//�v���[���e�[�V�����p�����[�^�̏�����
	if (!InitPresentParam())
		DebugAlert("�v���[���e�[�V�����p�����[�^��\n�������ł��܂���ł����B");

	//Direct3D�f�o�C�X�̏�����
	if (!InitD3dDevice())
		DebugAlert("Direct3D�f�o�C�X��\n�������ł��܂���ł����B");

	//DirectInput�̏�����
	if (!InitDirectInput(hInst))
		DebugAlert("DirectInput��\n�������ł��܂���ł����B");

	//DirectInput�f�o�C�X�̏�����
	if (!InitDinputDevice())
		DebugAlert("DirectInput�f�o�C�X��\n�������ł��܂���ł����B");

	DebugLog("�A�v���P�[�V���������������܂����B\n");
}

//�A�v���P�[�V�����̉��
void Application::Release()
{
	//�f�o�b�O�r���h���Ƀr�[�v����炷
#if defined (DEBUG) | defined(_DEBUG)
	MessageBeep(MB_OK);
#endif

	//�R���\�[�������
	CloseConsole();

	//���f���̃e���v���[�g��j������
	SafeRelease(d3dDevice);
	SafeRelease(direct3d);
	SafeRelease(directInput);
}

//���[�v����
bool Application::Loop()
{
	MSG msg;

	//�O�̃t���[���̕`����I������
	d3dDevice->EndScene();

	//�O�̃t���[���̕`��𔽉f����
	d3dDevice->Present(0, 0, 0, 0);

	//Windows���b�Z�[�W����������
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		//�E�B���h�E�����
		if (WM_QUIT == msg.message)
			return false;
	}

	//��ʂ̃N���A
	if (FAILED(d3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 102, 204), 1.0f, 0)))
		return false;

	//���̃t���[���̕`����J�n����
	d3dDevice->BeginScene();

	return true;
}

//�E�B���h�E�v���V�[�W��
LRESULT CALLBACK Application::winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

//�E�B���h�E�̏�����
bool Application::InitWindow(HINSTANCE hInst, int cmdShow)
{
	//�E�B���h�E�N���X�̏�����
	WNDCLASSEX winClass;
	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.style = 0;
	winClass.lpfnWndProc = winProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = hInst;
	winClass.hIcon = nullptr;
	winClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	winClass.hbrBackground = nullptr;
	winClass.lpszMenuName = nullptr;
	winClass.lpszClassName = "GameWindow";
	winClass.hIconSm = nullptr;

	//�E�B���h�E�N���X�̓o�^
	if (!RegisterClassEx(&winClass))
		return false;

	//�E�B���h�E���쐬
	if (win_fullscreen)
	{
		hWnd = CreateWindow(winClass.lpszClassName, win_title, WS_OVERLAPPEDWINDOW,
			0, 0, win_rect.right, win_rect.bottom,
			nullptr, nullptr, hInst, nullptr);
	}
	else
	{
		AdjustWindowRect(&win_rect, WS_OVERLAPPEDWINDOW, false);
		hWnd = CreateWindow(winClass.lpszClassName, win_title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, win_rect.right - win_rect.left, win_rect.bottom - win_rect.top,
			nullptr, nullptr, hInst, nullptr);
	}

	if (!hWnd)
		return false;

	if (FAILED(ShowWindow(hWnd, cmdShow)))
		return false;

	ValidateRect(hWnd, 0);

	return true;
}

//Direct3D�̏�����
bool Application::InitDirect3d()
{
	//Direct3D9�I�u�W�F�N�g�̍쐬
	if ((direct3d = Direct3DCreate9(D3D_SDK_VERSION)) == 0)
		return false;

	return true;
}

//�v���[���e�[�V�����p�����[�^�̏�����
bool Application::InitPresentParam()
{
	D3DDISPLAYMODE displayMode;

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(direct3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode)))
		return false;

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^��������
	ZeroMemory(&presentParam, sizeof(D3DPRESENT_PARAMETERS));
	presentParam.BackBufferCount = 1;
	//�t���X�N���[���̏ꍇ
	if (win_fullscreen)
	{
		presentParam.Windowed = false;						//�t���X�N���[���\���̎w��
		presentParam.BackBufferWidth = win_rect.right;		//�t���X�N���[�����̉���
		presentParam.BackBufferHeight = win_rect.bottom;	//�t���X�N���[�����̏c��
	}
	else
	{
		presentParam.Windowed = true;						// �E�C���h�E���\���̎w��
	}
	presentParam.BackBufferFormat = displayMode.Format;	//�J���[���[�h�̎w��
	presentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParam.EnableAutoDepthStencil = true;			//�G���[�΍�
	presentParam.AutoDepthStencilFormat = D3DFMT_D16;		//�G���[�΍�

	return true;
}


//Direct3D�f�o�C�X�̏�����
bool Application::InitD3dDevice()
{
	//�f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	//�`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
		//��L�̐ݒ肪���s������
		//�`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
			//��L�̐ݒ肪���s������
			//�`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
				return false;

	//�����_�����O�X�e�[�^�X�̐ݒ�
	d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//���ʕ`�惂�[�h�̐ݒ�
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);				//Z��r���s��
	d3dDevice->SetRenderState(D3DRS_DITHERENABLE, true);		//�f�B�U�����O���s��

	// �|�C���g�X�v���C�g�̐ݒ�
	d3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// �|�C���g�X�P�[���̌W��
	d3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	d3dDevice->SetRenderState(D3DRS_POINTSIZE, FloatToDWORD(1.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));

	return true;
}

//DirectInput�̏�����
bool Application::InitDirectInput(HINSTANCE hInstance)
{
	//DirectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr)))
		return false;

	return true;
}

//DirectInput�f�o�C�X�̏�����
bool Application::InitDinputDevice()
{
	//DirectInput�f�o�C�X���쐬
	if (FAILED(directInput->CreateDevice(GUID_SysMouse, &dinputDevice, nullptr)))
		return false;

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(dinputDevice->SetDataFormat(&c_dfDIMouse2)))
		return false;

	//�������[�h��ݒ�
	if (FAILED(dinputDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return false;

	//	�����[�h��ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(dinputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		MessageBox(NULL, "�ݒ�Ɏ��s", "Direct Input Error", MB_OK);
		return false;
	}

	// ���͐���J�n
	dinputDevice->Acquire();

	return true;
}

int Application::Check()
{
	//	�����[�X�R���p�C���̂Ƃ���Direct3D�����^�C�������e�[�����[�h�̂Ƃ��̂ݎ��s�\�Ƃ���
	//	�����^�C�����[�h���`�F�b�N����
	IDirect3DQuery9** ppQuery = NULL;
	if (SUCCEEDED(d3dDevice->CreateQuery(D3DQUERYTYPE_VERTEXSTATS, ppQuery)))
	{
		::MessageBox(hWnd, "Direct3D�����^�C�����f�o�b�O���[�h�ł��B���e�[�����[�h�ɕύX���Ă��������B", "���s���G���[", MB_OK);
		::DestroyWindow(hWnd);
		return 0;
	}

}

























