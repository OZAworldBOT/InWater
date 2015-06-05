/*
*	�v���C���[�̃N���X
*
*	2015/05/27		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Player.h"


extern LPDIRECT3DDEVICE9 d3dDevice;
extern LPDIRECTINPUTDEVICE8 dinputDevice;
D3DXVECTOR3 inputState;					//	�v���C���[�̍��W���擾
D3DXVECTOR3 bulletState[BULLET_MAX];	//	�v���C���[�̒e�̍��W���擾
D3DXVECTOR3 bombState[BOMB_MAX];		//	�v���C���[�̔��e�̍��W���擾
D3DXVECTOR3 razerState[RAZER_MAX];		//	�v���C���[�̃��[�U�[�̍��W���擾

//	�R���X�g���N�^
Player::Player()
{
	camera = new Camera();
	player = new Graphic();

	model = new Model("Model/Heli.x");
	texture = new Texture("Texture/enemy1.png");

	bullet = new Bullet();
	bomb = new Bomb();
	razer = new Razer();
	explosion = new Explosion();

	pBullet = new Pbullet;
	pBomb = new Pbomb;
	pRazer = new Prazer;
	pExp = new Pexplosion;

	DebugLog("�v���C���[�𐶐����܂����B\n");

	InitPlayer();
	InitBullet();
	InitBomb();
	InitRazer();
	InitExplosion();
}

//	�f�X�g���N�^
Player::~Player()
{
	Release();
}

//	������
void Player::InitPlayer()
{
	Position = D3DXVECTOR3(1000, 10, 1000);
	Rotation = D3DXVECTOR3(0, D3DXToRadian(90), 0);
	Scale = D3DXVECTOR3(0.05, 0.05, 0.05);

	camera_Pos = D3DXVECTOR3(0, 15, -10);
	camera_Rot = D3DXVECTOR3(D3DXToRadian(10), 0, 0);

	DebugLog("�v���C���[�����������܂����B\n");
}

void Player::InitBullet()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		pBullet->Count[i] = 0;
		pBullet->Exist[i] = false;
		pBullet->flag = false;
		pBullet->death[i] = true;
	}
}

void Player::InitBomb()
{
	pBomb->Exist = false;
	pBomb->flag = false;
	pBomb->death = true;
	pBomb->Alpha = false;
	pBomb->Count = 0;
	pBomb->Size = 3.0f;
	for (int i = 0; i < BOMB_MAX; i++)
	{
		pBomb->Explosion_Exist[i] = false;
		pBomb->Explosion_Count[i] = 0;
		pBomb->Explosion_Death[i] = true;
		pBomb->Explosion_Size = 1.0f;
		pBomb->Explosion_Alpha = true;
	}
}

void Player::InitRazer()
{
	for (int i = 0; i < RAZER_MAX; i++)
	{
		pRazer->Exist[i] = false;
		pRazer->flag = false;
		pRazer->death[i] = true;
		pRazer->Count[i] = 0;
	}
}

void Player::InitExplosion()
{
	for (int i = 0; i < EXPLOSION_MAX; i++)
	{
		pExp->Exist[i] = false;
		pExp->flag = false;
		pExp->Count[i] = 0;
	}
}

//	�������
void Player::Release()
{
	delete camera;
	delete player;
	delete model;
	delete texture;
	delete bullet;
	delete bomb;
	delete razer;
	delete explosion;
	delete pBullet;
	delete pBomb;
	delete pRazer;
	delete pExp;

	DebugLog("�v���C���[��j�����܂����B\n");
}

//	����
void Player::Move()
{
	//�ړ�����	�L�[�{�[�h�ő���
	if (GetAsyncKeyState(0x57))
	{
		Position.x += sin(Rotation.y) * 1.0f;
		Position.z += cos(Rotation.y) * 1.0f;
	}
	if (GetAsyncKeyState(0x53))
	{
		Position.x -= sin(Rotation.y) * 1.0f;
		Position.z -= cos(Rotation.y) * 1.0f;
	}
	if (GetAsyncKeyState(0x41))
	{
		Position.x += sin(Rotation.y - D3DXToRadian(90)) * 1.0f;
		Position.z += cos(Rotation.y - D3DXToRadian(90)) * 1.0f;
	}
	if (GetAsyncKeyState(0x44))
	{
		Position.x += sin(Rotation.y + D3DXToRadian(90)) * 1.0f;
		Position.z += cos(Rotation.y + D3DXToRadian(90)) * 1.0f;
	}
	oldPlayerPos.x = Position.x;
	oldPlayerPos.z = Position.z;


	//�J�����ړ�		�}�E�X�̓����ɍ��킹��
	DIMOUSESTATE2 dims;

	if (SUCCEEDED(dinputDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
	{
		cameraAngle += dims.lX * 0.005f;
		if (cameraAngle > D3DX_PI)
		{
			cameraAngle -= 2 * D3DX_PI;
		}
		
		camera_Rot.x += dims.lY * 0.005f;
		if (camera_Rot.x < D3DXToRadian(5))
		{
			camera_Rot.x = D3DXToRadian(5);
		}
		if (camera_Rot.x > D3DXToRadian(5))
		{
			camera_Rot.x = D3DXToRadian(5);
		}

	}

	//�v���C���[���J�����̒��S�ɂƂ炦��
	camera_Pos.x = Position.x - 10 * sinf(cameraAngle);
	camera_Pos.z = Position.z - 10 * cosf(cameraAngle);

	camera_Rot.y += dims.lX * 0.005f;
	Rotation.y = camera_Rot.y + D3DXToRadian(0);
	oldPlayerRot.y = Rotation.y;
	oldCameraRot.x = camera_Rot.x;

	inputState = oldPlayerPos;
}

//	�V���b�g
void Player::Shot()
{
	BulletShot();
	BombShot();
	RazerShot();
}

//	�`��
void Player::Draw()
{
	player->DrawModelTexture(Position, Rotation, Scale, *model, *texture, false);
}

void Player::View()
{
	camera->View(camera_Pos, camera_Rot);
}
//-----------------------------------------------------------------------------------------------------------
//	�ȉ��v���C���[�̍U���֘A�̓�����L�q
//-----------------------------------------------------------------------------------------------------------

//	�e�ۂ̃V���b�g
void Player::BulletShot()
{
	if (GetAsyncKeyState(VK_LBUTTON) && pBullet->flag == false && pRazer->flag == false)
	{
		/*
		*	���N���b�N����bullet_flag��false�̂Ƃ��ɏ�������������
		*/
		srand((unsigned int)time(NULL));

		for (int i = 0; i < BULLET_MAX; i++)
		{
			pBullet->Pos[i].x = oldPlayerPos.x;
			pBullet->Pos[i].z = oldPlayerPos.z;
			pBullet->Rot[i].y = oldPlayerRot.y;
			pBullet->Rot[i].x = oldCameraRot.x;

			pBullet->Exist[i] = true;
		}
	}

	for (int i = 0; i < BULLET_MAX; i++)
	{
		/*
		*	�e�̏������L�q	�A�̂悤�Ɍ����邽�ߗ�����D3DXVec3Normalize���g�p
		*/
		if (pBullet->Exist[i] == true)
		{
			pBullet->Count[i]++;
			if (pBullet->Count[i] < 30)
			{
				pBullet->flag = true;
				pBullet->death[i] = false;
				D3DXVec3Normalize(&pBullet->Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 40));
				pBullet->Pos[i].x += (sin(pBullet->Rot[i].y) * (2.0f + i) * cos(pBullet->Rot[i].x)) + pBullet->Accel[i].x * 0.7f;
				pBullet->Pos[i].z += (cos(pBullet->Rot[i].y) * (2.0f + i) * cos(pBullet->Rot[i].x)) + pBullet->Accel[i].z * 0.7f;
				pBullet->Pos[i].y = Position.y + pBullet->Accel[i].y * 0.5f;

				if (pBullet->death[i] == false)
				{
					bullet->Draw(&pBullet->Pos[i]);
				}

				Hit();
			}
			if (pBullet->Count[i] > 30)
			{
				pBullet->flag = false;
				pBullet->death[i] = true;
				pBullet->Exist[i] = false;
				pBullet->Count[i] = 0;
			}
			if (pBullet->death[i] == true)
			{
				pBullet->Pos[i].y = 1000.0f;
				pBullet->oldPos[i] = pBullet->Pos[i];
				//CreateExp(&pBullet->oldPos[i], BULLET_MAX);
			}
		}
		bulletState[i] = pBullet->Pos[i];
	}

}

//	���e�̃V���b�g
void Player::BombShot()
{
	if (GetAsyncKeyState(VK_RBUTTON) && pBomb->flag == false && pRazer->flag == false)
	{
		srand((unsigned int)time(NULL));

		/*
		*	���N���b�N����bomb_flag��false�̂Ƃ��ɏ�������������
		*/
		pBomb->Pos.x = oldPlayerPos.x;
		pBomb->Pos.z = oldPlayerPos.z + 0.5f;
		pBomb->Pos.y = Position.y + 2.0f;
		pBomb->Rot.y = oldPlayerRot.y;
		pBomb->Rot.x = oldCameraRot.x;

		pBomb->Exist = true;
	}
	if (pBomb->Exist == true)
	{
		pBomb->Count++;
		if (pBomb->Count < 60)
		{
			pBomb->flag = true;
			pBomb->death = false;
			timeCount += 0.5f;
			pBomb->Pos.y -= 1.0f + (timeCount * sin(pBomb->Rot.x) - 2.0f);
			pBomb->Pos.x += sin(pBomb->Rot.y) * 2.0f * cos(pBomb->Rot.x);
			pBomb->Pos.z += cos(pBomb->Rot.y) * 2.0f * cos(pBomb->Rot.x);
			pBomb->oldPos = pBomb->Pos;
			bomb->Draw(&pBomb->Pos, pBomb->Size, pBomb->Alpha);
		}
		if (pBomb->Count > 60)
		{
			pBomb->death = true;
		}
		for (int i = 0; i < BOMB_MAX; i++)
		{
			if (pBomb->Pos.y < 0)
			{
				pBomb->Explosion_Exist[i] = true;
				pBomb->Explosion_Pos[i] = pBomb->oldPos;
				pBomb->MinRange = D3DXVECTOR3(pBomb->Explosion_Pos[i].x - 10, -10, pBomb->Explosion_Pos[i].z - 10);
				pBomb->MaxRange = D3DXVECTOR3(pBomb->Explosion_Pos[i].x + 10, 10, pBomb->Explosion_Pos[i].z + 10);
			}
			if (pBomb->Explosion_Exist[i] == true)
			{
				pBomb->Explosion_Death[i] = false;
				pBomb->Explosion_Count[i]++;
				D3DXVECTOR3 range = pBomb->MaxRange - pBomb->MinRange;
				pBomb->Explosion_Pos[i].x = (float)((double)rand() / RAND_MAX * range.x) + pBomb->MinRange.x;
				pBomb->Explosion_Pos[i].z = (float)((double)rand() / RAND_MAX * range.z) + pBomb->MinRange.z;
				pBomb->Explosion_Pos[i].y = (float)((double)rand() / RAND_MAX * range.y) + pBomb->MinRange.y;

				if (pBomb->Explosion_Count[i] < 30)
				{
					D3DXVec3Normalize(&pBomb->Explosion_Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 40));
					pBomb->Explosion_Pos[i].x += pBomb->Explosion_Accel[i].x * 50.0f;
					pBomb->Explosion_Pos[i].y += rand() % 10 * 0.0008f;
					pBomb->Explosion_Pos[i].z += pBomb->Explosion_Accel[i].z * 50.0f;
					bomb->Draw(&pBomb->Explosion_Pos[i], pBomb->Explosion_Size, pBomb->Explosion_Alpha);
					Hit();
				}
				if (pBomb->Explosion_Count[i] > 30)
				{
					pBomb->Explosion_Count[i] = 0;
					pBomb->Explosion_Exist[i] = false;
					pBomb->Explosion_Death[i] = true;
					pBomb->flag = false;
					pBomb->Exist = false;
				}
				if (pBomb->Explosion_Death[i] == true)
				{
					pBomb->Explosion_Pos[i].y = 1000.0f;
					pBomb->Explosion_Pos[i].z = 0;
					pBomb->oldExplosionPos[i] = pBomb->Explosion_Pos[i];
					timeCount = 0;
					pBomb->Count = 0;
				}
			}
			bombState[i] = pBomb->Explosion_Pos[i];
		}
	}
}

//	���[�U�[�̃V���b�g
void Player::RazerShot()
{
	if (GetAsyncKeyState(VK_SPACE) && pRazer->flag == false)
	{
		srand((unsigned int)time(NULL));

		/*
		*	���N���b�N����bomb_flag��false�̂Ƃ��ɏ�������������
		*/
		for (int i = 0; i < RAZER_MAX; i++)
		{
			pRazer->Pos[i].x = oldPlayerPos.x;
			pRazer->Pos[i].z = oldPlayerPos.z + 0.5f;
			pRazer->Pos[i].y = Position.y + 2.0f;
			pRazer->Rot[i].y = oldPlayerRot.y;
			pRazer->Rot[i].x = oldCameraRot.x;

			pRazer->Exist[i] = true;
		}
	}
	for (int i = 0; i < RAZER_MAX; i++)
	{
		if (pRazer->Exist[i] == true)
		{
			pRazer->Count[i]++;
			if (pRazer->Count[i] < 500)
			{
				pRazer->flag = true;
				pRazer->death[i] = false;
				timeCount += 0.5f;
				D3DXVec3Normalize(&pRazer->Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 40));
				pRazer->Pos[i].y -= 0.1f + (timeCount * sin(pRazer->Rot[i].x) - 2.5f) - pRazer->Accel[i].y * 0.5f;
				pRazer->Pos[i].x += sin(pRazer->Rot[i].y) * 2.0f * cos(pRazer->Rot[i].x) + pRazer->Accel[i].x * 1.5f;
				pRazer->Pos[i].z += cos(pRazer->Rot[i].y) * 2.0f * cos(pRazer->Rot[i].x) + pRazer->Accel[i].z * 0.5f;
				razer->Draw(&pRazer->Pos[i]);
				Hit();
			}
			if (pRazer->Pos[i].y < 0)
			{
				pRazer->Exist[i] = false;
				pRazer->death[i] = true;

				pRazer->Count[i] = 0;
				timeCount = 0;
			}

			if (pRazer->Count[i] > 500)
			{
				pRazer->flag = false;
				pRazer->death[i] = true;
				pRazer->Exist[i] = false;
				pRazer->Count[i] = 0;
				timeCount = 0;
			}
			if (pRazer->death[i] == true)
			{
				pRazer->Pos[i].y = 1000.0f;
				pRazer->oldPos[i] = pRazer->Pos[i];
			}
		}
		razerState[i] = pRazer->Pos[i];
	}
}

//	�����蔻��
void Player::Hit()
{
	//	�G�̍��W
	extern D3DXVECTOR3 enemy_Collider[ENEMY_MAX];

	//----------------------------------------------------------------
	//	�e�ۂ̓����蔻��
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < BULLET_MAX; j++)
		{
			enemy_Radius[i] = 10.0f;
			pBullet->Radius[j] = 0.5f;

			if ((enemy_Collider[i].x - pBullet->Pos[j].x) * (enemy_Collider[i].x - pBullet->Pos[j].x) +
				(enemy_Collider[i].y - pBullet->Pos[j].y) * (enemy_Collider[i].y - pBullet->Pos[j].y) +
				(enemy_Collider[i].z - pBullet->Pos[j].z) * (enemy_Collider[i].z - pBullet->Pos[j].z) <=
				(pBullet->Radius[j] + enemy_Radius[i]) * (pBullet->Radius[j] + enemy_Radius[i]))
			{
				pBullet->death[j] = true;
			}

		}
	}

	//----------------------------------------------------------------
	//	���e�̓����蔻��
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < BOMB_MAX; j++)
		{
			enemy_Radius[i] = 10.0f;
			pBomb->Explosion_Radius[j] = 0.5f;

			if ((enemy_Collider[i].x - pBomb->Explosion_Pos[j].x) * (enemy_Collider[i].x - pBomb->Explosion_Pos[j].x) +
				(enemy_Collider[i].y - pBomb->Explosion_Pos[j].y) * (enemy_Collider[i].y - pBomb->Explosion_Pos[j].y) +
				(enemy_Collider[i].z - pBomb->Explosion_Pos[j].z) * (enemy_Collider[i].z - pBomb->Explosion_Pos[j].z) <=
				(pBomb->Explosion_Radius[j] + enemy_Radius[i]) * (pBomb->Explosion_Radius[j] + enemy_Radius[i]))
			{
				pBomb->Explosion_Death[j] = true;
			}

		}
	}

	//----------------------------------------------------------------
	//	���[�U�[�̓����蔻��
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < RAZER_MAX; j++)
		{
			enemy_Radius[i] = 10.0f;
			pRazer->Radius[j] = 0.5f;

			if ((enemy_Collider[i].x - pRazer->Pos[j].x) * (enemy_Collider[i].x - pRazer->Pos[j].x) +
				(enemy_Collider[i].y - pRazer->Pos[j].y) * (enemy_Collider[i].y - pRazer->Pos[j].y) +
				(enemy_Collider[i].z - pRazer->Pos[j].z) * (enemy_Collider[i].z - pRazer->Pos[j].z) <=
				(pRazer->Radius[j] + enemy_Radius[i]) * (pRazer->Radius[j] + enemy_Radius[i]))
			{
				pRazer->death[j] = true;
			}

		}
	}

}

//	����
void Player::CreateExp(D3DXVECTOR3 Pos[], int Num)
{
	
}



