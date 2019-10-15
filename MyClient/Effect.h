#pragma once

class CGameObject;
class CTextureRenderer;
class CTransform;
class CAnimator;
class CEffect
{
public:
	CEffect();
	~CEffect();
public:
	void Initialize();

public:
	void SetPosition(const D3DXVECTOR3& pos);
	void SetRotiation(const XMFLOAT3& rot);
	void SetScale(const D3DXVECTOR3& scale);

public:

	void SetEffectClip(const wstring& _ClipName, ANIMATION_TYPE type);
	void SetDeadTime(const float& time);

	static CGameObject* Create(D3DXVECTOR3& pos, XMFLOAT3& rot, D3DXVECTOR3& scale, const wstring& _ClipName, const wstring& _AnimName, ANIMATION_TYPE type = ANIMATION_ONCE, const float& time = 0.0f);


private:

};


