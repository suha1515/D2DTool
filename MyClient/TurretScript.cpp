#include "stdafx.h"
#include "TurretScript.h"

#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "BoxCollider.h"

CTurretScript::CTurretScript()
{
}


CTurretScript::~CTurretScript()
{
}

void CTurretScript::OnInit()
{
	m_pAnimator = m_pGameObject->GetComponent<CAnimator>();
	m_pTransform = m_pGameObject->GetComponent<CTransform>();
	m_pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();

	if (m_pAnimator != nullptr)
	{
		m_pAnimator->LoadClips(L"Turret");
		//�ʱ� Ŭ����Ʈ ����
		m_pAnimator->Play(L"Turret_Aim_Down", ANIMATION_TYPE::ANIMATION_LOOP);
	}

	m_pPlayer = CObjectMgr::GetInstance()->m_pPlayer;

	m_CurDir = DOWN;
	m_PreDir = m_CurDir;
	m_DirVec = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_CurState = IDLE;
	m_PreState = m_CurState;

	m_Pos = &m_pTransform->GetLocalPosition();

	m_Hp = 100.f;
}

void CTurretScript::OnEnable()
{
}

void CTurretScript::OnCollision(CGameObject * pGameObject , XMFLOAT2 * move)
{
}

void CTurretScript::OnInput()
{
}

int CTurretScript::OnUpdate()
{
	if (!m_bIsInit)
	{
		OnInit();
		m_bIsInit = true;
	}

	if (m_Hp < 0.0f)
	{
		m_pGameObject->SetObjectDestroy(true);
		return 0;
	}

	TrackPlayer();
	GetDirPlayer();
	DirState();
	AnimState();

	return 0;
}

void CTurretScript::OnLateUpdate()
{
}

void CTurretScript::OnRender()
{
}

void CTurretScript::OnDisable()
{
}

void CTurretScript::OnDestroy()
{
}

void CTurretScript::DirState()
{
	//360���� ��ȯ�ϱ� ���Ѱ�
	if (m_fAngle < 0.0f)
		m_fAngle = m_fAngle + 360.f;

	//���� 
	if (m_fAngle > 0.0f&&m_fAngle < 11.25f)
	{
		m_CurDir = RIGHT;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	//���� ������
	else if (m_fAngle > 348.75f&&m_fAngle < 360.f)
	{
		m_CurDir = RIGHT;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}	
	//���� ���
	else if (m_fAngle > 11.25f&&m_fAngle < 33.75f)
	{
		m_CurDir = RIGHT_22;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	//���� ���2 
	else if (m_fAngle > 33.75f&&m_fAngle < 56.25f)
	{
		m_CurDir = RIGHT_45;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	//���
	else if (m_fAngle > 56.25f&&m_fAngle < 78.75f)
	{
		m_CurDir = RIGHT_67;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	//���� ���
	else if (m_fAngle > 78.75f&&m_fAngle < 101.25f)
		m_CurDir = UP;
	//����
	else if (m_fAngle > 101.25f&&m_fAngle < 123.75f)
	{
		m_CurDir = LEFT_112;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
	//���� �ϴ�
	else if (m_fAngle > 123.75f&&m_fAngle < 146.25f)
	{
		m_CurDir = LEFT_135;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
	//�ϴ�
	else if (m_fAngle > 146.25f&&m_fAngle < 168.75f)
	{
		m_CurDir = LEFT_157;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
	//�����ϴ�
	else if (m_fAngle > 168.75f&&m_fAngle < 191.25f)
	{
		m_CurDir = LEFT;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
	else if (m_fAngle > 191.25f&&m_fAngle < 213.75f)
	{
		m_CurDir = LEFT_202;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
	else if (m_fAngle > 213.75f&&m_fAngle < 236.25f)
	{
		m_CurDir = LEFT_225;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
	else if (m_fAngle > 236.25f&&m_fAngle < 258.75f)
	{
		m_CurDir = LEFT_247;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
	else if (m_fAngle > 258.75f&&m_fAngle < 281.25f)
	{
		m_CurDir = DOWN;
		m_pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
	else if (m_fAngle > 281.25f&&m_fAngle < 303.75f)
	{
		m_CurDir = RIGHT_292;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	else if (m_fAngle > 303.75f&&m_fAngle < 326.25f)
	{
		m_CurDir = RIGHT_315;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
	else if (m_fAngle > 326.25f&&m_fAngle < 348.75f)
	{
		m_CurDir = RIGHT_337;
		m_pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	}
}

void CTurretScript::AnimState()
{
	if (m_CurState != m_PreState || m_CurDir != m_PreDir)
	{
		switch (m_CurState)
		{
		case AIM:
		case IDLE:
			//cout << "������" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Turret_Aim_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				m_pAnimator->Play(L"Turret_Aim_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_112:
			case RIGHT_67:
				m_pAnimator->Play(L"Turret_Aim_Right_Up3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_135:
			case RIGHT_45:
				m_pAnimator->Play(L"Turret_Aim_Right_Up2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_157:
			case RIGHT_22:
				m_pAnimator->Play(L"Turret_Aim_Right_Up1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Turret_Aim_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_202:
			case RIGHT_337:
				m_pAnimator->Play(L"Turret_Aim_Right_Down1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_225:
			case RIGHT_315:
				m_pAnimator->Play(L"Turret_Aim_Right_Down2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_247:
			case RIGHT_292:
				m_pAnimator->Play(L"Turret_Aim_Right_Down3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case SHOOT:
			//cout << "������" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Turret_Aim_Up_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				m_pAnimator->Play(L"Turret_Aim_Down_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_112:
			case RIGHT_67:
				m_pAnimator->Play(L"Turret_Aim_Right_Up3_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_135:
			case RIGHT_45:
				m_pAnimator->Play(L"Turret_Aim_Right_Up2_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_157:
			case RIGHT_22:
				m_pAnimator->Play(L"Turret_Aim_Right_Up1_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Turret_Aim_Right_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_202:
			case RIGHT_337:
				m_pAnimator->Play(L"Turret_Aim_Right_Down1_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_225:
			case RIGHT_315:
				m_pAnimator->Play(L"Turret_Aim_Right_Down2_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_247:
			case RIGHT_292:
				m_pAnimator->Play(L"Turret_Aim_Right_Down3_Shoot", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case CHARGE:
			//cout << "������" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Turret_Charge_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				m_pAnimator->Play(L"Turret_Charge_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_112:
			case RIGHT_67:
				m_pAnimator->Play(L"Turret_Charge_Right_Up3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_135:
			case RIGHT_45:
				m_pAnimator->Play(L"Turret_Charge_Right_Up2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_157:
			case RIGHT_22:
				m_pAnimator->Play(L"Turret_Charge_Right_Up1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Turret_Charge_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_202:
			case RIGHT_337:
				m_pAnimator->Play(L"Turret_Charge_Right_Down1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_225:
			case RIGHT_315:
				m_pAnimator->Play(L"Turret_Charge_Right_Down2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_247:
			case RIGHT_292:
				m_pAnimator->Play(L"Turret_Charge_Right_Down3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case CHARGE_SHOOT:
			//cout << "������" << endl;
			switch (m_CurDir)
			{
			case UP:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_112:
			case RIGHT_67:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Up3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_135:
			case RIGHT_45:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Up2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_157:
			case RIGHT_22:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Up1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_202:
			case RIGHT_337:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Down1", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_225:
			case RIGHT_315:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Down2", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_247:
			case RIGHT_292:
				m_pAnimator->Play(L"Turret_Charge_Shoot_Right_Down3", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;

			m_PreState = m_CurState;
		}
	}
}

void CTurretScript::Move()
{
}

void CTurretScript::GetHit(D3DXVECTOR3 dirVec, float power, float dmg)
{
}

void CTurretScript::GetDirPlayer()
{
}

void CTurretScript::TrackPlayer()
{
	if (m_pPlayer != nullptr)
	{
		D3DXVECTOR3 botPos = *m_pTransform->GetWorldPos();
		D3DXVECTOR3 playerPos = *m_pPlayer->GetComponent<CTransform>()->GetWorldPos();
		float dist = sqrtf((playerPos.x - botPos.x)*(playerPos.x - botPos.x) + (playerPos.y - botPos.y)*(playerPos.y - botPos.y));
		
		m_fAngle = GetAngle(botPos, playerPos);
		D3DXVECTOR3 dir = playerPos - botPos;
		D3DXVec3Normalize(&m_DirVec, &dir);

		cout << m_fAngle << endl;
			if (dist < 200.f&&dist>50.f)
			{		
			}
			else if (dist<50.f)
			{
			}
			else if (dist > 200.f)
			{
				m_CurState = IDLE;
			}
	}
}