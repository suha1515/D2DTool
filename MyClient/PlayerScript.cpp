#include "stdafx.h"
#include "PlayerScript.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "Animator.h"
#include "Camera.h"
#include "BulletScript.h"
#include "Mouse.h"

CPlayerScript::CPlayerScript()
{
	m_ScriptName = "CPlayerScript";
}


CPlayerScript::~CPlayerScript()
{
	delete m_playerFoot;
}

//������ ����Ŭ.
void CPlayerScript::OnInit()
{
	cout << "�÷��̾� �ʱ�ȭ" << endl;
	//Ű�Ŵ����� ������Ʈ ������Ʈ ������ ��������
	pKeyMgr = CKeyMgr::GetInstance();
	pAnimator = m_pGameObject->GetComponent<CAnimator>();
	pTransform = m_pGameObject->GetComponent<CTransform>();
	pBoxCollider = m_pGameObject->GetComponent<CBoxCollider>();
	pBoxCollider->SetCollideColor(D3DCOLOR_XRGB(164, 73, 164));
	//�ʱ� ���⼳��
	m_CurDir = DOWN;
	m_PreDir = m_CurDir;

	//�ʱ� ���ݹ��⼳�� (�÷��̾�� ���ʿ��������� �ֵθ���.)
	m_AtkDir = RIGHT_ATK;

	//�ʱ� �ִϸ��̼� ���� ����
	m_CurState = IDLE;
	m_PreState = m_CurState;

	//�ִϸ����Ͱ� ������� ���
	if (nullptr == pAnimator)
		MessageBox(0, L"�ִϸ����� ����", L"Error", 0);

	//ī�޶� �÷��̾ ���������
	CCameraMgr::GetInstance()->GetMainCamera()->Follow(m_pGameObject);

	//�ʱ�ӵ��� ���ӵ�
	m_fVelocity = 0.0f;
	m_fAcc = 0.0f;
	//�ʱ�ȭ�� �ϱ����� �Һ���.
	bIsInit = true;

	//�ʱ� Ŭ����Ʈ ����
	pAnimator->Play(L"Player_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);

	m_Right = false;
	m_Left = false;
	m_Down = true;
	m_Up = false;

	playerPos = &pTransform->GetLocalPosition();

	//�ʱ��� ����
	int layer = m_pGameObject->GetObjectLayer() - 1;

	m_CurLayer = (Layer)layer;
	m_PreLayer = m_CurLayer;

	//�÷��̾� �߷�Ʈ����
	m_playerFoot = new CBoxCollider;
	m_playerFoot->Initialize(m_pGameObject);
	m_playerFoot->SetBoxOffset( 0.0f, - 16.f);
	m_playerFoot->SetBoxSize(7.f, 5.f);
	m_playerFoot->SetBoxCollider();
	m_playerFoot->SetCollideColor(D3DCOLOR_XRGB(164, 73, 164));

	playerUpBox = pBoxCollider;
	playerDownBox = m_playerFoot;

	//����������
	m_GuideRange = 200.f;
}


void CPlayerScript::OnEnable()
{
}

void CPlayerScript::OnCollision(CGameObject * pGameObject, XMFLOAT2* move)
{
	if (nullptr != pGameObject)
	{

	}
}

void CPlayerScript::OnInput()
{
}

void CPlayerScript::OnUpdate()
{
	if (!bIsInit)
		OnInit();
	CTransform* pTransform = m_pGameObject->GetComponent<CTransform>();
	if (pTransform != nullptr)
	{
		m_PrePos = *playerPos;
		//Ÿ��Ȯ��
		CheckTiles();
		//���̾� Ȯ��
		CheckLayer();
		if (!m_bIsJump)
		{
			//����Ű �Է�
			MoveInput();
			//���콺 �Է�
			MouseInput();
			//�������� �Է�
			MeeleAttack();
			//���� ��ȭ.
			DirState();
			//���� ���� ��ȭ
			AtkState();
		}
		//�̵� �ִϸ��̼� ���º�ȭ
		AnimState();

		//�̵�
		Moving();
	}

	m_Right = false;
	m_Left = false;
	m_Down = false;
	m_Up = false;

	//Ÿ������
	m_NearTiles.clear();

	m_playerFoot->SetBoxCollider();
	
}

void CPlayerScript::OnLateUpdate()
{
}

void CPlayerScript::OnRender()
{
	if (m_bIsDebug)
	{
		m_playerFoot->DrawCollide();
		DrawGuideLine();
	}
}

void CPlayerScript::OnDisable()
{
}

void CPlayerScript::OnDestroy()
{
}

void CPlayerScript::MoveInput()
{
	//� Ű�� ��������� (����Ű)
	if (m_CurState != MEELE)
	{
		if (pKeyMgr->KeyPressing(KEY_LEFT) || pKeyMgr->KeyPressing(KEY_RIGHT)
			|| pKeyMgr->KeyPressing(KEY_UP) || pKeyMgr->KeyPressing(KEY_DOWN))
		{
			//cout << "Ű������" << endl;
			m_fVelocity += (3.0f*powf(m_fAcc, 2.0f));						//���ӵ� ���� ������ �ʿ䰡����
			m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime();

			m_fAcc = __min(2.0f, m_fAcc);
			m_fVelocity = __min(10.f, m_fVelocity);
			if (m_CurState == IDLE || m_CurState == RUN_END)
				m_CurState = RUN_START;
			else if (m_CurState == RUN_START && !pAnimator->IsPlaying())
			{
				if (m_CurState != THROW_END && !pAnimator->IsPlaying())
					m_CurState = RUN;
			}
		}
	}
	//���� Ű�� ����� (����Ű) ���!
	if (!pKeyMgr->KeyPressing(KEY_LEFT) && !pKeyMgr->KeyPressing(KEY_RIGHT)
		&& !pKeyMgr->KeyPressing(KEY_UP) && !pKeyMgr->KeyPressing(KEY_DOWN))
	{
		m_fVelocity -= (2.0f*powf(m_fAcc, 2.0f));
		m_fAcc -= CTimeMgr::GetInstance()->GetDeltaTime();

		m_fAcc = __max(0.5f, m_fAcc);
		m_fVelocity = __max(0.f, m_fVelocity);

		if (m_CurState == RUN || m_CurState == RUN_START)
			m_CurState = RUN_END;
		else if (m_CurState == RUN_END && !pAnimator->IsPlaying())
			m_CurState = IDLE;
	}
	//���������߿��� �������� �ʴ´�
	if (m_CurState != MEELE)
	{
		if (pKeyMgr->KeyPressing(KEY_LEFT))
		{
			m_Left = true;
		}
		else if (pKeyMgr->KeyPressing(KEY_RIGHT))
		{
			m_Right = true;
		}
		if (pKeyMgr->KeyPressing(KEY_UP))
		{
			m_Up = true;
		}
		else if (pKeyMgr->KeyPressing(KEY_DOWN))
		{
			m_Down = true;
		}
		//������ ����߿��� ������ �ٲ��� �ʴ´�.
		if (!m_bIsThrow)
		{
			if (m_Right)
				m_CurDir = RIGHT;
			else if (m_Left)
				m_CurDir = LEFT;
			if (m_Up)
				m_CurDir = UP;
			else if (m_Down)
				m_CurDir = DOWN;
			if (m_Right&&m_Up)
				m_CurDir = RIGHT_UP_45;
			else if (m_Right&&m_Down)
				m_CurDir = RIGHT_DOWN_45;
			else if (m_Left&&m_Up)
				m_CurDir = LEFT_UP_45;
			else if (m_Left&&m_Down)
				m_CurDir = LEFT_DOWN_45;
		}
		if (m_Right)
			m_CurMoveDir = RIGHT;
		else if (m_Left)
			m_CurMoveDir = LEFT;
		if (m_Up)
			m_CurMoveDir = UP;
		else if (m_Down)
			m_CurMoveDir = DOWN;
		if (m_Right&&m_Up)
			m_CurMoveDir = RIGHT_UP_45;
		else if (m_Right&&m_Down)
			m_CurMoveDir = RIGHT_DOWN_45;
		else if (m_Left&&m_Up)
			m_CurMoveDir = LEFT_UP_45;
		else if (m_Left&&m_Down)
			m_CurMoveDir = LEFT_DOWN_45;
	}
	else
		m_fVelocity = 0.0f;

	//����׸��
	if (pKeyMgr->KeyDown(KEY_P))
	{
		if (!m_bIsDebug)
		{
			CObjectMgr::GetInstance()->SetDebug(true);
			m_bIsDebug = true;
		}
		else
		{
			CObjectMgr::GetInstance()->SetDebug(false);
			m_bIsDebug = false;
		}
	}
	if (pKeyMgr->KeyDown(KEY_O))
	{
		if (!m_bIsLayerDebug)
		{
			CObjectMgr::GetInstance()->SetLayerDebug(true);
			m_bIsLayerDebug = true;
		}
		else
		{
			CObjectMgr::GetInstance()->SetLayerDebug(false);
			m_bIsLayerDebug = false;
		}
	}
	m_Right = false;
	m_Left = false;
	m_Down = false;
	m_Up = false;
}

void CPlayerScript::MouseInput()
{
	D3DXVECTOR3 mousePos = pKeyMgr->GetMouse()->GetMousePos();
	m_BulletAngle = GetAngle(*playerPos, mousePos);
	if (pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		cout << "������!" << endl;
		
		MouseDir();

		m_CurState = THROW;
		m_bIsThrow = true;
	}
	if (pKeyMgr->KeyPressing(KEY_LBUTTON))
	{
		m_ChargeCancle = 0.0f;
		m_bIsCharging = true;

		//���� �� �浹ü �ε���
		CheckRange(m_GuideLineLeftEndPoint, &m_LeftPointCollide);
		//���� �� �浹ü �ε���
		CheckRange(m_GuideLineRightEndPoint, &m_RightPointCollide);

		if (m_bIsCharged)
		{
			//m_CurState = AIM_WALK;
			CheckRange(m_GuideLineEndPoint, &m_MidPointCollide);
			MouseDir();
		}	
		if (m_ChargeTime > 1.3f)
		{
			m_bIsCharged = true;
		}
		else
		{
			m_ChargeTime += CTimeMgr::GetInstance()->GetDeltaTime();
			m_LeftGuideAngle += (LerpFloat(m_LefttempAngle, m_GuideAngle, m_ChargeTime / 1.3f)- m_LeftGuideAngle);
			if (m_GuideAngle == 0)
				m_RightGuideAngle += ( LerpFloat(m_RighttempAngle, m_GuideAngle + 360.f, m_ChargeTime / 1.3f)- m_RightGuideAngle);
			else
				m_RightGuideAngle += (LerpFloat(m_RighttempAngle, m_GuideAngle, m_ChargeTime / 1.3f) - m_RightGuideAngle);
		}
	}
	else
	{
		if(m_bIsCharged)
			m_bIsCharged = false;
		if (m_bIsCharging)
		{
			if (m_ChargeCancle > 0.7f)
			{
				m_bIsCharging = false;
				m_ChargeTime -= m_ChargeTime;
				m_LeftGuideAngle = m_LefttempAngle;
				m_RightGuideAngle = m_RighttempAngle;
				m_ChargeCancle -= m_ChargeCancle;
			}
			else
				m_ChargeCancle += CTimeMgr::GetInstance()->GetDeltaTime();	
		}
	}
	
}

void CPlayerScript::MeeleAttack()
{
	if (pKeyMgr->GetInstance()->KeyDown(KEY_V))
	{
		m_CurState = MEELE;
		m_bIsJump = true;
		m_JumpStartPos = *playerPos;
		m_JumpEndPos = D3DXVECTOR3(playerPos->x-32.f, playerPos->y+32.f, 0.0f);
		m_fJumpEndTime = 1.0f;
		m_JumpControlPos = D3DXVECTOR3(playerPos->x-10.f, playerPos->y+60.f, 0.0f);
	
	}
}

void CPlayerScript::Moving()
{
	float m_fSpeed = 0.5f;
	float m_moveX = 0.0f;
	float m_moveY = 0.0f;
	
	if (!m_bIsJump)
	{
		if (m_CurState == THROW || m_CurState == THROW_END)
		{
			m_fSpeed = 0.2f;
		}
		switch (m_CurMoveDir)
		{
		case UP:
			m_moveY = m_fSpeed*m_fVelocity;
			break;
		case DOWN:
			m_moveY = -1 * (m_fSpeed*m_fVelocity);
			break;
		case LEFT_UP_45:
			m_moveX = -1 * (m_fSpeed*m_fVelocity);
			m_moveY = m_fSpeed*m_fVelocity;
			break;
		case RIGHT_UP_45:
			m_moveX = m_fSpeed*m_fVelocity;
			m_moveY = m_fSpeed*m_fVelocity;
			break;
		case LEFT:
			m_moveX = -1 * (m_fSpeed*m_fVelocity);
			break;
		case RIGHT:
			m_moveX = m_fSpeed*m_fVelocity;
			break;
		case LEFT_DOWN_45:
			m_moveX = -1 * (m_fSpeed*m_fVelocity);
			m_moveY = -1 * (m_fSpeed*m_fVelocity);
			break;
		case RIGHT_DOWN_45:
			m_moveX = m_fSpeed*m_fVelocity;
			m_moveY = -1 * (m_fSpeed*m_fVelocity);
			break;
		}

		playerPos->x += m_moveX;
		m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
		if (StepStair())
			return;
		if (CollideTiles())
		{
			*playerPos = m_PrePos;
			m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
			cout << "x�浹��" << endl;
		}
		m_PrePos = *playerPos;
		playerPos->y += m_moveY;
		m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
		//��� Ȯ��
		if (StepStair())
			return;
		if (CollideTiles())
		{
			*playerPos = m_PrePos;
			m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
			cout << "y�浹��" << endl;
		}
	}
	else
	{
		Jump();
	}

}

void CPlayerScript::AnimState()
{
	if (m_CurState != m_PreState || m_CurDir != m_PreDir)
	{
		switch (m_CurState)
		{
		case IDLE:
			//cout << "������" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_Idle_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				pAnimator->Play(L"Player_Idle_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_Idle_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_Idle_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_Idle_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case RUN_START:
			//cout << "�ٱ� �غ�" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_ReadyRun_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				pAnimator->Play(L"Player_ReadyRun_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_ReadyRun_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_ReadyRun_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_ReadyRun_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		case RUN:
			//cout << "�ٱ�" << endl;
			switch (m_CurDir)
			{
			case UP:
				cout << "������ �ٱ�" << endl;
				pAnimator->Play(L"Player_Run_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				cout << "�Ʒ����� �ٱ�" << endl;
				pAnimator->Play(L"Player_Run_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
				cout << "���������� �ٱ�" << endl;
			case RIGHT_UP_45:
				cout << "������������ �ٱ�" << endl;
				pAnimator->Play(L"Player_Run_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
				cout << "���ʹ��� �ٱ�" << endl;
			case RIGHT:
				cout << "�����ʹ��� �ٱ�" << endl;
				pAnimator->Play(L"Player_Run_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
				cout << "���ʾƷ����� �ٱ�" << endl;
			case RIGHT_DOWN_45:
				cout << "�����ʾƷ����� �ٱ�" << endl;
				pAnimator->Play(L"Player_Run_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case  RUN_END:
			//cout << "�ٱ� ����" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_RunStop_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				pAnimator->Play(L"Player_RunStop_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_RunStop_RightUp", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_RunStop_Righ", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_RunStop_RightDown", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
			//���Ÿ� ����
		case THROW:
			//cout << "���Ÿ� ����" << endl;
			if (m_AtkDir == RIGHT_ATK)
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Throw_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Throw_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Throw_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Throw_Right", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Throw_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			else
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Throw_Up2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Throw_Down2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Throw_Right2_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Throw_Right2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Throw_Right2_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			break;
		case THROW_END:
			//cout << "���Ÿ� ���� �ߴ�." << endl;
			if (m_AtkDir == RIGHT_ATK)
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Throw_Up_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Throw_Down_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Throw_Right_Up_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Throw_Right_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Throw_Right_Down_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			else
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Throw_Up2_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Throw_Down2_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Throw_Right2_Up_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Throw_Right2_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Throw_Right2_Down_End", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			break;
		case MEELE:
			cout << "�ٰŸ� ����" << endl;
			if (m_AtkDir == RIGHT_ATK)
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Attack_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Attack_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Attack_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Attack_Right", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Attack_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			else
			{
				switch (m_CurDir)
				{
				case UP:
					pAnimator->Play(L"Player_Attack_Up2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case DOWN:
					pAnimator->Play(L"Player_Attack_Down2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_UP_45:
				case RIGHT_UP_45:
					pAnimator->Play(L"Player_Attack_Right_Up2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT:
				case RIGHT:
					pAnimator->Play(L"Player_Attack_Right2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				case LEFT_DOWN_45:
				case RIGHT_DOWN_45:
					pAnimator->Play(L"Player_Attack_Right_Down2", ANIMATION_TYPE::ANIMATION_ONCE);
					break;
				}
			}
			break;
		case JUMP:
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_Jump_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case DOWN:
				pAnimator->Play(L"Player_Jump_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_Jump_Right_Up", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_Jump_Right", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_Jump_Right_Down", ANIMATION_TYPE::ANIMATION_ONCE);
				break;
			}
			break;
		}
		m_PreState = m_CurState;
		m_PreDir = m_CurDir;
	}


}

void CPlayerScript::DirState()
{
	if (!m_bIsJump)
	{
		if (m_CurDir != m_PreDir)
		{
			switch (m_CurDir)
			{
			case UP:
				cout << "������" << endl;
				m_JumpControlPos = D3DXVECTOR3(playerPos->x, playerPos->y, 0.0f);
				m_GuideAngle = 90.f;

					m_LeftGuideAngle = 120.f;
					m_RightGuideAngle = 60.f;
					m_LefttempAngle = m_LeftGuideAngle;
					m_RighttempAngle = m_RightGuideAngle;

				break;
			case DOWN:
				cout << "�Ʒ�����" << endl;
				m_JumpControlPos = D3DXVECTOR3(playerPos->x, playerPos->y, 0.0f);
				m_GuideAngle = 270.f;

					m_LeftGuideAngle = 300.f;
					m_RightGuideAngle = 240.f;
					m_LefttempAngle = m_LeftGuideAngle;
					m_RighttempAngle = m_RightGuideAngle;
				
				break;
			case LEFT_UP_45:
				cout << "������" << endl;
				pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3(playerPos->x - 10.f, playerPos->y + 40.f, 0.0f);
				m_GuideAngle = 135.f;

					m_LeftGuideAngle = 165.f;
					m_RightGuideAngle = 105.f;
					m_LefttempAngle = m_LeftGuideAngle;
					m_RighttempAngle = m_RightGuideAngle;
				
				break;
			case RIGHT_UP_45:
				cout << "������" << endl;
				pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 40.f, 0.0f);
				m_GuideAngle = 45.f;

					m_LeftGuideAngle = 75.f;
					m_RightGuideAngle = 15.f;
					m_LefttempAngle = m_LeftGuideAngle;
					m_RighttempAngle = m_RightGuideAngle;
				
				break;
			case LEFT:
				cout << "����" << endl;
				pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 20.f, 0.0f);
				m_GuideAngle = 180.f;

					m_LeftGuideAngle = 210.f;
					m_RightGuideAngle = 150.f;
					m_LefttempAngle = m_LeftGuideAngle;
					m_RighttempAngle = m_RightGuideAngle;
				
				break;
			case RIGHT:
				cout << "������" << endl;
				pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 20.f, 0.0f);
				m_GuideAngle = 0.f;

					m_LeftGuideAngle = 30.f;
					m_RightGuideAngle = 330.f;
					m_LefttempAngle = m_LeftGuideAngle;
					m_RighttempAngle = m_RightGuideAngle;
				
				break;
			case LEFT_DOWN_45:
				cout << "���ʾƷ�" << endl;
				pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 10.f, 0.0f);
				m_GuideAngle = 225.f;

					m_LeftGuideAngle = 195.f;
					m_RightGuideAngle = 255.f;
					m_LefttempAngle = m_LeftGuideAngle;
					m_RighttempAngle = m_RightGuideAngle;
				
				break;
			case RIGHT_DOWN_45:
				cout << "�����Ʒ�." << endl;
				pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
				m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 10.f, 0.0f);
				m_GuideAngle = 315.f;

					m_LeftGuideAngle = 345.f;
					m_RightGuideAngle = 285.f;
					m_LefttempAngle = m_LeftGuideAngle;
					m_RighttempAngle = m_RightGuideAngle;
				
				break;
			}
		}
	}
}

void CPlayerScript::AtkState()
{
	//������ �������.
	if ((m_CurState == THROW && !pAnimator->IsPlaying()) || (m_CurState == MEELE && !pAnimator->IsPlaying()))
	{
		//���ݹ����� �������ϰ��
		if (m_AtkDir == RIGHT_ATK)
			m_AtkDir = LEFT_ATK;
		//������ ���.
		else if (m_AtkDir == LEFT_ATK)
			m_AtkDir = RIGHT_ATK;
		//���Ÿ� ������ �������.
		if (m_CurState == THROW)
		{
			m_CurState = THROW_END;
			AttackBullet();
		}
		//���� ������ �������.
		if (m_CurState == MEELE)
		{
			m_CurState = IDLE;
		}
	}
	else if (m_CurState == THROW_END && !pAnimator->IsPlaying())
	{
		m_CurState = IDLE;
		m_bIsThrow = false;
	}


}

void CPlayerScript::AttackBullet()
{
	CGameObject* pBullet = CObjectMgr::GetInstance()->AddCopy(L"Small_Ball", L"my_Bullet");
	pBullet->GetComponent<CTransform>()->SetPosition(*pTransform->GetWorldPos());
	pBullet->SetObjectLayer(m_pGameObject->GetObjectLayer());

	pBullet->AddScripts(CBulletScript::Create(m_BulletAngle, 400.f, pBullet));
}

void CPlayerScript::MouseDir()
{
	//360���� ��ȯ�ϱ� ���Ѱ�
	if (m_BulletAngle < 0.0f)
		m_MouseAngle = m_BulletAngle + 360.f;
	else
		m_MouseAngle = m_BulletAngle;

	//���� 
	if (m_MouseAngle > 0.0f&&m_MouseAngle < 22.5f)
		m_CurDir = RIGHT;
	//���� ������
	else if (m_MouseAngle > 337.5f&&m_MouseAngle < 360.f)
		m_CurDir = RIGHT;
	//���� ���
	else if (m_MouseAngle > 22.5f&&m_MouseAngle < 67.5f)
		m_CurDir = RIGHT_UP_45;
	//���
	else if (m_MouseAngle > 67.5f&&m_MouseAngle < 112.5f)
		m_CurDir = UP;
	//���� ���
	else if (m_MouseAngle > 112.5f&&m_MouseAngle < 157.5f)
		m_CurDir = LEFT_UP_45;
	//����
	else if (m_MouseAngle > 157.5f&&m_MouseAngle < 202.5f)
		m_CurDir = LEFT;
	//���� �ϴ�
	else if (m_MouseAngle > 202.5f&&m_MouseAngle < 247.5f)
		m_CurDir = LEFT_DOWN_45;
	//�ϴ�
	else if (m_MouseAngle > 247.5f&&m_MouseAngle < 292.5f)
		m_CurDir = DOWN;
	//�����ϴ�
	else if (m_MouseAngle > 292.5f&&m_MouseAngle < 337.5f)
		m_CurDir = RIGHT_DOWN_45;
}

void CPlayerScript::CheckRange(D3DXVECTOR3 point,list<CGameObject*>* objlist)
{
	const vector<CGameObject*>& tiles = CObjectMgr::GetInstance()->GetTiles();

	int mapSizex = (CObjectMgr::GetInstance()->m_MapSizeX/2);
	int mapSizey = (CObjectMgr::GetInstance()->m_MapSizeY/2);

	int playerIndexX = (mapSizex + playerPos->x) / 16;
	int playerIndexY = (mapSizey + playerPos->y) / 16;
	if (!m_bIsCharged)
	{
		int pointIndexX = (mapSizex + point.x) / 16;
		int pointIndexY = (mapSizey + point.y) / 16;
		int pointGapX = playerIndexX - pointIndexX;
		int pointGapY = playerIndexY - pointIndexY;

		//�ε����� �����°��� �������� �ε����� �÷��̾� �ε��� ���� �������̹Ƿ�.(���� ��� �밢��)
		if (pointGapX > 0&& pointGapX>0)
		{
			for (int i = pointIndexX; i < playerIndexX; ++i)
			{
				for (int j = pointIndexY; j < playerIndexY; ++j)
				{
					int index = i + j*mapSizex;
					m_LeftPointCollide.push_back(tiles[index]);
				}
			}
		}
		//�ε���x�� ���� Y�� ����� ���� X�δ� �÷��̾�� ��������.Y�δ� �۴� (���� ��� �밢��)
		else if (pointGapX < 0 && pointGapX>0)
		{
			for (int i = playerIndexX; i <pointIndexX; ++i)
			{
				for (int j = pointIndexY; j < playerIndexY; ++j)
				{
					int index = i + j*mapSizex;
					m_LeftPointCollide.push_back(tiles[index]);
				}
			}
		}
		//�ε��� X�� ũ�� Y�� �������� �÷��̾�� ������ Y�� ũ��(���� �ϴ� �밢��)
		else if (pointGapX > 0 && pointGapX < 0) 
		{
			for (int i = pointIndexX; i < playerIndexX; ++i)
			{
				for (int j = playerIndexY; j <pointIndexY; ++j)
				{
					int index = i + j*mapSizex;
					m_LeftPointCollide.push_back(tiles[index]);
				}
			}
		}
		//�ε��� X�� ���� Y�� ������°��� �Ѵ� �÷��̾� �ε������� ������ (���� �ϴ� �밢��)
		else if (pointGapX < 0 && pointGapX < 0)
		{
			for (int i = playerIndexX; i < pointIndexX; ++i)
			{
				for (int j = playerIndexY; j < pointIndexY; ++j)
				{
					int index = i + j*mapSizex;
					m_LeftPointCollide.push_back(tiles[index]);
				}
			}
		}
		//x�ε����� ������ Y�ε����� ���� �÷��̾��  (�ϴ�)
		else if (pointGapX == 0 && pointGapX < 0)
		{
				for (int j = playerIndexY; j <pointIndexY; ++j)
				{
					int index = playerIndexX + j*mapSizex;
					m_LeftPointCollide.push_back(tiles[index]);
				}
		}
		// (���)
		else if (pointGapX == 0 && pointGapX > 0) 
		{
				for (int j = pointIndexY; j < playerIndexY; ++j)
				{
					int index = playerIndexX + j*mapSizex;
					m_LeftPointCollide.push_back(tiles[index]);
				}
		}
		//(����)
		else if (pointGapX > 0 && pointGapX == 0)
		{
			for (int i = pointIndexX; i < playerIndexX; ++i)
			{
					int index = i + playerIndexY*mapSizex;
					m_LeftPointCollide.push_back(tiles[index]);	
			}
		}
		//(����)
		else if (pointGapX < 0 && pointGapX == 0)
		{
			for (int i = playerIndexX; i < pointIndexX; ++i)
			{
					int index = i + playerIndexY*mapSizex;
					m_LeftPointCollide.push_back(tiles[index]);
			}
		}

	}
}

void CPlayerScript::CheckRangeCollide()
{
	if (!m_bIsCharged)
	{
		if (!m_LeftPointCollide.empty())
		{
			for (auto&i : m_LeftPointCollide)
			{
				if (i != nullptr && (m_pGameObject->GetObjectLayer() <= i->GetObjectLayer() || i->GetObjectLayer() == LAYER_GROUND))
				{
					CBoxCollider* pDestBoxCollider = i->GetComponent<CBoxCollider>();
					D3DXVECTOR3	  destPos = *i->GetComponent<CTransform>()->GetWorldPos();
					if (m_bIsDebug)
						i->DebugRender(true, false);
					if (pDestBoxCollider != nullptr)
					{
						COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
						//�簢���浹�� �ﰢ���浹�� �ִ�
						//�簢�� �浹��
						if (coltype == NORMAL)
						{
							if (CCollisionMgr::GetInstance()->CheckAABB(pBoxCollider, pDestBoxCollider))
							{
								cout << "�簢���浹" << endl;
								return true;
							}
						}
						//�ﰢ�� �浹��. �׿� �ﰢ�� �浹
						else if (coltype != NORMAL)
						{
							if (CCollisionMgr::GetInstance()->CheckLineBox(pBoxCollider, pDestBoxCollider))
								return true;
						}
					}
				}
			}
		}
		if (!m_RightPointCollide.empty())
		{

		}
	}
	else
	{
		if (!m_MidPointCollide.empty())
		{

		}
	}
}

void CPlayerScript::CheckTiles()
{
	const vector<CGameObject*>& tiles = CObjectMgr::GetInstance()->GetTiles();

	int mapSizex = CObjectMgr::GetInstance()->m_MapSizeX;
	int mapSizey = CObjectMgr::GetInstance()->m_MapSizeY;

	D3DXVECTOR3 pos = *pTransform->GetWorldPos();
	int indexX = (((mapSizex / 2) + playerPos->x) / 16);
	int indexY = (((mapSizey / 2) - playerPos->y) / 16);
	int index = indexX + indexY*mapSizex;

	m_NearTiles.push_back(tiles[(indexX - 1) + (indexY - 1)*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + (indexY - 1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX + 1) + (indexY - 1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX - 1) + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX + 1) + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX - 1) + (indexY + 1)*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + (indexY + 1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX + 1) + (indexY + 1)*mapSizex]);

}

bool CPlayerScript::CollideTiles()
{
	if (!m_bIsJump)
	{
		D3DXVECTOR3& playerPos = pTransform->GetLocalPosition();
		for (auto& i : m_NearTiles)
		{
			//�÷��̾�� �����Ʒ����ִ� ������Ʈ�ϰ��� �浹���� �ʴ´�. Layer1�̸� Layer1�̻����θ� �浹. Layer2�� �ö󰥽� 2�̻��� �͸�.
			//�������̾�Ǹ� ���̻� Y������ �� �ʿ䰡 ����.
			if (i != nullptr&&(m_pGameObject->GetObjectLayer() <= i->GetObjectLayer()||i->GetObjectLayer()==LAYER_GROUND))
			{
				CBoxCollider* pDestBoxCollider = i->GetComponent<CBoxCollider>();
				D3DXVECTOR3	  destPos = *i->GetComponent<CTransform>()->GetWorldPos();
				//Ÿ���ø��Ǵ� ������Ʈ�� Ȯ��. ( �浹ü �ִ³༮��)
				if (m_bIsDebug)
					i->DebugRender(true,false);
				if (pDestBoxCollider != nullptr)
				{
					COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
					//�簢���浹�� �ﰢ���浹�� �ִ�
					//�簢�� �浹��
					if (coltype == NORMAL)
					{
						if (CCollisionMgr::GetInstance()->CheckAABB(pBoxCollider, pDestBoxCollider))
						{
							cout << "�簢���浹" << endl;
							return true;
						}
					}
					//�ﰢ�� �浹��. �׿� �ﰢ�� �浹
					else if (coltype != NORMAL)
					{
						if (CCollisionMgr::GetInstance()->CheckLineBox(pBoxCollider, pDestBoxCollider))
							return true;
					}
				}
			}
		}
	}
	return false;
}

bool CPlayerScript::StepStair()
{
	if (!m_bIsJump)
	{
		const map<Layer, vector<CGameObject*>>& stairs = CObjectMgr::GetInstance()->GetStairs();
		Layer playerLayer = m_pGameObject->GetObjectLayer();

		for (auto& j : stairs)
		{
			for (auto&i : j.second)
			{
				//���� ��ܵ� �������̾�� �۵������ʴ´� ������ �ۿ��Ѵ�. �ٸ� �Ʒ� ���̾�� ���������ϹǷ� ������츸 �浹Ȯ������ �ʴ´�.
				int Uplayer = m_CurLayer + 1;
				Layer layer = Layer(Uplayer);
				if (i != nullptr)
				{
					CBoxCollider* pDestBoxCollider = i->GetComponent<CBoxCollider>();
					D3DXVECTOR3	  destPos = *i->GetComponent<CTransform>()->GetWorldPos();
					D3DXVECTOR3  jumpPos;
					if (pDestBoxCollider != nullptr)
					{
						int current;
						Layer currentLayer;
						switch (playerLayer)
						{
						case LAYER_1:
							//���� ���� �����̵Ǿ� �״ܰ�� �ö󰣴�.
							if (layer == i->GetObjectLayer())
							{
								if (CCollisionMgr::GetInstance()->CheckAABB(playerUpBox, pDestBoxCollider))
								{
									COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
									//�簢���浹�� �ﰢ���浹�� �ִ�
									//�簢�� �浹��
									if (coltype == NORMAL)
									{
										cout << " �� ���� �浹" << endl;
										current = m_pGameObject->GetObjectLayer() + 1;
										currentLayer = (Layer)current;
										m_pGameObject->SetObjectLayer(currentLayer);
										m_ChangeLayer = currentLayer;
										m_CurLayer = layer;
										JumpSetUp(*playerPos, destPos, 1.0f);
										return true;
									}
								}
							} 
							break;
						case LAYER_2:
							//���� ���� �����̵Ǿ� �״ܰ�� �ö󰣴�.
							if (layer == i->GetObjectLayer())
							{
								if (CCollisionMgr::GetInstance()->CheckAABB(playerUpBox, pDestBoxCollider))
								{
									COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
									//�簢���浹�� �ﰢ���浹�� �ִ�
									//�簢�� �浹��
									if (coltype == NORMAL)
									{
										cout << " �� ���� �浹" << endl;
										current = m_pGameObject->GetObjectLayer() + 1;
										currentLayer = (Layer)current;
										m_pGameObject->SetObjectLayer(currentLayer);
										m_ChangeLayer = currentLayer;
										m_CurLayer = layer;
										JumpSetUp(*playerPos, destPos, 1.0f);
										return true;
									}
								}
							} //�÷��̾�� �������̾��� ������ �������.
							else if (m_CurLayer > i->GetObjectLayer())
							{
								if (CCollisionMgr::GetInstance()->CheckAABB(playerDownBox, pDestBoxCollider))
								{
									COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
									//�簢���浹�� �ﰢ���浹�� �ִ�
									//�簢�� �浹��
									if (coltype == NORMAL)
									{
										cout << " �Ʒ� ���� �浹" << endl;
										if (i->GetObjectLayer() == LAYER_0)
										{
											current = m_pGameObject->GetObjectLayer() - 1;
											if (m_CurDir == DOWN)
												destPos.y -= 20.f;
											else
												destPos.y -= 20.f;
										}
										m_pGameObject->SetObjectLayer(LAYER_3);
										currentLayer = (Layer)current;		
										m_ChangeLayer = currentLayer;
										m_CurLayer = (Layer)(current - 1);
										JumpSetUp(*playerPos, destPos, 1.0f);
										return true;
									}
								}
							}
							break;
						case LAYER_3:
							//�÷��̾�� �������̾��� ������ �������.
						if (m_CurLayer > i->GetObjectLayer())
							{
								if (CCollisionMgr::GetInstance()->CheckAABB(playerDownBox, pDestBoxCollider))
								{
									COLLIDE_TYPE coltype = pDestBoxCollider->GetCollideType();
									//�簢���浹�� �ﰢ���浹�� �ִ�
									//�簢�� �浹��
									if (coltype == NORMAL)
									{
										cout << " �Ʒ� ���� �浹" << endl;
										if (i->GetObjectLayer() == LAYER_0)
										{
											current = m_pGameObject->GetObjectLayer() - 2;
											if(m_CurDir == DOWN)
												destPos.y -= 40.f;
											else
												destPos.y -= 20.f;
										}	
										else
										{
											current = m_pGameObject->GetObjectLayer() - 1;
											if (m_CurDir == DOWN)
												destPos.y -= 20.f;
											else
												destPos.y -= 20.f;
										}
											
										currentLayer = (Layer)current;
										m_ChangeLayer = currentLayer;
										m_CurLayer = (Layer)(current - 1);
										JumpSetUp(*playerPos, destPos, 1.0f);
										return true;
									}
								}
							}
							break;
						}
		
					}
				}
			}
		}
	}
	return false;
}
void CPlayerScript::Jump()
{

	if (m_fJumpTime <= m_fJumpEndTime)
	{
		m_CurState = JUMP;
		DIR dir = m_CurDir;
		D3DXVECTOR3 newPos = BezierCurve(m_JumpStartPos, m_JumpEndPos, m_JumpControlPos, (m_fJumpTime / m_fJumpEndTime));
		newPos.y += 16.f;
		pTransform->SetPosition(newPos);
	}
	else
	{
		m_pGameObject->SetObjectLayer(m_ChangeLayer);
		m_bIsJump = false;
		m_fJumpTime -= m_fJumpTime;
		cout << "������" << endl;
		m_CurState = IDLE;
	}
	m_fJumpTime += CTimeMgr::GetInstance()->GetDeltaTime();

}

void CPlayerScript::JumpSetUp(const D3DXVECTOR3 & startPos, const D3DXVECTOR3 & endPos, const float & endTime)
{
	switch (m_CurDir)
	{
	case UP:
		m_JumpControlPos = D3DXVECTOR3(playerPos->x, playerPos->y, 0.0f);
		break;
	case DOWN:
		m_JumpControlPos = D3DXVECTOR3(playerPos->x, playerPos->y-20.f, 0.0f);
		break;
	case LEFT_UP_45:
		m_JumpControlPos = D3DXVECTOR3(playerPos->x - 10.f, playerPos->y + 40.f, 0.0f);
		break;
	case RIGHT_UP_45:
		m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 40.f, 0.0f);
		break;
	case LEFT:
		m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 20.f, 0.0f);
		break;
	case RIGHT:
		pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 20.f, 0.0f);
		break;
	case LEFT_DOWN_45:
		m_JumpControlPos = D3DXVECTOR3((playerPos->x - 10.f), playerPos->y + 10.f, 0.0f);
		break;
	case RIGHT_DOWN_45:
		m_JumpControlPos = D3DXVECTOR3((playerPos->x + 10.f), playerPos->y + 10.f, 0.0f);
		break;
	}
	m_JumpStartPos = startPos;
	m_JumpEndPos = endPos;
	m_fJumpEndTime = endTime;
	m_bIsJump = true;
}

void CPlayerScript::CheckLayer()
{
	if (m_CurLayer != m_PreLayer)
	{
		switch (m_CurLayer)
		{
		case LAYER_0:
			cout << "1��" << endl;
			playerUpBox = pBoxCollider;
			playerDownBox = pBoxCollider;
			break;
		case LAYER_1:
			cout << "1.5��" << endl;
			playerUpBox = pBoxCollider;
			playerDownBox = m_playerFoot;
			break;
		case LAYER_2:
			cout << "2��" << endl;
			playerUpBox = m_playerFoot;
			playerDownBox = m_playerFoot;
			break;
		case LAYER_3:
			cout << "2.5��" << endl;
			break;
		case LAYER_4:
			cout << "3��" << endl;
			break;
		case LAYER_5:
			cout << "���� �ö����ȵ�" << endl;
			break;
		}
		m_PreLayer = m_CurLayer;
	}
}

void CPlayerScript::DrawGuideLine()
{
	D3DXMATRIX*	mat = CCameraMgr::GetInstance()->GetViewProjMatrix();

	m_GuideLineEndPoint = D3DXVECTOR3(playerPos->x + m_GuideRange*cosf(D3DXToRadian(m_GuideAngle)), playerPos->y + m_GuideRange*sinf(D3DXToRadian(m_GuideAngle)), 0.0f);
	
	m_GuideLineLeftEndPoint = D3DXVECTOR3(playerPos->x + m_GuideRange*cosf(D3DXToRadian(m_LeftGuideAngle)), playerPos->y + m_GuideRange*sinf(D3DXToRadian(m_LeftGuideAngle)), 0.0f);
	m_GuideLineRightEndPoint = D3DXVECTOR3(playerPos->x + m_GuideRange*cosf(D3DXToRadian(m_RightGuideAngle)), playerPos->y + m_GuideRange*sinf(D3DXToRadian(m_RightGuideAngle)), 0.0f);
	CDeviceMgr::GetInstance()->GetLine()->SetWidth(2.f);
	CDeviceMgr::GetInstance()->GetLine()->Begin();
	{
		D3DXVECTOR3 m_Line[2];
		m_Line[0] = *playerPos;
		m_Line[1] = m_GuideLineEndPoint;
		CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(0,0,255));
		if (!m_bIsCharged)
		{
			m_Line[0] = *playerPos;
			m_Line[1] = m_GuideLineLeftEndPoint;
			CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(255, 0, 255));

			m_Line[0] = *playerPos;
			m_Line[1] = m_GuideLineRightEndPoint;
			CDeviceMgr::GetInstance()->GetLine()->DrawTransform(m_Line, 2, mat, D3DCOLOR_XRGB(255, 0, 255));

		}		
	}
	CDeviceMgr::GetInstance()->GetLine()->End();
}
