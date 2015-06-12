/*
*	�G�̃N���X
*
*	2015/05/28		Y.Ozawa
*/

#pragma once

class Texture;
class Model;
class Graphic;
class Bullet;

class Enemy
{
	//-----------------------------------------------------
	//	�G�֘A
	//-----------------------------------------------------
	Graphic *enemy;							//	X�t�@�C��
	struct EnemyState
	{
		D3DXVECTOR3 Position[ENEMY_MAX];		//	�G�̍��W
		D3DXVECTOR3 Rotation[ENEMY_MAX];		//	�G�̌X��
		D3DXVECTOR3 Accel[ENEMY_MAX];			//	�G�̏���
		D3DXVECTOR3 Scale[ENEMY_MAX];			//	�G�̊g�嗦
		D3DXVECTOR3 oldEnemyPos[ENEMY_MAX];		//	�O�t���[���̓G�̍��W
		D3DXVECTOR3 oldEnemyRot[ENEMY_MAX];		//	�O�t���[���̓G�̌X��
		D3DXVECTOR3 Collider[ENEMY_MAX];		//	�G�̒��S���W
		D3DXVECTOR3 MaxRange;					//	�G�o���̍ő�͈�
		D3DXVECTOR3 MinRange;					//	�G�o���̍ŏ��͈�
		int			Vitality[ENEMY_MAX];		//	�G�̗̑�
		int			enemyDeathCount;			//	�G�ɍU��������������
		float		Radius[ENEMY_MAX];			//	�G�̔��a
		bool		enemyDeathFlag[ENEMY_MAX];	//	�G�������Ă邩�ǂ���
		bool		enemyHitFlag[ENEMY_MAX];	//	�G�ɓ�������
		bool		enemyBulletFlag;			//	�G�ɓ�������
		bool		enemyCountFlag[ENEMY_MAX];	//	�G�������Ă��鐔�𒲂ׂ�
		int			Speed[ENEMY_MAX];			//	���x�̐���
		int			x_Speed[ENEMY_MAX];			//	�g�嗦�̐���	x
		int			z_Speed[ENEMY_MAX];			//	�g�嗦�̐���	z
		int			y_Speed[ENEMY_MAX];			//	�g�嗦�̐���	y
		int			Count;						//	�G�̐�
	};
	//-----------------------------------------------------
	//	�U���֘A
	//-----------------------------------------------------
	struct EnemyAttack
	{
		D3DXVECTOR3 Pos;			//	�e�̍��W
		D3DXVECTOR3 Rot;			//	�e�̌X��
		D3DXVECTOR3 Accel;			//	�e�̏���
		D3DXVECTOR3 oldPos;			//	�O�t���[���̒e�̍��W
		int			Frame;			//	�o�߃t���[��
		int			Count;			//	�e�̐�������
		bool		Exist;			//	�e�������Ă邩�ǂ���
		bool		flag;			//	�e�̐���t���O
		bool		death;			//	�e���G�ɓ�������
		float		Radiu;			//	�e�̔��a
	};

	//-----------------------------------------------------
	//	�����蔻��֘A
	//-----------------------------------------------------
	float		bullet_Radius[BULLET_MAX];		//	�e�ۂ̔��a
	float		razer_Radius[RAZER_MAX];		//	���[�U�[�̔��a


private:

	float cameraAngle;		//�J�����̊p�x

private:

	Model *model;
	Texture *texture;
	Bullet *bullet;
	EnemyState *State;
	EnemyAttack *Attack;

public:

	//	�f�t�H���g�R���X�g���N�^
	Enemy();

	//	�f�X�g���N�^
	~Enemy();

	//	�G�̓���
	void Move();

	//	�`�悷��
	void Draw();

	//	������
	void InitEnemy();
	void InitBullet();

	//	�U��
	void Shot();

	//	�����蔻��
	void Hit();

	//	�����̏���
	void DestroyEnemy();

	//	���
	void Release();
};