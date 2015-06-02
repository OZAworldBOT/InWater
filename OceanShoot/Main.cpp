/*
*	�A�v���P�[�V�����̃G���g���[�|�C���g�ƃ��C�����[�v
*
*	2015/05/18		Y.Ozawa
*/

#include "Lib.h"
#include <memory>
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"

using namespace std;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int cmdShow)
{
	//	�R���\�[�����J��
#if defined (DEBUG) | defined(_DEBUG)
	OpenConsole();
#endif


	//	�A�v���P�[�V�������s�����\�z
	unique_ptr<Application> app(new Application("�w���Ń|���I�K���I�I", Rect(0, 0, 800, 600), false, hInst, cmdShow));
	unique_ptr<Light> light(new Light());
	unique_ptr<Player> player(new Player());
	unique_ptr<Enemy> enemy(new Enemy());
	unique_ptr<Stage> stage(new Stage());

	RECT recDisplay;
	HWND hDeskWnd;
	hDeskWnd = GetDesktopWindow();

	GetWindowRect(hDeskWnd, &recDisplay);



	while (app->Loop())
	{
		//-----------------------
		//	�v���C���[
		//-----------------------
		player->Draw();
		player->Shot();
		player->Move();

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


