/*
*	�{�X
*
*	2016/06/10		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Boss.h"

extern LPDIRECT3DDEVICE9 d3dDevice;
D3DXVECTOR3 Boss_Collider;

//�R���X�g���N�^
//	�R���X�g���N�^
Boss::Boss()
{
	boss = new Graphic();
	model = new Model("Model/whale.x");
	texture = new Texture("Texture/blue_skin.png");
	State = new BossState;

	DebugLog("�G�𐶐����܂����B\n");

	InitBoss();
}

//	�f�X�g���N�^
Boss::~Boss()
{
	Release();
}

//	�G�̏�����
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
	State->Vitality = 150;
	State->DeathCount = 0;
	State->DeathFlag = false;
	State->HitFlag = false;
	State->BulletFlag = false;
	State->Speed = 1;
	State->x_Speed = 1;
	State->z_Speed = 1;
	State->y_Speed = 1;

	DebugLog("�G�����������܂����B\n");
}

//	�������
void Boss::Release()
{
	delete boss;
	delete State;
	delete model;
	delete texture;

	DebugLog("�G��j�����܂����B\n");
}

//	�G�̕`��
void Boss::Draw()
{
	if (State->DeathFlag == false)
	{
		boss->DrawModelTexture(State->Position, State->Rotation, State->Scale, *model, *texture, true);
	}

	if (State->DeathCount >= ENEMY_MAX - 1)
	{
		InitBoss();
		boss->DrawModelTexture(State->Position, State->Rotation, State->Scale, *model, *texture, true);
	}

}

//	�G�̓���
void Boss::Move()
{
	srand((unsigned int)time(NULL));
	D3DXVec3Normalize(&State->Accel, &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
	State->Position.x += State->Accel.x * 0.06f;
	State->Position.z += State->Accel.z * 0.06f;
	State->Scale.x += (0.02 + State->Accel.x * 0.8) * State->x_Speed;
	State->Scale.z += (0.02 + State->Accel.x * 0.8) * State->z_Speed;
	State->Scale.y += (0.02 + State->Accel.y * 0.8) * State->y_Speed;
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
	State->oldEnemyPos = State->Position;
	Boss_Collider = State->oldEnemyPos;


}
//	�����蔻��
void Boss::Hit()
{
}

void Boss::DestroyEnemy()
{
	if (State->HitFlag == true)
	{
		D3DXVec3Normalize(&State->Accel, &D3DXVECTOR3(rand() % 10, rand() % 10, rand() % 10));
		State->Scale.x += State->Accel.x * 8.0f * State->x_Speed;
		State->Scale.y += State->Accel.y * 8.0f * State->y_Speed;
		State->Scale.z += State->Accel.z * 8.0f * State->z_Speed;

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



