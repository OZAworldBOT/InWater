/*
*	3D���f���ǂݍ���
*
*	2015/05/19		Y.Ozawa
*/

#include "Lib.h"

extern LPDIRECT3DDEVICE9 d3dDevice;

//	�f�t�H���g�R���X�g���N�^
Model::Model()
{
	mesh = nullptr;
	buffer = nullptr;
	numMaterial = 0;
}

//	�R���X�g���N�^
Model::Model(const TCHAR *meshFileName)
{
	LoadMesh(meshFileName);
}

//	�f�X�g���N�^
Model::~Model()
{
	SafeRelease(buffer);
	SafeRelease(mesh);
}

//	���b�V����ǂݍ���
bool Model::LoadMesh(const TCHAR *meshFileName)
{
	if (FAILED(D3DXLoadMeshFromX(meshFileName, D3DXMESH_SYSTEMMEM, d3dDevice, nullptr, &buffer, nullptr, &numMaterial, &mesh)))
	{
		DebugAlert("%s��\n�ǂݍ��߂܂���ł����B\n�t�@�C������f�B���N�g�����ԈႦ�Ă��Ȃ������m�F���������B", meshFileName);
		return false;
	}

	return true;
}
















