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

//���q�̃f�[�^�[
struct e_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};


class Enemy
{
	//-----------------------------------------------------
	//	�G�֘A
	//-----------------------------------------------------
	Graphic *enemy;							//	X�t�@�C��
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
	float		Radius[ENEMY_MAX];			//	�G�̔��a
	bool		enemyDeathFlag[ENEMY_MAX];	//	�G�������Ă邩�ǂ���

	//-----------------------------------------------------
	//	�U���֘A
	//-----------------------------------------------------
	float		bullet_Radius[BULLET_MAX];		//	�e�ۂ̔��a
	float		razer_Radius[RAZER_MAX];		//	���[�U�[�̔��a
	float		bomb_Radius[BOMB_MAX];			//	���e�̔��a
	
private:

	float cameraAngle;		//�J�����̊p�x

private:

	Model *model;
	Texture *texture;
	Bullet *bullet;

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

	//	���
	void Release();
};








