#pragma once

#ifndef __STRUCT_H__

typedef struct tagFrame
{
	float fStartFrame;
	float fMaxFrameCnt;
}FRAME;

typedef struct tagTile
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	BYTE byDrawID;
	BYTE byOption;
}TILE_INFO;

typedef struct tagTexture
{
	// IDirect3DTexture9�� ������
	// IDirect3DTexture9: �ҷ��� �ؽ�ó�� �����ϴ� Com��ü.
	LPDIRECT3DTEXTURE9	pTexture;

	// ����� �ؽ�ó�� ������ �����ִ� ����ü.
	D3DXIMAGE_INFO tImgInfo;
}TEX_INFO;

typedef struct tagInfo
{
	D3DXVECTOR3	vPos;	// ��ġ����
	D3DXVECTOR3 vDir;	// ���⺤��
	D3DXVECTOR3 vLook;	// ���غ���
	D3DXVECTOR3 vSize;
	D3DXMATRIX matWorld; // ������� = ũ����� * ������� * �̵���� * ������� * �θ����
}INFO;

#define __STRUCT_H__
#endif