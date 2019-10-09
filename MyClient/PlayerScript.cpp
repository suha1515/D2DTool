#include "stdafx.h"
#include "PlayerScript.h"
#include "GameObject.h"
#include "Transform.h"
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
}

void CPlayerScript::OnEnable()
{
}

void CPlayerScript::OnCollision(CGameObject * pGameObject, XMFLOAT2* move)
{
	if (nullptr != pGameObject)
	{
		//CTransform* pTransform = m_pGameObject->GetComponent<CTransform>();
		//wstring pname = pGameObject->GetObjectName();
		//wstring name = m_pGameObject->GetObjectName();

		//wstring text = name + L" ������Ʈ�� " + pname + L" ������Ʈ�� �浹���̴�";
		////string text2;
		////text2.assign(text.begin(), text.end());
		//wcout << text << endl;
		//cout << "�浹��" << endl;
		////�о��
		//D3DXVECTOR3 playerPos = pTransform->GetPosition();
		//D3DXVECTOR3 destPos =   pGameObject->GetComponent<CTransform>()->GetPosition();
		////�İ�� ���̰� ª�������� �о��
		//if (move->x > move->y)
		//{
		//	//y�� �о��		
		//	if (playerPos.y > destPos.y)
		//		pTransform->SetPosition(D3DXVECTOR3(playerPos.x,playerPos.y + move->y,0.0f));
		//	else
		//		pTransform->SetPosition(D3DXVECTOR3(playerPos.x, playerPos.y- move->y, 0.0f));
		//}
		//else
		//	//x�� �о��
		//{
		//	if (playerPos.x > destPos.x)
		//		pTransform->SetPosition(D3DXVECTOR3(playerPos.x+ move->x, playerPos.y, 0.0f));
		//	else
		//		pTransform->SetPosition(D3DXVECTOR3(playerPos.x- move->x, playerPos.y , 0.0f));
		//}
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
		//����Ű �Է�
		MoveInput();
		//���콺 �Է�
		MouseInput();	
		//���� �ִϸ��̼� ���º�ȭ
		AtkState();
		//���� ��ȭ.
		DirState();
		//�̵� �ִϸ��̼� ���º�ȭ
		AnimState();
		

		/*switch (m_CurState)
		{
		case IDLE:
			cout << "���" << endl;
			break;
		case RUN_START:
			cout << "�ٱ� ����" << endl;
			break;
		case RUN_END:
			cout << "�ٱ� ��" << endl;
			break;
		case RUN:
			cout << "�� ��" << endl;
			break;
		}*/

	}

	m_Right = false;
	m_Left = false;
	m_Down = false;
	m_Up = false;

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
	if (pKeyMgr->KeyPressing(KEY_LEFT) || pKeyMgr->KeyPressing(KEY_RIGHT)
		|| pKeyMgr->KeyPressing(KEY_UP) || pKeyMgr->KeyPressing(KEY_DOWN))
	{
		//cout << "Ű������" << endl;
		m_fVelocity += (6.0f*powf(m_fAcc, 2.0f));						//���ӵ� ���� ������ �ʿ䰡����
		m_fAcc += CTimeMgr::GetInstance()->GetDeltaTime();

		m_fAcc = __min(2.0f, m_fAcc);
		m_fVelocity = __min(7.f, m_fVelocity);
		if (m_CurState == IDLE || m_CurState == RUN_END)
			m_CurState = RUN_START;
		else if (m_CurState == RUN_START && !pAnimator->IsPlaying())
			m_CurState = RUN;
	}
	//���� Ű�� ����� (����Ű) ���!
	else if (!pKeyMgr->KeyPressing(KEY_LEFT) && !pKeyMgr->KeyPressing(KEY_RIGHT)
		&& !pKeyMgr->KeyPressing(KEY_UP) && !pKeyMgr->KeyPressing(KEY_DOWN))
	{
			m_fVelocity -= (2.0f*powf(m_fAcc, 2.0f));
			m_fAcc -= CTimeMgr::GetInstance()->GetDeltaTime()*2.0f;

			m_fAcc = __max(0.0f, m_fAcc);
			m_fVelocity = __max(0.f, m_fVelocity);

		if (m_CurState == RUN || m_CurState == RUN_START)
			m_CurState = RUN_END;
		else if (m_CurState == RUN_END && !pAnimator->IsPlaying())
			m_CurState = IDLE;
	}
	if (pKeyMgr->KeyPressing(KEY_LEFT))
	{
		//cout << "���� �̵�!" << endl;
		D3DXVECTOR3& playerPos = pTransform->GetPosition();
		playerPos.x -= 0.5f*m_fVelocity;
		m_Left = true;
	}
	else if (pKeyMgr->KeyPressing(KEY_RIGHT))
	{
		D3DXVECTOR3& playerPos = pTransform->GetPosition();
		playerPos.x += 0.5f*m_fVelocity;
		m_Right = true;
	}
	if (pKeyMgr->KeyPressing(KEY_UP))
	{
		D3DXVECTOR3& playerPos = pTransform->GetPosition();
		playerPos.y += 0.5f*m_fVelocity;
		m_Up = true;
	}
	else if (pKeyMgr->KeyPressing(KEY_DOWN))
	{
		D3DXVECTOR3& playerPos = pTransform->GetPosition();
		playerPos.y -= 0.5f*m_fVelocity;
		m_Down = true;
	}

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
}

void CPlayerScript::MouseInput()
{
	if (pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		cout << "������!" << endl;
		//������ ������ ���� �̰ų� �ִϸ��̼��� �۵����� ������.
		//if(m_CurState==THROW_END||!pAnimator->IsPlaying())
		m_CurState = THROW;
	}
}

void CPlayerScript::AnimState()
{
	if (m_CurState != m_PreState||m_CurDir!=m_PreDir)
	{
		switch (m_CurState)
		{
		case IDLE:
			cout << "������" << endl;
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
			cout << "�ٱ� �غ�" << endl;
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
			cout << "�ٱ�" << endl;
			switch (m_CurDir)
			{
			case UP:
				pAnimator->Play(L"Player_Run_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case DOWN:
				pAnimator->Play(L"Player_Run_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_UP_45:
			case RIGHT_UP_45:
				pAnimator->Play(L"Player_Run_Right_Up", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT:
			case RIGHT:
				pAnimator->Play(L"Player_Run_Right", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			case LEFT_DOWN_45:
			case RIGHT_DOWN_45:
				pAnimator->Play(L"Player_Run_Right_Down", ANIMATION_TYPE::ANIMATION_LOOP);
				break;
			}
			break;
		case  RUN_END:
			cout << "�ٱ� ����" << endl;
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
			cout << "���Ÿ� ����" << endl;
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
			cout << "���Ÿ� ���� �ߴ�." << endl;
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
		}
		m_PreState = m_CurState;
		m_PreDir = m_CurDir;
	}


}

void CPlayerScript::DirState()
{
	if (m_Right)
	{
		m_CurDir = RIGHT;
		pTransform->SetScaling(D3DXVECTOR3(1.0f,1.0f,1.0f));
	}
	else if (m_Left)
	{
		m_CurDir = LEFT;
		pTransform->SetScaling(D3DXVECTOR3(-1.0f, 1.0f, 1.0f));
	}
		

	if (m_Up)
		m_CurDir = UP;
	else if (m_Down)
		m_CurDir = DOWN;

	if (m_Right&&m_Up)
		m_CurDir = RIGHT_UP_45;
	else if (m_Right&&m_Down)
		m_CurDir = RIGHT_DOWN_45;
	else if (m_Left&&m_Down)
		m_CurDir = LEFT_DOWN_45;
	else if (m_Left&&m_Up)
		m_CurDir = LEFT_UP_45;

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
			break;
		case RIGHT_UP_45:
			cout << "������" << endl;
			break;
		case LEFT:
			cout << "����" << endl;
			break;
		case RIGHT:
			cout << "������" << endl;
			break;
		case LEFT_DOWN_45:
			cout << "���ʾƷ�" << endl;
			break;
		case RIGHT_DOWN_45:
			cout << "�����Ʒ�." << endl;
			break;
		}
		
	}


}

void CPlayerScript::AtkState()
{
	//������ �������.
	if (m_CurState == THROW && !pAnimator->IsPlaying())
	{
		//���ݹ����� �������ϰ��
		if (m_AtkDir == RIGHT_ATK)
			m_AtkDir = LEFT_ATK;
		//������ ���.
		else if (m_AtkDir == LEFT_ATK)
			m_AtkDir = RIGHT_ATK;

		m_CurState = THROW_END;
		AttackBullet();
	}
	else if (m_CurState == THROW_END && !pAnimator->IsPlaying())
	{
		m_CurState = IDLE;
	}
}

void CPlayerScript::AttackBullet()
{
	CGameObject* pBullet = CObjectMgr::GetInstance()->AddCopy(L"Big_Bullet", L"my_Bullet");
	pBullet->GetComponent<CTransform>()->SetPosition(pTransform->GetPosition());

	D3DXVECTOR3 mousePos = pKeyMgr->GetMouse()->GetMousePos();
	D3DXVECTOR3& playerPos = pTransform->GetPosition();

	float angle = GetAngle(playerPos, mousePos);
	
	pBullet->AddScripts(CBulletScript::Create(angle, 400.f, pBullet));
	cout << "�Ѿ˻���!" << endl;
}

