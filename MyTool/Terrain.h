#pragma once
#include "GameObject.h"

class CMyToolView;
class CTerrain :
	public CGameObject
{
private:
	CTerrain();

public:
	virtual ~CTerrain();

public:
	int GetTileIndex(const D3DXVECTOR3& vPos);

public:
	void SetView(CMyToolView* pView);

public:
	// CGameObject을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInit() override;
	virtual void Release() override;

public:
	void TileChange(const D3DXVECTOR3& vPos, BYTE byDrawID, BYTE byOption = 0);
	void MiniRender();

private:
	bool IsPicking(const D3DXVECTOR3& vPos, size_t iIndex);

public:
	static CTerrain* Create(CMyToolView* pView);

private:
	vector<TILE_INFO*>	m_vecTile;
	CMyToolView*			m_pView;
};

