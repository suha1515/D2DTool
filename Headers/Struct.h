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
	float			  _startTime=0.0f;					//�ִϸ��̼� �÷��̽ð�
	D3DXVECTOR3		  _pos;								//�ִϸ��̼� ��ġ
	XMFLOAT3		  _rot;								//�ִϸ��̼� ȸ��
	D3DXVECTOR3		  _Scale;							//�ִϸ��̼� ũ��
	XMFLOAT2		  _Tex[4];							//������ �ؽ��� ��ǥ

	TCHAR 			_texName[256] = L"";				//�ؽ��� �̸�
	float			_texSizeX=0.0f;						//�ؽ��� ������x
	float			_texSizeY=0.0f;						//�ؽ��� ������y
	int				_index;								//Ŭ�� �ε���
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
//��ũ��Ʈ ������ �б����� ��� �о���ϴ��� �˾ƾ���.
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