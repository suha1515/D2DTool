
#include "EffectScript.h"
class CBossRockSkill :
	public CEffectScript
{
public:
	CBossRockSkill();
	~CBossRockSkill();

	// CScripts을(를) 통해 상속됨
	virtual void OnInit() override;
	virtual void OnEnable() override;
	virtual void OnCollision(CGameObject * pGameObject = nullptr, XMFLOAT2 * move = nullptr) override;
	virtual void OnInput() override;
	virtual int OnUpdate() override;
	virtual void OnLateUpdate() override;
	virtual void OnRender() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;

private:
	float		m_fLockOnTime;
	float		m_fFlickerTIme;
	bool		m_bFlicker;
};
