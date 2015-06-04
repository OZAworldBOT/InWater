/*
*	�^�C�g����ʂ̃N���X
*
*	2015/06/03		Y.Ozawa
*/

#pragma once


class Graphic;
class Texture;
class Camera;
class Model;

//���q�̃f�[�^�[
struct t_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};


class Title
{
	D3DXVECTOR3 MinRange;
	D3DXVECTOR3 MaxRange;
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Rot;
	D3DXVECTOR3 Scale;
	D3DXVECTOR3 camera_Pos;
	D3DXVECTOR3 camera_Rot;
	D3DXVECTOR3 tako_Pos;
	D3DXVECTOR3 tako_Rot;
	D3DXVECTOR3 tako_Scale;
	D3DXVECTOR3 tako_Accel;
	int			Speed1;
	int			Speed2;
	int			Rand;
	bool		tako_Flag;
private:
	Texture *texture;

	Graphic *graphic;

	Model *model;

	t_point *point;

	Camera *camera;
public:

	//�f�t�H���g�R���X�g���N�^
	Title();

	//�f�X�g���N�^
	~Title();

	void Move();

	//�B�e����
	void View();

	//	������
	void Init();

	//	���
	void Release();
};