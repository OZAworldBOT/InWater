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

//	�R���X�g���N�^
Enemy::Enemy()
{
	enemy = new Graphic[ENEMY_MAX];
	State = new EnemyState;
	model = new Model("Model/tako.x");
	texture = new Texture("Texture/gold_block.png");
	bullet = new Bullet();
	explosion = new Explosion();
	eExp = new Eexplosion;

	DebugLog("�G�𐶐����܂����B\n");

	InitEnemy();
	InitBullet();
	InitExplosion();
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
		State->Scale[i] = D3DXVECTOR3(10, 10, 10);
		State->Vitality[i] = 100;
		State->enemyDeathFlag[i] = false;
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
}

//	�����̏�����
void Enemy::InitExplosion()
{
}

//	�������
void Enemy::Release()
{
	delete[] enemy;
	delete State;
	delete model;
	delete texture;
	delete bullet;
	delete explosion;
	delete eExp;

	DebugLog("�G��j�����܂����B\n");
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
		if (State->Scale[i].x > 11)
		{
			State->x_Speed[i] = -1;
		}
		if (State->Scale[i].x < 9)
		{
			State->x_Speed[i] = 1;
		}
		if (State->Scale[i].z > 11)
		{
			State->z_Speed[i] = -1;
		}
		if (State->Scale[i].z < 9)
		{
			State->z_Speed[i] = 1;
		}
		if (State->Scale[i].y > 12)
		{
			State->y_Speed[i] = -1;
		}
		if (State->Scale[i].y < 10)
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
	}
}

//	�G�̍U��
void Enemy::Shot()
{

}

//	�����蔻��
void Enemy::Hit()
{
	//	�v���C���[�̒e�̍��W
	extern D3DXVECTOR3 bulletState[BULLET_MAX];
	extern D3DXVECTOR3 bombState[BOMB_MAX];
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
			}
		}
		if (State->Vitality[i] < 0)
		{
			State->Scale[i].y -= 0.3f;
			if (State->Scale[i].y < 0)
			{
				State->enemyDeathFlag[i] = true;
			}
		}
	}

	//----------------------------------------------------------------
	//	���e�Ƃ̓����蔻��
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < BOMB_MAX; j++)
		{
			State->Collider[i] = State->Position[i];
			State->Radius[i] = 10.0f;
			bomb_Radius[j] = 0.5f;

			if ((State->Collider[i].x - bombState[j].x) * (State->Collider[i].x - bombState[j].x) +
				(State->Collider[i].y - bombState[j].y) * (State->Collider[i].y - bombState[j].y) +
				(State->Collider[i].z - bombState[j].z) * (State->Collider[i].z - bombState[j].z) <=
				(bomb_Radius[j] + State->Radius[i]) * (bomb_Radius[j] + State->Radius[i]))
			{
				State->Vitality[i] -= 2;
			}
		}
		if (State->Vitality[i] < 0)
		{
			State->Scale[i].y -= 0.3f;
			if (State->Scale[i].y < 0)
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
			State->Radius[i] = 10.0f;
			razer_Radius[j] = 0.5f;

			if ((State->Collider[i].x - razerState[j].x) * (State->Collider[i].x - razerState[j].x) +
				(State->Collider[i].y - razerState[j].y) * (State->Collider[i].y - razerState[j].y) +
				(State->Collider[i].z - razerState[j].z) * (State->Collider[i].z - razerState[j].z) <=
				(razer_Radius[j] + State->Radius[i]) * (razer_Radius[j] + State->Radius[i]))
			{
				State->Vitality[i] -= 1;
			}
		}
		if (State->Vitality[i] < 0)
		{
			State->Scale[i].y -= 0.3f;
			if (State->Scale[i].y < 0)
			{
				State->enemyDeathFlag[i] = true;
			}
		}
	}
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
}

void Enemy::DestroyEnemy()
{
}