/*
*	�G�̃N���X
*
*	2015/05/28		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Enemy.h"


extern LPDIRECT3DDEVICE9 d3dDevice;
D3DXVECTOR3 enemy_Collider[ENEMY_MAX];
D3DXVECTOR3 enemyBulletPos[ENEMY_MAX];

//	�R���X�g���N�^
Enemy::Enemy()
{
	enemy = new Graphic[ENEMY_MAX];
	model = new Model("Model/jellyfish.x");
	texture = new Texture("Texture/font_skin.png");
	State = new EnemyState;
	bullet = new Bullet("Texture/r_ball.bmp", 1);
	Attack = new EnemyAttack[ENEMY_MAX];

	DebugLog("�G�𐶐����܂����B\n");

	InitEnemy();
	InitBullet();
}

//	�f�X�g���N�^
Enemy::~Enemy()
{
	Release();
}

//	�G�̏�����
void Enemy::InitEnemy()
{
	State->MinRange = D3DXVECTOR3(750, 0, 750);
	State->MaxRange = D3DXVECTOR3(1250, 10, 1250);
	D3DXVECTOR3 range = State->MaxRange - State->MinRange;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		State->Position[i].x = (float)((double)rand() / RAND_MAX * range.x) + State->MinRange.x;
		State->Position[i].y = 10.0f;
		State->Position[i].z = (float)((double)rand() / RAND_MAX * range.z) + State->MinRange.z;
		State->Rotation[i] = D3DXVECTOR3(0, 0, 0);
		State->Scale[i] = D3DXVECTOR3(6, 6, 6);
		State->Vitality[i] = 200;
		State->enemyDeathCount = 0;
		State->enemyDeathFlag[i] = false;
		State->enemyHitFlag[i] = false;
		State->enemyBulletFlag = false;
		State->Speed[i] = 1;
		State->x_Speed[i] = 1;
		State->z_Speed[i] = 1;
		State->y_Speed[i] = 1;
	}
	DebugLog("�G�����������܂����B\n");
}

//	�G�̒e�̏�����
void Enemy::InitBullet()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		Attack[i].Frame = 0;
		Attack[i].Count = 0;
		Attack[i].Exist = false;
		Attack[i].flag = false;
		Attack[i].death = true;
	}
}

//	�������
void Enemy::Release()
{
	delete[] enemy;
	delete State;
	delete model;
	delete texture;
	delete bullet;
	delete[] Attack;

	DebugLog("�G��j�����܂����B\n");
}

//	�G�̕`��
void Enemy::Draw()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (State->enemyDeathFlag[i] == false)
		{
			enemy->DrawModelTexture(State->Position[i], State->Rotation[i], State->Scale[i], *model, *texture, true);
		}
	}
	if (State->enemyDeathCount >= ENEMY_MAX - 1)
	{
		InitEnemy();
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			enemy->DrawModelTexture(State->Position[i], State->Rotation[i], State->Scale[i], *model, *texture, true);
		}
	}

}

//	�G�̓���
void Enemy::Move()
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		D3DXVec3Normalize(&State->Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
		State->Position[i].x += State->Accel[i].x * 0.06f;
		State->Position[i].z += State->Accel[i].z * 0.06f;
		State->Scale[i].x += (0.02 + State->Accel[i].x * 0.02) * State->x_Speed[i];
		State->Scale[i].z += (0.02 + State->Accel[i].x * 0.02) * State->z_Speed[i];
		State->Scale[i].y += (0.02 + State->Accel[i].y * 0.02) * State->y_Speed[i];
		if (State->Scale[i].x > 7)
		{
			State->x_Speed[i] = -1;
		}
		if (State->Scale[i].x < 5)
		{
			State->x_Speed[i] = 1;
		}
		if (State->Scale[i].z > 7)
		{
			State->z_Speed[i] = -1;
		}
		if (State->Scale[i].z < 5)
		{
			State->z_Speed[i] = 1;
		}
		if (State->Scale[i].y > 7)
		{
			State->y_Speed[i] = -1;
		}
		if (State->Scale[i].y < 6)
		{
			State->y_Speed[i] = 1;
		}


		State->Position[i].y += 0.04 * (State->Speed[i] + State->Accel[i].y);
		if (State->Position[i].y > 13)
		{
			State->Speed[i] = -1;
		}
		if (State->Position[i].y < 8)
		{
			State->Speed[i] = 1;
		}
		State->oldEnemyPos[i] = State->Position[i];
		enemy_Collider[i] = State->oldEnemyPos[i];
	}

}

//	�G�̍U��
void Enemy::Shot()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{

		Attack[i].Frame++;
		if (Attack[i].Frame % 600 == 0 && Attack[i].flag == false)
		{
			srand((unsigned int)time(NULL));
			Attack[i].Pos = State->oldEnemyPos[i];
			Attack[i].Exist = true;
		}
	}
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (Attack[i].Exist == true && State->enemyHitFlag[i] == false && State->enemyDeathFlag[i] == false)
		{
			Attack[i].Count++;
			if (Attack[i].Count < 50 && State->enemyDeathFlag[i] == false)
			{
				Attack[i].flag = true;
				Attack[i].death = false;
				Attack[i].Accel = D3DXVECTOR3(rand() % 3 - 1, rand() % 3 - 1, rand() % 3 - 1);
				Attack[i].Pos.x += 1.5f * (1 + i) * Attack[i].Accel.x;
				Attack[i].Pos.z += 1.5f * (1 + i) * Attack[i].Accel.z;
				Attack[i].Pos.y += 0.5f * (1 + i) * Attack[i].Accel.y;

				if (Attack[i].death == false)
				{
					bullet->Draw(&Attack[i].Pos, 10.0f, 1, true);
				}
			}
			if (Attack[i].Count > 50)
			{
				Attack[i].flag = false;
				Attack[i].death = true;
				Attack[i].Exist = false;
				Attack[i].Count = 0;
			}
		}
		enemyBulletPos[i] = Attack[i].Pos;
	}
}

//	�����蔻��
void Enemy::Hit()
{
	//	�v���C���[�̒e�̍��W
	extern D3DXVECTOR3 bulletState[BULLET_MAX];
	extern D3DXVECTOR3 razerState[RAZER_MAX];

	//----------------------------------------------------------------
	//	�e�ۂƂ̓����蔻��
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < BULLET_MAX; j++)
		{
			State->Collider[i] = State->Position[i];
			State->Radius[i] = 15.0f;
			bullet_Radius[j] = 0.5f;

			if ((State->Collider[i].x - bulletState[j].x) * (State->Collider[i].x - bulletState[j].x) +
				(State->Collider[i].y - bulletState[j].y) * (State->Collider[i].y - bulletState[j].y) +
				(State->Collider[i].z - bulletState[j].z) * (State->Collider[i].z - bulletState[j].z) <=
				(bullet_Radius[j] + State->Radius[i]) * (bullet_Radius[j] + State->Radius[i]))
			{
				State->Vitality[i] -= 1;
				State->enemyHitFlag[i] = true;
				DestroyEnemy();
				if (State->Vitality[i] == 0)
				{
					State->enemyDeathCount += 1;
				}
			}
		}
		if (State->Vitality[i] <= 0)
		{
			State->Vitality[i] = -2;
			State->Scale[i].x -= 0.1f;
			State->Scale[i].z -= 0.1f;
			State->oldEnemyPos[i] = State->Position[i];
			State->Position[i].y += 0.3f;
			if (State->Scale[i].x < 0)
			{
				State->enemyDeathFlag[i] = true;
			}
		}
	}

	//----------------------------------------------------------------
	//	���[�U�[�Ƃ̓����蔻��
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < RAZER_MAX; j++)
		{
			State->Collider[i] = State->Position[i];
			State->Radius[i] = 15.0f;
			razer_Radius[j] = 0.5f;

			if ((State->Collider[i].x - razerState[j].x) * (State->Collider[i].x - razerState[j].x) +
				(State->Collider[i].y - razerState[j].y) * (State->Collider[i].y - razerState[j].y) +
				(State->Collider[i].z - razerState[j].z) * (State->Collider[i].z - razerState[j].z) <=
				(razer_Radius[j] + State->Radius[i]) * (razer_Radius[j] + State->Radius[i]))
			{
				State->Vitality[i] -= 4;
				State->enemyHitFlag[i] = true;
				DestroyEnemy();
				if (State->Vitality[i] == 0)
				{
					State->enemyDeathCount += 1;
				}
			}
		}
		if (State->Vitality[i] <= 0)
		{
			State->Vitality[i] = -2;
			State->Scale[i].x -= 0.1f;
			State->Scale[i].z -= 0.1f;
			State->oldEnemyPos[i] = State->Position[i];
			State->Position[i].y += 0.3f;
			if (State->Scale[i].x < 0)
			{
				State->enemyDeathFlag[i] = true;
			}
		}
	}

}

void Enemy::DestroyEnemy()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (State->enemyHitFlag[i] == true)
		{
			D3DXVec3Normalize(&State->Accel[i], &D3DXVECTOR3(rand() % 10, rand() % 10, rand() % 10));
			State->Scale[i].x += State->Accel[i].x * 0.8f * State->x_Speed[i];
			State->Scale[i].y += State->Accel[i].y * 0.8f * State->y_Speed[i];
			State->Scale[i].z += State->Accel[i].z * 0.8f * State->z_Speed[i];

			if (State->Scale[i].x > 8)
			{
				State->x_Speed[i] = -1;
			}
			if (State->Scale[i].y > 8)
			{
				State->y_Speed[i] = -1;
			}
			if (State->Scale[i].z > 8)
			{
				State->z_Speed[i] = -1;
			}

			if (State->Scale[i].x < 4)
			{
				State->x_Speed[i] = 1;
			}
			if (State->Scale[i].y < 4)
			{
				State->y_Speed[i] = 1;
			}
			if (State->Scale[i].z < 4)
			{
				State->z_Speed[i] = 1;
			}

		}
		State->enemyHitFlag[i] = false;
	}
}


