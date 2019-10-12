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
		MessageBox(0,L"�ִϸ����� ����", L"Error",0);
	
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
	
}

void CPlayerScript::OnLateUpdate()
{
}

void CPlayerScript::OnRender()
{
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
	m_Right = false;
	m_Left = false;
	m_Down = false;
	m_Up = false;
}

void CPlayerScript::MouseInput()
{
	if (pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		cout << "������!" << endl;
	
		D3DXVECTOR3 mousePos = pKeyMgr->GetMouse()->GetMousePos();
		

		m_BulletAngle = GetAngle(*playerPos, mousePos);

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

		m_CurState = THROW;
		m_bIsThrow = true;
	}
}

void CPlayerScript::MeeleAttack()
{
	if (pKeyMgr->GetInstance()->KeyDown(KEY_V))
	{
		m_CurState = MEELE;
	}
}

void CPlayerScript::Moving()
{
	float m_fSpeed=0.5f;
	float m_moveX = 0.0f;
	float m_moveY = 0.0f;
	if (m_CurState == THROW|| m_CurState == THROW_END)
	{
		m_fSpeed = 0.2f;
	}
	switch (m_CurMoveDir)
	{
	case UP:
		m_moveY = m_fSpeed*m_fVelocity;
		break;
	case DOWN:
		m_moveY = -1*(m_fSpeed*m_fVelocity);
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
	if (CollideTiles())
	{
		*playerPos = m_PrePos;
		m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
		cout << "x�浹��" << endl;
	}
	m_PrePos = *playerPos;
	playerPos->y += m_moveY;
	m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
	if (CollideTiles())
	{
		*playerPos = m_PrePos;
		m_pGameObject->GetComponent<CBoxCollider>()->SetBoxCollider();
		cout << "y�浹��" << endl;
	}
	

}

void CPlayerScript::AnimState()
{
	if (m_CurState != m_PreState||m_CurDir != m_PreDir)
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
		}
		m_PreState = m_CurState;
		m_PreDir = m_CurDir;
	}


}

void CPlayerScript::DirState()
{
	if (m_CurDir != m_PreDir)
	{
		switch (m_CurDir)
		{
		case UP:
			cout << "������" << endl;
			break;
		case DOWN:
			cout << "�Ʒ�����" << endl;
			break;
		case LEFT_UP_45:
			cout << "������" << endl;
			pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT_UP_45:
			cout << "������" << endl;
			pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		case LEFT:
			cout << "����" << endl;
			pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT:
			cout << "������" << endl;
			pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		case LEFT_DOWN_45:
			cout << "���ʾƷ�" << endl;
			pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
			break;
		case RIGHT_DOWN_45:
			cout << "�����Ʒ�." << endl;	
			pTransform->SetScaling(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			break;
		}
	}
}

void CPlayerScript::AtkState()
{
	//������ �������.
	if ((m_CurState == THROW && !pAnimator->IsPlaying())|| (m_CurState == MEELE && !pAnimator->IsPlaying()))
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

	pBullet->AddScripts(CBulletScript::Create(m_BulletAngle, 400.f, pBullet));
}

void CPlayerScript::CheckTiles()
{
	const vector<CGameObject*>& tiles = CObjectMgr::GetInstance()->GetTiles();

	int mapSizex = CObjectMgr::GetInstance()->m_MapSizeX;
	int mapSizey = CObjectMgr::GetInstance()->m_MapSizeY;

	D3DXVECTOR3 pos = *pTransform->GetWorldPos();
	int indexX = ((( mapSizex / 2)+ playerPos->x)/ 16);
	int indexY = ((( mapSizey / 2)- playerPos->y)/ 16);
	int index = indexX + indexY*mapSizex;

	m_NearTiles.push_back(tiles[(indexX-1)+ (indexY-1)*mapSizex]);
	m_NearTiles.push_back(tiles[ indexX + (indexY-1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX+1) + (indexY-1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX-1) + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX+1) + indexY*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX-1) + (indexY+1)*mapSizex]);
	m_NearTiles.push_back(tiles[indexX + (indexY+1)*mapSizex]);
	m_NearTiles.push_back(tiles[(indexX+1) + (indexY+1)*mapSizex]);

}

bool CPlayerScript::CollideTiles()
{
	D3DXVECTOR3& playerPos = pTransform->GetLocalPosition();
	for (auto& i : m_NearTiles)
	{
		if (i != nullptr)
		{
			CBoxCollider* pDestBoxCollider = i->GetComponent<CBoxCollider>();
			D3DXVECTOR3	  destPos = *i->GetComponent<CTransform>()->GetWorldPos();
			//Ÿ���ø��Ǵ� ������Ʈ�� Ȯ��. ( �浹ü �ִ³༮��)
			i->SetObjectCliked(true, D3DCOLOR_XRGB(255, 0, 0));
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
				else if(coltype!=NORMAL)
				{
					if (CCollisionMgr::GetInstance()->CheckLineBox(pBoxCollider, pDestBoxCollider))
						return true;					
				}		
			}
		}
	}
	return false;
}

