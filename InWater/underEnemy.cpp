/*
*	下にいる敵
*
*	2016/06/10		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "underEnemy.h"

extern LPDIRECT3DDEVICE9 d3dDevice;
D3DXVECTOR3 Uenemy_Collider[ENEMY_MAX];
D3DXVECTOR3 UenemyBulletPos[ENEMY_MAX];

//コンストラクタ
//	コンストラクタ
underEnemy::underEnemy()
{
	enemy = new Graphic[ENEMY_MAX];
	model = new Model("Model/tako.x");
	texture = new Texture("Texture/red_skin.png");
	State = new EnemyState;
	bullet = new Bullet("Texture/g_ball.bmp", 1);
	Attack = new EnemyAttack[ENEMY_MAX];

	DebugLog("敵を生成しました。\n");

	InitUEnemy();
	InitBullet();
}

//	デストラクタ
underEnemy::~underEnemy()
{
	Release();
}

//	敵の初期化
void underEnemy::InitUEnemy()
{
	State->MinRange = D3DXVECTOR3(750, -35, 750);
	State->MaxRange = D3DXVECTOR3(1250, -25, 1250);
	D3DXVECTOR3 range = State->MaxRange - State->MinRange;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		State->Position[i].x = (float)((double)rand() / RAND_MAX * range.x) + State->MinRange.x;
		State->Position[i].y = (float)((double)rand() / RAND_MAX * range.y) + State->MinRange.y;
		State->Position[i].z = (float)((double)rand() / RAND_MAX * range.z) + State->MinRange.z;
		State->Rotation[i] = D3DXVECTOR3(0, 0, 0);
		State->Scale[i] = D3DXVECTOR3(4, 4, 4);
		State->Vitality[i] = 150;
		State->enemyDeathCount = 0;
		State->enemyDeathFlag[i] = false;
		State->enemyHitFlag[i] = false;
		State->enemyBulletFlag = false;
		State->Speed[i] = 1;
		State->x_Speed[i] = 1;
		State->z_Speed[i] = 1;
		State->y_Speed[i] = 1;
	}
	DebugLog("敵を初期化しました。\n");
}

//	敵の弾の初期化
void underEnemy::InitBullet()
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

//	解放処理
void underEnemy::Release()
{
	delete[] enemy;
	delete State;
	delete model;
	delete texture;
	delete bullet;
	delete[] Attack;

	DebugLog("敵を破棄しました。\n");
}

//	敵の描画
void underEnemy::Draw()
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
		InitUEnemy();
		for (int i = 0; i < ENEMY_MAX; i++)
		{
			enemy->DrawModelTexture(State->Position[i], State->Rotation[i], State->Scale[i], *model, *texture, true);
		}
	}

}

//	敵の動作
void underEnemy::Move()
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
		if (State->Scale[i].x > 5)
		{
			State->x_Speed[i] = -1;
		}
		if (State->Scale[i].x < 3)
		{
			State->x_Speed[i] = 1;
		}
		if (State->Scale[i].z > 5)
		{
			State->z_Speed[i] = -1;
		}
		if (State->Scale[i].z < 3)
		{
			State->z_Speed[i] = 1;
		}
		if (State->Scale[i].y > 5)
		{
			State->y_Speed[i] = -1;
		}
		if (State->Scale[i].y < 4)
		{
			State->y_Speed[i] = 1;
		}


		State->Position[i].y += 0.04 * (State->Speed[i] + State->Accel[i].y);
		if (State->Position[i].y > -27)
		{
			State->Speed[i] = -1;
		}
		if (State->Position[i].y < -32)
		{
			State->Speed[i] = 1;
		}
		State->oldEnemyPos[i] = State->Position[i];
		Uenemy_Collider[i] = State->oldEnemyPos[i];
	}

}

//	敵の攻撃
void underEnemy::Shot()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{

		Attack[i].Frame++;
		if (Attack[i].Frame % 400 == 0 && Attack[i].flag == false)
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
				Attack[i].Pos.x += 0.3f * i * Attack[i].Accel.x;
				Attack[i].Pos.z += 0.3f * i * Attack[i].Accel.z;
				Attack[i].Pos.y += 1.5f * (1 + i) * Attack[i].Accel.y;

				if (Attack[i].death == false)
				{
					bullet->Draw(&Attack[i].Pos, 5.0f, 1, false);
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
		UenemyBulletPos[i] = Attack[i].Pos;
	}
}

//	当たり判定
void underEnemy::Hit()
{
	//	プレイヤーの弾の座標
	extern D3DXVECTOR3 bombState[BOMB_MAX];

	//----------------------------------------------------------------
	//	爆弾との当たり判定
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < BOMB_MAX; j++)
		{
			State->Collider[i] = State->Position[i];
			State->Radius[i] = 15.0f;
			bomb_Radius[j] = 0.5f;

			if ((State->Collider[i].x - bombState[j].x) * (State->Collider[i].x - bombState[j].x) +
				(State->Collider[i].y - bombState[j].y) * (State->Collider[i].y - bombState[j].y) +
				(State->Collider[i].z - bombState[j].z) * (State->Collider[i].z - bombState[j].z) <=
				(bomb_Radius[j] + State->Radius[i]) * (bomb_Radius[j] + State->Radius[i]))
			{
				State->Vitality[i] -= 3;
				State->enemyHitFlag[i] = true;
				DestroyEnemy();
				if (State->Vitality[i] == 0)
				{
					State->enemyDeathCount += 1;
					DebugLog("たこ：いたい～っ\n");
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

void underEnemy::DestroyEnemy()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (State->enemyHitFlag[i] == true)
		{
			D3DXVec3Normalize(&State->Accel[i], &D3DXVECTOR3(rand() % 10, rand() % 10, rand() % 10));
			State->Scale[i].x += State->Accel[i].x * 0.8f * State->x_Speed[i];
			State->Scale[i].y += State->Accel[i].y * 0.8f * State->y_Speed[i];
			State->Scale[i].z += State->Accel[i].z * 0.8f * State->z_Speed[i];

			if (State->Scale[i].x > 6)
			{
				State->x_Speed[i] = -1;
			}
			if (State->Scale[i].y > 6)
			{
				State->y_Speed[i] = -1;
			}
			if (State->Scale[i].z > 6)
			{
				State->z_Speed[i] = -1;
			}

			if (State->Scale[i].x < 2)
			{
				State->x_Speed[i] = 1;
			}
			if (State->Scale[i].y < 2)
			{
				State->y_Speed[i] = 1;
			}
			if (State->Scale[i].z < 2)
			{
				State->z_Speed[i] = 1;
			}

		}
		State->enemyHitFlag[i] = false;
	}
}


