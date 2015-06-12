/*
*	�^�C�g����ʂ̃N���X
*
*	2015/06/03		Y.Ozawa
*/

#pragma once


class Graphic;
class Texture;
class Camera;

//���q�̃f�[�^�[
struct f_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};


class Failed
{
	D3DXVECTOR3 MinRange;
	D3DXVECTOR3 MaxRange;
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Rot;
	D3DXVECTOR3 Scale;
	D3DXVECTOR3 camera_Pos;
	D3DXVECTOR3 camera_Rot;

private:
	Texture *texture;

	Graphic *graphic;

	f_point *point;

	Camera *camera;
public:

	//�f�t�H���g�R���X�g���N�^
	Failed();

	//�f�X�g���N�^
	~Failed();

	//�B�e����
	void View();

	//	������
	void Init();

	//	���
	void Release();
};