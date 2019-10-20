
#include "EffectScript.h"
class CBossRockSkill :
	public CEffectScript
{
	enum STATE{DEAD,LOCKING,FALLING,CRUSHING,PARTICLE};
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

	void	AttackState();
	void	Destruct();
	void	Throw();

private:
	float		m_fLockOnTime;
	float		m_fFlickerTIme;
	bool		m_bFlicker;
	bool		m_bRockFalling;

private:
	D3DXVECTOR3*	m_Pos;

	STATE		m_CurState;
	STATE		m_PreState;

	CGameObject*	m_pRock;
	D3DXVECTOR3     m_PreRockPos;
	D3DXVECTOR3*	m_RockPos;
	float			m_fFallingTime;
	float			m_fRadius;

	float			m_fRemainTime;

	bool			m_bRemain;
	vector<tuple<CGameObject*, D3DXVECTOR3, D3DXVECTOR3,float>> m_fragment;

	CGameObject*	m_pPlayer;

};
