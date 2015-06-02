/*
*	�I�u�W�F�N�g
*
*	2015/05/19		Y.Ozawa
*/

#include "Lib.h"
#include <memory>

#define VERTEX3_FVF (D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE)

extern LPDIRECT3DDEVICE9 d3dDevice;

// �f�t�H���g�R���X�g���N�^
Graphic::Graphic()
{
	// �|�C���g�X�v���C�g�̃o�b�t�@�̏�����
	if (FAILED(d3dDevice->CreateVertexBuffer(10000 * sizeof(Vertex3), D3DUSAGE_WRITEONLY, VERTEX3_FVF, D3DPOOL_MANAGED, &pointBuffer, nullptr)))
		DebugAlert("���_�o�b�t�@��\n�쐬�ł��܂���ł����B");

	// �t�H���g�̏�����
	D3DXCreateFont(d3dDevice,
		32,							// �����̍���
		16,							// �t�H���g�̕����̕�
		FW_NORMAL,					// �t�H���g�̃E�F�C�g
		0,							// �v�������~�b�v���x���̐�
		false,						// �Α̃t�H���g�̏ꍇ��true
		0,							// �����Z�b�g
		0,							//�o�͐��x
		DEFAULT_QUALITY,			//�o�͕i��
		DEFAULT_PITCH || FF_DONTCARE,	//�t�H���g�̃s�b�`�ƃt�@�~��
		"",							//�t�H���g�̏���
		&font);

	D3DXMatrixIdentity(&m_world);
}

// �f�X�g���N�^
Graphic::~Graphic()
{
	SafeRelease(pointBuffer);
	SafeRelease(font);
}

// 3D���f����`�悷��
void Graphic::DrawModel(D3DXVECTOR3 &position, D3DXVECTOR3 &rotation, D3DXVECTOR3 &scale, Model &model)
{

	// ���[���h�s��̐ݒ�
	TransForm(position, rotation, scale);
	d3dDevice->SetTransform(D3DTS_WORLD, &m_world);

	d3dDevice->SetTexture(0, nullptr);

	// ���f���̕`��
	D3DXMATERIAL *materials = (D3DXMATERIAL *)model.buffer->GetBufferPointer();
	for (DWORD i = 0; i < model.numMaterial; i++)
	{
		// �}�e���A���̐ݒ�
		d3dDevice->SetMaterial(&materials[i].MatD3D);

		// �������ꂽ���b�V���̕`��
		model.mesh->DrawSubset(i);
	}
}

// 3D���f����`�悷��
void Graphic::DrawModelTexture(D3DXVECTOR3 &position, D3DXVECTOR3 &rotation, D3DXVECTOR3 &scale, Model &model, Texture &texture)
{
	// ���[���h�s��̐ݒ�
	TransForm(position, rotation, scale);
	d3dDevice->SetTransform(D3DTS_WORLD, &m_world);

	d3dDevice->SetTexture(0, texture.texture);

	//�A���t�@�u�����f�B���O���s��
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	// ���f���̕`��
	D3DXMATERIAL *materials = (D3DXMATERIAL *)model.buffer->GetBufferPointer();
	for (DWORD i = 0; i < model.numMaterial; i++)
	{
		// �}�e���A���̐ݒ�
		d3dDevice->SetMaterial(&materials[i].MatD3D);


		// �������ꂽ���b�V���̕`��
		model.mesh->DrawSubset(i);
	}
	//�A���t�@�u�����h���I��点��
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

}

// �|�C���g�X�v���C�g��`�悷�� �A���t�@�u�����hOFF
void Graphic::DrawPointSprite(Vertex3 point[], int numPoint, Texture &texture)
{
	Vertex3 *vertex;

	pointBuffer->Lock(0, 0, (LPVOID*)&vertex, 0);
	for (int i = 0; i < numPoint; i++)
	{
		// �|�C���g�X�v���C�g�̈ʒu�̌v�Z
		vertex[i].pos = point[i].pos;

		// �|�C���g�X�v���C�g�̃T�C�Y��ݒ�
		vertex[i].size = point[i].size;

		// �|�C���g�X�v���C�g�̐F��ݒ�
		vertex[i].color = point[i].color;
	}
	pointBuffer->Unlock();

	// �e�N�X�`�����p�C�v���C���ɃZ�b�g
	d3dDevice->SetTexture(0, texture.texture);

	// ���[���h�ϊ��}�g���b�N�X���p�C�v���C���ɃZ�b�g
	D3DXMATRIX m_world;
	D3DXMatrixIdentity(&m_world);
	d3dDevice->SetTransform(D3DTS_WORLD, &m_world);

	//�A���t�@�u�����f�B���O���s��
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// �|�C���g�X�v���C�g�̕`��
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);	// Z��r���s��Ȃ�
	d3dDevice->SetStreamSource(0, pointBuffer, 0, sizeof(Vertex3));
	d3dDevice->SetFVF(VERTEX3_FVF);
	d3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, numPoint);
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);		// Z��r���s��

	//�A���t�@�u�����h���I��点��
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

// �|�C���g�X�v���C�g��`�悷�� �A���t�@�u�����hON
void Graphic::DrawPointSprite_A(Vertex3 point[], int numPoint, Texture &texture, bool alpha)
{
	Vertex3 *vertex;

	pointBuffer->Lock(0, 0, (LPVOID*)&vertex, 0);
	for (int i = 0; i < numPoint; i++)
	{
		// �|�C���g�X�v���C�g�̈ʒu�̌v�Z
		vertex[i].pos = point[i].pos;

		// �|�C���g�X�v���C�g�̃T�C�Y��ݒ�
		vertex[i].size = point[i].size;

		// �|�C���g�X�v���C�g�̐F��ݒ�
		vertex[i].color = point[i].color;
	}
	pointBuffer->Unlock();

	// �e�N�X�`�����p�C�v���C���ɃZ�b�g
	d3dDevice->SetTexture(0, texture.texture);

	// ���[���h�ϊ��}�g���b�N�X���p�C�v���C���ɃZ�b�g
	D3DXMATRIX m_world;
	D3DXMatrixIdentity(&m_world);
	d3dDevice->SetTransform(D3DTS_WORLD, &m_world);

	//�A���t�@�u�����f�B���O���s��
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// �|�C���g�X�v���C�g�̕`��
	d3dDevice->SetRenderState(D3DRS_ZENABLE, alpha);	// Z��r���s��Ȃ�
	d3dDevice->SetStreamSource(0, pointBuffer, 0, sizeof(Vertex3));
	d3dDevice->SetFVF(VERTEX3_FVF);
	d3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, numPoint);
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);		// Z��r���s��

	//�A���t�@�u�����h���I��点��
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

// ������̕`��
void Graphic::DrawString(RECT rect, const TCHAR *str)
{
	font->DrawText(nullptr,
		str,
		-1,
		&rect,
		DT_CENTER | DT_VCENTER | DT_NOCLIP,
		D3DCOLOR_XRGB(255, 255, 255));
}

// ���W�ϊ�
void Graphic::TransForm(D3DXVECTOR3 &position, D3DXVECTOR3 &rotation, D3DXVECTOR3 &scale)
{
	D3DXMATRIX m_temp;
	D3DXMatrixIdentity(&m_world);
	D3DXMatrixIdentity(&m_temp);

	// �s����g��k������
	D3DXMatrixScaling(&m_temp, scale.x, scale.y, scale.z);
	m_world *= m_temp;

	// �s�����]����
	D3DXMatrixRotationYawPitchRoll(&m_temp, rotation.y, rotation.x, rotation.z);
	m_world *= m_temp;

	// �s��𕽍s�ړ�������
	D3DXMatrixTranslation(&m_temp, position.x, position.y, position.z);
	m_world *= m_temp;
}


