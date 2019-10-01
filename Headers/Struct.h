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
	D3DXIMAGE_INFO     tImgInfo;
	wstring			textureName;
}TEX_INFO;

typedef struct tagInfo
{
	D3DXVECTOR3	vPos;	// ��ġ����
	D3DXVECTOR3 vDir;	// ���⺤��
	D3DXVECTOR3 vLook;	// ���غ���
	D3DXVECTOR3 vSize;
	D3DXMATRIX matWorld; // ������� = ũ����� * ������� * �̵���� * ������� * �θ����
}INFO;

#ifdef TOOL
typedef struct tagMapInfo
{
	int		i_Index = 0;
	wstring s_fileName = L"";
	wstring s_filePath = L"";
}MAP_INFO;

typedef struct tagMapDB
{
	TCHAR   objectkey[256] = L"";
	TCHAR	fileName[256]  = L"";
	TCHAR	filePath[256]  = L"";
	int     Index;
	//MAP_INFO info;
}MAP_SAVE;

typedef struct tagMapTile
{
	D3DXVECTOR3 pos;
	XMFLOAT2 tex[4];
	TCHAR  texture[256] = L"";
}MAP_TILE;

#endif

typedef struct tagAnimationClip
{
	float			  _startTime;
	D3DXVECTOR3		  _pos;
	XMFLOAT3		  _rot;
	D3DXVECTOR3		  _Scale;
	XMFLOAT2		  _Tex[4];
}ANIM_CLIP;

typedef struct tagAnimation
{
	wstring				_texName;
	float				_texSizeX;
	float				_texSizeY;
	vector<ANIM_CLIP>	_clips;
}ANIMATION;

#define __STRUCT_H__
#endif