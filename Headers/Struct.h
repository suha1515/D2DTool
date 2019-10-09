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
typedef struct tagMapDB
{
	TCHAR   objectkey[256] = L"";
	TCHAR	fileName[256] = L"";
	TCHAR	filePath[256] = L"";
	int     Index;
	//MAP_INFO info;
}MAP_SAVE;

#ifdef TOOL
typedef struct tagMapInfo
{
	int		i_Index = 0;
	wstring s_fileName = L"";
	wstring s_filePath = L"";
}MAP_INFO;


typedef struct tagMapTile
{
	D3DXVECTOR3 pos;
	XMFLOAT2 tex[4];
	TCHAR  texture[256] = L"";
}MAP_TILE;

#endif

typedef struct tagClip_Info
{
	float			  _startTime=0.0f;					//애니메이션 플레이시간
	D3DXVECTOR3		  _pos;								//애니메이션 위치
	XMFLOAT3		  _rot;								//애니메이션 회전
	D3DXVECTOR3		  _Scale;							//애니메이션 크기
	XMFLOAT2		  _Tex[4];							//프레임 텍스쳐 좌표

	TCHAR 			_texName[256] = L"";				//텍스쳐 이름
	float			_texSizeX=0.0f;						//텍스쳐 사이즈x
	float			_texSizeY=0.0f;						//텍스쳐 사이즈y
	int				_index;								//클립 인덱스
}CLIP_INFO;
typedef struct tagAnimationClip
{
	TCHAR				_clipName[256] = L"";
	int					_clipSize = 0;
	vector<CLIP_INFO>	_clips;
}ANIM_CLIP;

typedef struct tagAnimation
{
	TCHAR							_AnimName[256] = L"";
	int								_AnimSize;
	map<wstring, vector<ANIM_CLIP>>	_AnimClip;
}ANIMATION;

typedef struct ObjectInfo
{
	TCHAR				_ObjectName[256] = L"";
	TCHAR				_ObjectTag[256] = L"";
	Layer				_ObjectLayer;
	int					_ObjectLevel;
	TCHAR				_ParentObject[256] = L"";
}OBJ_INFO;

typedef struct TransformInfo
{
	D3DXVECTOR3		   _ObjectPos;
	D3DXVECTOR3		   _ObjectScale;
	XMFLOAT3		   _ObjectRotation;
}TRANSFORM_INFO;

typedef struct TextureInfo
{
	XMFLOAT2		   _TexturPos[4];
	XMFLOAT2		   _TextureSize;
	TCHAR			   _TextrueName[256] = L"";
}TEXTURE_INFO;

typedef struct BoxColliderInfo
{
	float			  _BoxWidth, _BoxHeight;
	float			  _BoxOffsetX, _BoxOffsetY;
}BOXCOL_INFO;

typedef struct AnimatorInfo
{
	TCHAR			_AnimationName[256] = L"";
}ANIM_INFO;
//스크립트 인포는 읽기전에 몇개를 읽어야하는지 알아야함.
typedef struct ScriptInfo
{
	TCHAR			_ScriptName[256] = L"";
}SCRIPT_INFO;

typedef struct ComponentCount
{
	int			_Transform=0;
	int			_Texture=0;
	int			_BoxCol=0;
	int			_Animator=0;
}COMP_INFO;

typedef struct CopyObject
{
	OBJ_INFO objInfo;
	TRANSFORM_INFO transformInfo;
	TEXTURE_INFO   textureInfo;
	BOXCOL_INFO		boxcolInfo;
	ANIM_INFO		animInfo;
	vector<SCRIPT_INFO>		scriptInfo;
	COMP_INFO		compInfo;
	vector<CopyObject>	childInfo;
}OBJ_COPY;
#define __STRUCT_H__
#endif