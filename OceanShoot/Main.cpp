/*
*	�A�v���P�[�V�����̃G���g���[�|�C���g�ƃ��C�����[�v
*
*	2015/05/18		Y.Ozawa
*/

#include "Lib.h"
#include <memory>
#include "Main.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "Title.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int cmdShow)
{
	//	�R���\�[�����J��
#if defined (DEBUG) | defined(_DEBUG)
	OpenConsole();
#endif


	//	�A�v���P�[�V�������s�����\�z
	unique_ptr<Application> app(new Application("�I�[�V�����E�V���[�b�I�I", Rect(0, 0, 800, 600), false, hInst, cmdShow));
	unique_ptr<Light> light(new Light());
	unique_ptr<Title> title(new Title());
	unique_ptr<Stage> stage(new Stage());
	unique_ptr<Player> player(new Player());
	unique_ptr<Enemy> enemy(new Enemy());
	gameState = GAME_STATE_TITLE;
	 
	RECT recDisplay;
	HWND hDeskWnd;
	hDeskWnd = GetDesktopWindow();

	GetWindowRect(hDeskWnd, &recDisplay);



	while (app->Loop())
	{
		switch (gameState)
		{
		case GAME_STATE_TITLE:
			
			if (!GetAsyncKeyState(VK_RETURN))
			{
				title->View();
				title->Move();
				if (GetAsyncKeyState(VK_ESCAPE))
				{
					title->Release();
				}
			}
			else
			{ 
				title->Release();
				gameState = GAME_STATE_MAIN;
			}
			break;
			
		case GAME_STATE_MAIN:
			//-----------------------
			//	�v���C���[
			//-----------------------
			player->Move();
			player->View();
			player->Draw();

			//-----------------------
			//	�G
			//-----------------------
			enemy->Draw();
			enemy->Move();
			enemy->Hit();

			//-----------------------
			//	�X�e�[�W
			//-----------------------
			stage->View();

			player->Shot();



			break;
		case GAME_STATE_CLEAR:
			break;
		case GAME_STATE_FAILED:
			break;
		}

		//	�G�X�P�[�v�L�[����������Q�[���I��
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			break;
		}

		ShowCursor(FALSE);
		SetCursorPos(recDisplay.right / 2, recDisplay.bottom / 2);
	}

	return 0;
}


