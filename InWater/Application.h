/*
*	�A�v���P�[�V�����̏��������s���N���X
*
*	2015/05/17		Y.Ozawa
*/

#pragma once

//	�A�v���P�[�V�������s���𐧌䂷��N���X
class Application
{
private:
	HWND	hWnd;
	char*	win_title;
	RECT	win_rect;
	bool	win_fullscreen;

	LPDIRECT3D9				direct3d;
	LPDIRECTINPUT8			directInput;

public:
	//�f�t�H���g�R���X�g���N�^
	Application();

	//�R���X�g���N�^
	Application(char* win_title, RECT win_rect, bool win_fullscreen, HINSTANCE hInst, int cmdShow);


	//�f�X�g���N�^
	~Application();

	//������
	void Initialize(char* win_title, RECT win_rect, bool win_fullscreen, HINSTANCE hInst, int cmdShow);

	//���
	void Release();

	//���[�v����
	bool Loop();

private:
	//�E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//�E�B���h�E�̏�����
	bool InitWindow(HINSTANCE hInst, int cmdShow);

	//Direct3D�̏�����
	bool InitDirect3d();

	//�v���[���e�[�V�����̏�����
	bool InitPresentParam();

	//Direct3D�f�o�C�X�̏�����
	bool InitD3dDevice();

	//DirectInput�̏�����
	bool InitDirectInput(HINSTANCE hInst);

	//DirectInput�f�o�C�X�̏�����
	bool InitDinputDevice();

	//	�����^�C�����[�h�̃`�F�b�N
	int Check();
};

