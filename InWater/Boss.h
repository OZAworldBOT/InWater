/*
*	�{�X
*
*	2016/06/10		Y.Ozawa
*/

#pragma once

class Texture;
class Model;
class Graphic;

class Boss
{
	//-----------------------------------------------------
	//	�G�֘A
	//-----------------------------------------------------
	Graphic *boss;						//	X�t�@�C��
	struct BossState
	{
		D3DXVECTOR3 Position;			//	�G�̍��W
		D3DXVECTOR3 Rotation;			//	�G�̌X��
		D3DXVECTOR3 Accel;				//	�G�̏���
		D3DXVECTOR3 Scale;				//	�G�̊g�嗦
		D3DXVECTOR3 oldEnemyPos;		//	�O�t���[���̓G�̍��W
		D3DXVECTOR3 oldEnemyRot;		//	�O�t���[���̓G�̌X��
		D3DXVECTOR3 Collider;			//	�G�̒��S���W
		D3DXVECTOR3 MaxRange;			//	�G�o���̍ő�͈�
		D3DXVECTOR3 MinRange;			//	�G�o���̍ŏ��͈�
		int			Vitality;			//	�G�̗̑�
		int			DeathCount;	//	�G�ɍU��������������
		float		Radius;				//	�G�̔��a
		bool		DeathFlag;			//	�G�������Ă邩�ǂ���
		bool		HitFlag;			//	�G�ɓ�������
		bool		BulletFlag;			//	�G�ɓ�������
		bool		CountFlag;			//	�G�������Ă��鐔�𒲂ׂ�
		int			Speed;				//	���x�̐���
		int			x_Speed;			//	�g�嗦�̐���	x
		int			z_Speed;			//	�g�嗦�̐���	z
		int			y_Speed;			//	�g�嗦�̐���	y
		int			Count;				//	�G�̐�
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
	BossState *State;

public:

	//	�f�t�H���g�R���X�g���N�^
	Boss();

	//	�f�X�g���N�^
	~Boss();

	//	�G�̓���
	void Move();

	//	�`�悷��
	void Draw();

	//	������
	void InitBoss();

	//	�����蔻��
	void Hit();

	//	�����̏���
	void DestroyEnemy();

	//	���
	void Release();
};
