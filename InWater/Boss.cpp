/*
*	ボス
*
*	2016/06/10		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Boss.h"

extern LPDIRECT3DDEVICE9 d3dDevice;
D3DXVECTOR3 Boss_Collider;

//コンストラクタ
Boss::Boss()
{
	boss = new Graphic();
	model = new Model("Model/whale.x");
	texture = new Texture("Texture/blue_skin.png");
	State = new BossState;

	DebugLog("敵を生成しました。\n");

	InitBoss();
}

//	デストラクタ
Boss::~Boss()
{
	Release();
}

//	敵の初期化
void Boss::InitBoss()
{
	State->MinRange = D3DXVECTOR3(750, -90, 750);
	State->MaxRange = D3DXVECTOR3(1250, -50, 1250);
	D3DXVECTOR3 range = State->MaxRange - State->MinRange;

	srand((unsigned int)time(NULL));

	State->Position.x = (float)((double)rand() / RAND_MAX * range.x) + State->MinRange.x;
	State->Position.y = (float)((double)rand() / RAND_MAX * range.y) + State->MinRange.y;
	State->Position.z = (float)((double)rand() / RAND_MAX * range.z) + State->MinRange.z;
	State->Rotation = D3DXVECTOR3(0, 0, 0);
	State->Scale = D3DXVECTOR3(1000, 1000, 1000);
	State->Vitality = 1000;
	State->DeathCount = 0;
	State->CountFlag = false;
	State->DeathFlag = false;
	State->HitFlag = false;
	State->BulletFlag = false;
	State->Speed = 1;
	State->x_Speed = 1;
	State->z_Speed = 1;
	State->y_Speed = 1;

	DebugLog("敵を初期化しました。\n");
}

//	解放処理
void Boss::Release()
{
	delete boss;
	delete State;
	delete model;
	delete texture;

	DebugLog("敵を破棄しました。\n");
}

//	敵の描画
void Boss::Draw()
{
	int c = 0;
	int random = 0;
	c++;
	if (c % 100 == 0)
	{
		if (State->CountFlag == false)
		{
			random = rand() % 100;
			if (random == 59)
			{
				State->CountFlag = true;
			}
		}
		if (State->DeathFlag == false && State->CountFlag == true)
		{
			boss->DrawModelTexture(State->Position, State->Rotation, State->Scale, *model, *texture, true);
		}
	}
	if (State->DeathFlag == true)
	{
		InitBoss();
		State->CountFlag = false;
	}
}

//	敵の動作
void Boss::Move()
{
	srand((unsigned int)time(NULL));
	D3DXVec3Normalize(&State->Accel, &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
	State->Position.x += State->Accel.x * 0.06f;
	State->Position.z += State->Accel.z * 0.06f;
	State->Scale.x += (0.02 + State->Accel.x * 0.8) * State->x_Speed;
	State->Scale.z += (0.02 + State->Accel.x * 0.8) * State->z_Speed;
	State->Scale.y += (0.02 + State->Accel.y * 0.8) * State->y_Speed;
	State->Rotation.y += 0.003 + State->Accel.y * 0.03;
	if (State->Scale.x > 1005)
	{
		State->x_Speed = -1;
	}
	if (State->Scale.x < 995)
	{
		State->x_Speed = 1;
	}
	if (State->Scale.z > 1005)
	{
		State->z_Speed = -1;
	}
	if (State->Scale.z < 995)
	{
		State->z_Speed = 1;
	}
	if (State->Scale.y > 1005)
	{
		State->y_Speed = -1;
	}
	if (State->Scale.y < 995)
	{
		State->y_Speed = 1;
	}


	State->Position.y += 0.04 * (State->Speed + State->Accel.y);
	if (State->Position.y > -47)
	{
		State->Speed = -1;
	}
	if (State->Position.y < -92)
	{
		State->Speed = 1;
	}

	if (State->CountFlag == true)
	{
		State->oldEnemyPos = State->Position;
		Boss_Collider = State->oldEnemyPos;
	}
}
//	当たり判定
void Boss::Hit()
{
	//	プレイヤーの弾の座標
	extern D3DXVECTOR3 bulletState[BULLET_MAX];
	extern D3DXVECTOR3 razerState[RAZER_MAX];

	//----------------------------------------------------------------
	//	弾丸との当たり判定
	//----------------------------------------------------------------
	for (int j = 0; j < BULLET_MAX; j++)
	{
		State->Collider = State->Position;
		State->Radius = 110.0f;
		bullet_Radius[j] = 0.5f;

		if ((State->Collider.x - bulletState[j].x) * (State->Collider.x - bulletState[j].x) +
			(State->Collider.y - bulletState[j].y) * (State->Collider.y - bulletState[j].y) +
			(State->Collider.z - bulletState[j].z) * (State->Collider.z - bulletState[j].z) <=
			(bullet_Radius[j] + State->Radius) * (bullet_Radius[j] + State->Radius))
		{
			State->Vitality -= 0.01;
			DestroyEnemy();
			if (State->Vitality == 0)
			{
				State->DeathCount += 1;
			}
		}
	}
	if (State->Vitality <= 0)
	{
		State->Vitality = -2;
		State->Scale.x -= 6.0f;
		State->Scale.z -= 6.0f;
		State->oldEnemyPos = State->Position;
		State->Position.y += 0.3f;
		if (State->Scale.x < 0)
		{
			State->DeathFlag = true;
		}
	}

	//----------------------------------------------------------------
	//	レーザーとの当たり判定
	//----------------------------------------------------------------
	for (int j = 0; j < RAZER_MAX; j++)
	{
		State->Collider = State->Position;
		State->Radius = 110.0f;
		razer_Radius[j] = 0.5f;

		if ((State->Collider.x - razerState[j].x) * (State->Collider.x - razerState[j].x) +
			(State->Collider.y - razerState[j].y) * (State->Collider.y - razerState[j].y) +
			(State->Collider.z - razerState[j].z) * (State->Collider.z - razerState[j].z) <=
			(razer_Radius[j] + State->Radius) * (razer_Radius[j] + State->Radius))
		{
			State->Vitality -= 0.04;
			State->HitFlag = true;
			DestroyEnemy();
			if (State->Vitality == 0)
			{
				State->DeathCount += 1;
			}
		}
	}
	if (State->Vitality <= 0)
	{
		State->Vitality = -2;
		State->Scale.x -= 6.0f;
		State->Scale.z -= 6.0f;
		State->oldEnemyPos = State->Position;
		State->Position.y += 0.3f;
		if (State->Scale.x < 0)
		{
			State->DeathFlag = true;
		}
	}
}


void Boss::DestroyEnemy()
{
	if (State->HitFlag == true)
	{
		D3DXVec3Normalize(&State->Accel, &D3DXVECTOR3(rand() % 10, rand() % 10, rand() % 10));
		State->Scale.x += State->Accel.x * 4.0f * State->x_Speed;
		State->Scale.y += State->Accel.y * 4.0f * State->y_Speed;
		State->Scale.z += State->Accel.z * 4.0f * State->z_Speed;

		if (State->Scale.x > 1020)
		{
			State->x_Speed = -1;
		}
		if (State->Scale.y > 1020)
		{
			State->y_Speed = -1;
		}
		if (State->Scale.z > 1020)
		{
			State->z_Speed = -1;
		}

		if (State->Scale.x < 980)
		{
			State->x_Speed = 1;
		}
		if (State->Scale.y < 980)
		{
			State->y_Speed = 1;
		}
		if (State->Scale.z < 980)
		{
			State->z_Speed = 1;
		}

	}
	State->HitFlag = false;
}



