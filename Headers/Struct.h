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
	// IDirect3DTexture9의 포인터
	// IDirect3DTexture9: 불러온 텍스처를 조작하는 Com객체.
	LPDIRECT3DTEXTURE9	pTexture;

	// 사용할 텍스처의 정보를 갖고있는 구조체.
	D3DXIMAGE_INFO     tImgInfo;
	wstring			textureName;
}TEX_INFO;

typedef struct tagInfo
{
	D3DXVECTOR3	vPos;	// 위치벡터
	D3DXVECTOR3 vDir;	// 방향벡터
	D3DXVECTOR3 vLook;	// 기준벡터
	D3DXVECTOR3 vSize;
	D3DXMATRIX matWorld; // 월드행렬 = 크기행렬 * 자전행렬 * 이동행렬 * 공전행렬 * 부모행렬
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