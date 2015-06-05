/*
*	�v���C���[�̃N���X
*
*	2015/05/27		Y.Ozawa
*/

#pragma once

class Camera;
class Model;
class Texture;
class Graphic;
class BUllet;
class Bomb;
class Razer;
class Explosion;

//���q�̃f�[�^�[
struct p_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};

class Player
{
	float		timeCount;		//	�o�ߎ���

	//-----------------------------------------------------
	//	�v���C���[�֘A
	//-----------------------------------------------------
	//	X�t�@�C��
	Graphic		*player;
	D3DXVECTOR3 Position;						//	�v���C���[�̍��W
	D3DXVECTOR3 Rotation;						//	�v���C���[�̌X��
	D3DXVECTOR3 Scale;							//	�v���C���[�̊g�嗦
	D3DXVECTOR3 oldPlayerPos;					//	�O�t���[���̃v���C���[�̍��W
	D3DXVECTOR3 oldPlayerRot;					//	�O�t���[���̃v���C���[�̌X��
	float		enemy_Radius[ENEMY_MAX];		//	�G�̔��a

	//-----------------------------------------------------
	//	�J�����֘A
	//-----------------------------------------------------
	D3DXVECTOR3 camera_Pos;						//	�J�����̍��W
	D3DXVECTOR3 camera_Rot;						//	�J�����̌X��
	D3DXVECTOR3 oldCameraRot;					//	�O�t���[���̃J�����̌X��

	//-----------------------------------------------------
	//	�U���֘A
	//-----------------------------------------------------
	//	�ʏ�e
	struct Pbullet
	{
		D3DXVECTOR3 Pos[BULLET_MAX];			//	�e�̍��W
		D3DXVECTOR3 Rot[BULLET_MAX];			//	�e�̌X��
		D3DXVECTOR3 Accel[BULLET_MAX];			//	�e�̏���
		D3DXVECTOR3 oldPos[BULLET_MAX];			//	�O�t���[���̒e�̍��W
		int			Count[BULLET_MAX];			//	�e�̐�������
		bool		Exist[BULLET_MAX];			//	�e�������Ă邩�ǂ���
		bool		flag;						//	�e�̐���t���O
		bool		death[BULLET_MAX];			//	�e���G�ɓ�������
		float		Radius[BULLET_MAX];			//	�e�̔��a
	};
	//	���e
	struct Pbomb
	{
		D3DXVECTOR3 Pos;							//	���e�̍��W
		D3DXVECTOR3 Rot;							//	���e�̌X��
		D3DXVECTOR3 Accel;							//	���e�̏���
		D3DXVECTOR3 oldPos;							//	�O�t���[���̔��e�̍��W
		bool		Exist;							//	���e�������Ă邩�ǂ���
		bool		flag;							//	���e�̐���t���O
		bool		death;							//	���e���G�ɓ�������
		bool		Alpha;							//	Z�o�b�t�@�[���s�����ǂ���
		float		Radius;							//	���e�̔��a
		float		Size;							//	���e�̑傫��
		int			Count;							//	���e�̐�������
		D3DXVECTOR3 Explosion_Rot[BOMB_MAX];		//	�����̌X��
		D3DXVECTOR3 Explosion_Pos[BOMB_MAX];		//	�����̍��W
		D3DXVECTOR3 Explosion_Accel[BOMB_MAX];		//	�����̏���
		D3DXVECTOR3 oldExplosionPos[BOMB_MAX];		//	�O�t���[���̔����̍��W
		D3DXVECTOR3 MinRange;						//	�����̍ŏ��͈�
		D3DXVECTOR3 MaxRange;						//	�����̍ő�͈�
		bool		Explosion_Exist[BOMB_MAX];		//	�����������Ă邩�ǂ���
		bool		Explosion_Death[BOMB_MAX];		//	�������G�ɓ�������
		bool		Explosion_Alpha;				//	Z�o�b�t�@�[���s�����ǂ���
		float		Explosion_Radius[BOMB_MAX];		//	�����̔��a
		float		Explosion_Size;					//	�����̑傫��
		int			Explosion_Count[BOMB_MAX];		//	�����̐�������
	};
	//	���[�U�[
	struct Prazer
	{
		D3DXVECTOR3 Pos[RAZER_MAX];				//	���[�U�[�̍��W
		D3DXVECTOR3 Rot[RAZER_MAX];				//	���[�U�[�̌X��
		D3DXVECTOR3 Accel[RAZER_MAX];			//	���[�U�[�̏���
		D3DXVECTOR3 oldPos[RAZER_MAX];			//	�O�t���[���̃��[�U�[�̍��W
		int			Count[RAZER_MAX];			//	���[�U�[�̐�������
		bool		Exist[RAZER_MAX];			//	���[�U�[�������Ă邩�ǂ���
		bool		flag;						//	���[�U�[�̐���t���O
		bool		death[RAZER_MAX];			//	���[�U�[���G�ɓ�������
		float		Radius[RAZER_MAX];			//	���[�U�[�̔��a
	};

	//-----------------------------------------------------
	//	�����֘A
	//-----------------------------------------------------
	struct Pexplosion
	{
		D3DXVECTOR3 Pos[EXPLOSION_MAX];				//	�����̍��W
		D3DXVECTOR3 Rot[EXPLOSION_MAX];				//	�����̌X��
		D3DXVECTOR3 Accel[EXPLOSION_MAX];			//	�����̏���
		D3DXVECTOR3 oldPos[EXPLOSION_MAX];			//	�O�t���[���̔����̍��W
		int			Count[EXPLOSION_MAX];			//	�����̐�������
		bool		Exist[EXPLOSION_MAX];			//	�����������Ă邩�ǂ���
		bool		flag;							//	�����̐���t���O
	};

private:

	float cameraAngle;		//�J�����̊p�x

private:

	Camera		*camera;
	Model		*model;
	Texture		*texture;
	Bullet		*bullet;
	Bomb		*bomb;
	Razer		*razer;
	Explosion	*explosion;
	Pbullet		*pBullet;
	Pbomb		*pBomb;
	Prazer		*pRazer;
	Pexplosion	*pExp;

public:

	//	�f�t�H���g�R���X�g���N�^
	Player();

	//	�f�X�g���N�^
	~Player();

	//	�v���C���[�̓���
	void Move();

	//	�`�悷��
	void Draw();
	void View();

	//	������
	void InitPlayer();
	void InitBullet();
	void InitBomb();
	void InitRazer();
	void InitExplosion();

	//	�V���b�g
	void Shot();

	//	�e�ۂ̃V���b�g
	void BulletShot();

	//	���e�̃V���b�g
	void BombShot();

	//	���[�U�[�̃V���b�g
	void RazerShot();

	//	�����蔻��
	void Hit();

	//	����
	void CreateExp(D3DXVECTOR3 Pos[], int Num);

	//	���
	void Release();
};






