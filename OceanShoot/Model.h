/*
*	3D���f���ǂݍ���
*
*	2015/05/19		Y.Ozawa
*/

#pragma once

class Model
{
public:

	LPD3DXMESH mesh;		//	���b�V��
	LPD3DXBUFFER buffer;	//	���_�o�b�t�@
	DWORD numMaterial;		//	�}�e���A���̐�

public:

	//	�f�t�H���g�R���X�g���N�^
	Model();

	//	�R���X�g���N�^
	Model(const TCHAR *meshFileName);

	//	�f�X�g���N�^
	~Model();

	//	���b�V�������[�h����
	bool LoadMesh(const TCHAR *meshFileName);


};