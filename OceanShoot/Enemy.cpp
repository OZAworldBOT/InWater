/*
*	敵のクラス
*
*	2015/05/28		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Enemy.h"


extern LPDIRECT3DDEVICE9 d3dDevice;
D3DXVECTOR3 enemy_Collider[ENEMY_MAX];

//	コンストラクタ
Enemy::Enemy()
{
	enemy = new Graphic[ENEMY_MAX];
	model = new Model("Model/tako.x");
	texture = new Texture("Texture/gold_block.png");
	bullet = new Bullet();

	DebugLog("敵を生成しました。\n");

	InitEnemy();
	InitBullet();
}

//	デストラクタ
Enemy::~Enemy()
{
	Release();
}

//	敵の初期化
void Enemy::InitEnemy()
{
	MinRange = D3DXVECTOR3(750, 0, 750);
	MaxRange = D3DXVECTOR3(1250, 10, 1250);
	D3DXVECTOR3 range = MaxRange - MinRange;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		Position[i].x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
		Position[i].y = 10.0f;
		Position[i].z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
		Rotation[i] = D3DXVECTOR3(0, 0, 0);
		Scale[i] = D3DXVECTOR3(10, 10, 10);
		Vitality[i] = 100;
		enemyDeathFlag[i] = false;
	}
	DebugLog("敵を初期化しました。\n");
}

//	敵の弾の初期化
void Enemy::InitBullet()
{
}

//	解放処理
void Enemy::Release()
{
	delete[] enemy;
	delete model;
	delete texture;
	delete bullet;

	DebugLog("敵を破棄しました。\n");
}

//	敵の動作
void Enemy::Move()
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		D3DXVec3Normalize(&Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
		Position[i].x += Accel[i].x * 0.08f;
		Position[i].z += Accel[i].z * 0.08f;
		Scale[i] = D3DXVECTOR3(10, 10, 10);
	}
}

//	敵の攻撃
void Enemy::Shot()
{

}

//	当たり判定
void Enemy::Hit()
{
	//	プレイヤーの弾の座標
	extern D3DXVECTOR3 bulletState[BULLET_MAX];
	extern D3DXVECTOR3 bombState[BOMB_MAX];
	extern D3DXVECTOR3 razerState[RAZER_MAX];

	//----------------------------------------------------------------
	//	弾丸との当たり判定
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < BULLET_MAX; j++)
		{
			Collider[i] = Position[i];
			Radius[i] = 10.0f;
			bullet_Radius[j] = 0.5f;

			if ((Collider[i].x - bulletState[j].x) * (Collider[i].x - bulletState[j].x) +
				(Collider[i].y - bulletState[j].y) * (Collider[i].y - bulletState[j].y) +
				(Collider[i].z - bulletState[j].z) * (Collider[i].z - bulletState[j].z) <=
				(bullet_Radius[j] + Radius[i]) * (bullet_Radius[j] + Radius[i]))
			{
				Vitality[i] -= 1;
			}
		}
		if (Vitality[i] < 0)
		{
			enemyDeathFlag[i] = true;
		}
	}

	//----------------------------------------------------------------
	//	爆弾との当たり判定
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < BOMB_MAX; j++)
		{
			Collider[i] = Position[i];
			Radius[i] = 10.0f;
			bomb_Radius[j] = 0.5f;

			if ((Collider[i].x - bombState[j].x) * (Collider[i].x - bombState[j].x) +
				(Collider[i].y - bombState[j].y) * (Collider[i].y - bombState[j].y) +
				(Collider[i].z - bombState[j].z) * (Collider[i].z - bombState[j].z) <=
				(bomb_Radius[j] + Radius[i]) * (bomb_Radius[j] + Radius[i]))
			{
				Vitality[i] -= 1;
			}
		}
		if (Vitality[i] < 0)
		{
			enemyDeathFlag[i] = true;
		}
	}

	//----------------------------------------------------------------
	//	レーザーとの当たり判定
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < RAZER_MAX; j++)
		{
			Collider[i] = Position[i];
			Radius[i] = 10.0f;
			razer_Radius[j] = 0.5f;

			if ((Collider[i].x - razerState[j].x) * (Collider[i].x - razerState[j].x) +
				(Collider[i].y - razerState[j].y) * (Collider[i].y - razerState[j].y) +
				(Collider[i].z - razerState[j].z) * (Collider[i].z - razerState[j].z) <=
				(razer_Radius[j] + Radius[i]) * (razer_Radius[j] + Radius[i]))
			{
				Vitality[i] -= 1;
			}
		}
		if (Vitality[i] < 0)
		{
			enemyDeathFlag[i] = true;
		}
	}

}

//	敵の描画
void Enemy::Draw()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemyDeathFlag[i] == false)
		{
			enemy[i].DrawModelTexture(Position[i], Rotation[i], Scale[i], *model, *texture);
		}
	}
}