/*
*	プレイヤーのクラス
*
*	2015/05/27		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Player.h"


extern LPDIRECT3DDEVICE9 d3dDevice;
extern LPDIRECTINPUTDEVICE8 dinputDevice;
D3DXVECTOR3 inputState;					//	プレイヤーの座標を取得
D3DXVECTOR3 bulletState[BULLET_MAX];	//	プレイヤーの弾の座標を取得
D3DXVECTOR3 bombState[BOMB_MAX];		//	プレイヤーの爆弾の座標を取得
D3DXVECTOR3 razerState[RAZER_MAX];		//	プレイヤーのレーザーの座標を取得
bool		playerDeath;				//	自機の消滅


//	コンストラクタ
Player::Player()
{
	camera = new Camera();
	player = new Graphic();
	HpBar = new Graphic();

	model = new Model[2];
	model[0].LoadMesh("Model/dolphin.x");
	model[1].LoadMesh("Model/HpBar.x");
	texture = new Texture[2];
	texture[0].LoadTexture("Texture/blue_skin.png");
	texture[1].LoadTexture("Texture/hp_skin.png");

	bullet = new Bullet("Texture/ball.bmp", BULLET_MAX);
	bomb = new Bomb();
	razer = new Razer();
	explosion = new Explosion();

	pBullet = new Pbullet;
	pBomb = new Pbomb;
	pRazer = new Prazer;
	hp = new HitPoint;

	DebugLog("プレイヤーを生成しました。\n");

	InitPlayer();
}

//	デストラクタ
Player::~Player()
{
	Release();
}

//------------------------------------------------------------------------------------------------------------
//	初期化
//------------------------------------------------------------------------------------------------------------
void Player::InitPlayer()
{
	Position = D3DXVECTOR3(1000, 8, 1000);
	Rotation = D3DXVECTOR3(0, D3DXToRadian(90), 0);
	Scale = D3DXVECTOR3(25, 25, 25);
	playerHitFlag = false;
	playerDeath = false;
	Count = 0;
	x_Speed = 1;
	y_Speed = 1;
	z_Speed = 1;
	Vitality = 400;

	camera_Pos = D3DXVECTOR3(0, 15, -10);
	camera_Rot = D3DXVECTOR3(D3DXToRadian(-10), 0, 0);

	InitBullet();
	InitBomb();
	InitRazer();
	InitHpBar();
	DebugLog("プレイヤーを初期化しました。\n");
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
void Player::InitHpBar()
{
	hp->Pos = D3DXVECTOR3(997, 17, 1000);
	hp->Rot = D3DXVECTOR3(0, -0.1, 0);
	hp->Scale = D3DXVECTOR3(40, 40, 40);
}
void Player::ResetPlayer()
{
	playerHitFlag = false;
	playerDeath = false;
	Count = 0;
	x_Speed = 1;
	y_Speed = 1;
	z_Speed = 1;
	Vitality = 400;
	hp->Scale = D3DXVECTOR3(40, 40, 40);
}

//	解放
void Player::Release()
{
	delete camera;
	delete player;
	delete[] model;
	delete[] texture;
	delete bullet;
	delete bomb;
	delete razer;
	delete explosion;
	delete pBullet;
	delete pBomb;
	delete pRazer;
	delete HpBar;
	delete hp;

	DebugLog("プレイヤーを破棄しました。\n");
}

//------------------------------------------------------------------------------------------------------------
//	描画
//------------------------------------------------------------------------------------------------------------
void Player::Draw()
{
	player->DrawModelTexture(Position, Rotation, Scale, model[0], texture[0], true);
	HpBar->DrawModelTexture(hp->Pos, hp->Rot, hp->Scale, model[1], texture[1], true);
}
void Player::View()
{
	camera->View(camera_Pos, camera_Rot);
}

//-----------------------------------------------------------------------------------------------------------
//	以下プレイヤーの攻撃関連の動作を記述
//-----------------------------------------------------------------------------------------------------------
void Player::Move()
{
	//移動する	キーボードで操作
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


	//カメラ移動		マウスの動きに合わせる
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
		if (camera_Rot.x > D3DXToRadian(15))
		{
			camera_Rot.x = D3DXToRadian(15);
		}

	}

	//プレイヤーをカメラの中心にとらえる
	camera_Pos.x = Position.x - 10 * sinf(cameraAngle);
	camera_Pos.z = Position.z - 10 * cosf(cameraAngle);

	camera_Rot.y += dims.lX * 0.005f;
	Rotation.y = camera_Rot.y + D3DXToRadian(0);
	hp->Pos.x = Position.x - 3 * cosf(cameraAngle);
	hp->Pos.y = Position.y + 10 * (sinf(camera_Rot.x * -1) + cosf(camera_Rot.x * -1));
	hp->Pos.z = Position.z + 3 * sinf(cameraAngle);
	hp->Rot.y = Rotation.y + D3DXToRadian(-20);
	oldPlayerRot.y = Rotation.y;
	oldCameraRot.x = camera_Rot.x;

	inputState = oldPlayerPos;
}

//	ショット
void Player::Shot()
{
	BulletShot();
	BombShot();
	RazerShot();
}

//	弾丸のショット
void Player::BulletShot()
{
	if (GetAsyncKeyState(VK_LBUTTON) && pBullet->flag == false && pRazer->flag == false)
	{
		/*
		*	左クリック＆＆bullet_flagがfalseのときに初期化されるもの
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
		*	弾の処理を記述	泡のように見せるため乱数とD3DXVec3Normalizeを使用
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
					bullet->Draw(&pBullet->Pos[i], 0.7f, BULLET_MAX, true);
				}

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
			}
		}
		bulletState[i] = pBullet->Pos[i];
	}

}

//	爆弾のショット
void Player::BombShot()
{
	if (GetAsyncKeyState(VK_RBUTTON) && pBomb->flag == false && pRazer->flag == false)
	{
		srand((unsigned int)time(NULL));

		/*
		*	左クリック＆＆bomb_flagがfalseのときに初期化されるもの
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
		if (pBomb->Pos.y > -30)
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
		if (pBomb->Pos.y < -30)
		{
			pBomb->death = true;
		}
		for (int i = 0; i < BOMB_MAX; i++)
		{
			if (pBomb->Pos.y < -30)
			{
				pBomb->Explosion_Exist[i] = true;
				pBomb->Explosion_Pos[i] = pBomb->oldPos;
				pBomb->MinRange = D3DXVECTOR3(pBomb->Explosion_Pos[i].x - 10, -40, pBomb->Explosion_Pos[i].z - 10);
				pBomb->MaxRange = D3DXVECTOR3(pBomb->Explosion_Pos[i].x + 10, -20, pBomb->Explosion_Pos[i].z + 10);
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
					pBomb->Explosion_Pos[i].x += (pBomb->Explosion_Accel[i].x + i * 0.005f) * 50.0f;
					pBomb->Explosion_Pos[i].y += rand() % 10 * 0.0008f;
					pBomb->Explosion_Pos[i].z += (pBomb->Explosion_Accel[i].z + i * 0.005f) * 50.0f;
					bomb->Draw(&pBomb->Explosion_Pos[i], pBomb->Explosion_Size, pBomb->Explosion_Alpha);
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

//	レーザーのショット
void Player::RazerShot()
{
	if (GetAsyncKeyState(VK_SPACE) && pRazer->flag == false)
	{
		srand((unsigned int)time(NULL));

		/*
		*	左クリック＆＆bomb_flagがfalseのときに初期化されるもの
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

//	当たり判定
void Player::Destroy()
{
	extern D3DXVECTOR3 enemyBulletPos[ENEMY_MAX];
	extern D3DXVECTOR3 UenemyBulletPos[ENEMY_MAX];
	extern D3DXVECTOR3 enemy_Collider[ENEMY_MAX];
	extern D3DXVECTOR3 Boss_Collider;

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		player_Radius = 10.0f;
		enemyBullet_Radius[i] = 8.0f;
		UenemyBullet_Radius[i] = 4.0f;
		enemy_Radius[i] = 8.0f;
		//----------------------------------------------------------------
		//	敵の弾との当たり判定
		//----------------------------------------------------------------
		//	プレイヤーと同じ位置の敵の弾
		if ((enemyBulletPos[i].x - inputState.x) * (enemyBulletPos[i].x - inputState.x) +
			(enemyBulletPos[i].y - inputState.y) * (enemyBulletPos[i].y - inputState.y) +
			(enemyBulletPos[i].z - inputState.z) * (enemyBulletPos[i].z - inputState.z) <=
			(player_Radius + enemyBullet_Radius[i]) * (player_Radius + enemyBullet_Radius[i]))
		{
			playerHitFlag = true;
			Vitality -= 1;
			hp->Scale.x -= 0.1;
			hp->Scale.z -= 0.1;
		}

		//	プレイヤーより下の位置の敵の弾
		if ((UenemyBulletPos[i].x - inputState.x) * (UenemyBulletPos[i].x - inputState.x) +
			(UenemyBulletPos[i].y - inputState.y) * (UenemyBulletPos[i].y - inputState.y) +
			(UenemyBulletPos[i].z - inputState.z) * (UenemyBulletPos[i].z - inputState.z) <=
			(player_Radius + UenemyBullet_Radius[i]) * (player_Radius + UenemyBullet_Radius[i]))
		{
			playerHitFlag = true;
			Vitality -= 2;
			hp->Scale.x -= 0.2;
			hp->Scale.z -= 0.2;
		}

		//----------------------------------------------------------------
		//	敵の弾との当たり判定
		//----------------------------------------------------------------
		//	プレイヤーと同じ位置の敵
		if ((enemy_Collider[i].x - inputState.x) * (enemy_Collider[i].x - inputState.x) +
			(enemy_Collider[i].y - inputState.y) * (enemy_Collider[i].y - inputState.y) +
			(enemy_Collider[i].z - inputState.z) * (enemy_Collider[i].z - inputState.z) <=
			(player_Radius + enemy_Radius[i]) * (player_Radius + enemy_Radius[i]))
		{
			playerHitFlag = true;
			Vitality -= 0.3;
			hp->Scale.x -= 0.03;
			hp->Scale.z -= 0.03;
		}
		if (Vitality < 0)
		{
			playerDeath = true;
		}
	}

	//	ボス
	Boss_Radius = 110;
	if ((Boss_Collider.x - inputState.x) * (Boss_Collider.x - inputState.x) +
		(Boss_Collider.y - inputState.y) * (Boss_Collider.y - inputState.y) +
		(Boss_Collider.z - inputState.z) * (Boss_Collider.z - inputState.z) <=
		(player_Radius + Boss_Radius) * (player_Radius + Boss_Radius))
	{
		playerHitFlag = true;
		Vitality -= 2;
		hp->Scale.x -= 0.2;
		hp->Scale.z -= 0.2;
	}
	if (Vitality < 0)
	{
		playerDeath = true;
	}



	//------------------------------------------------------------------------------------------
	//	当たり判定後の処理
	//------------------------------------------------------------------------------------------
	if (playerHitFlag == true)
	{
		Count++;
		D3DXVec3Normalize(&Accel, &D3DXVECTOR3(rand() % 10, rand() % 10, rand() % 10));
		Scale.x += Accel.x * 0.5f * x_Speed;
		Scale.y += Accel.y * 0.5f * y_Speed;
		Scale.z += Accel.z * 0.5f * z_Speed;
		
		if (Scale.x > 27)
		{
			x_Speed = -1;
		}
		if (Scale.y > 27)
		{
			y_Speed = -1;
		}
		if (Scale.z > 27)
		{
			z_Speed = -1;
		}

		if (Scale.x < 23)
		{
			x_Speed = 1;
		}
		if (Scale.y < 23)
		{
			y_Speed = 1;
		}
		if (Scale.z < 23)
		{
			z_Speed = 1;
		}

	}
	if (Count > 20)
	{
		Count = 0;
		Scale = D3DXVECTOR3(25, 25, 25);
		playerHitFlag = false;
	}
}

