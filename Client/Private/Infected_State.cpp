#include "SMath.h"
#include "stdafx.h"
#include "GameInstance.h"
#include "Infected_State.h"
#include "Data_Manager.h"
#include "Body_Infected.h"	

#include "Infected_Idle.h"
#include "Infected_IdlePose.h"
#include "Infected_IdleAct_01.h"
#include "Infected_IdleAct_02.h"
#include "Infected_IdleAct_03.h"
#include "Infected_RelaxedIdleAct_01.h"
#include "Infected_RelaxedIdleAct_02.h"
#include "Infected_RelaxedIdleAct_03.h"

#include "Infected_Walk_B.h"
#include "Infected_Walk_F.h"
#include "Infected_Walk_FL.h"
#include "Infected_Walk_FL45.h"
#include "Infected_Walk_FR.h"
#include "Infected_Walk_FR45.h"

#include "Infected_Run_B.h"
#include "Infected_Run_F.h"
#include "Infected_Run_FL.h"
#include "Infected_Run_FL45.h"
#include "Infected_Run_FR.h"
#include "Infected_Run_FR45.h"

#include "Infected_Melee_RD_01.h"
#include "Infected_Melee_RM_01.h"
#include "Infected_Melee_RM_02.h"
#include "Infected_Melee_RU_01.h"
#include "Infected_Melee_RU_02.h"

#include "Infected_Dodge_F_01_TEMP.h"
#include "Infected_Dodge_L_01_TEMP.h"
#include "Infected_Dodge_R_01_TEMP.h"

#include "Infected_MeleeDynamic_RU_01.h"
#include "Infected_MeleeDynamic_RU_02.h"
#include "Infected_MeleeDynamic_RU_03.h"

void CInfected_State::Initialize(CInfected* pActor)
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);
}

CState<CInfected>* CInfected_State::Update(CInfected* pActor, _float fTimeDelta)
{
	return nullptr;
}

void CInfected_State::Release(CInfected* pActor)
{
	Safe_Release(m_pGameInstance);
}

CState<CInfected>* CInfected_State::Normal_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	CState<CInfected>* pState = { nullptr };

	pState = Normal(pActor, fTimeDelta, _iAnimIndex);
	if (pState)	return pState;

	return nullptr;
}

CState<CInfected>* CInfected_State::Walk_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	CState<CInfected>* pState = { nullptr };

	pState = Normal(pActor, fTimeDelta, _iAnimIndex);
	if (pState)	return pState;

	return nullptr;
}

CState<CInfected>* CInfected_State::Run_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	CState<CInfected>* pState = { nullptr };

	pState = Run(pActor, fTimeDelta, _iAnimIndex);
	if (pState)	return pState;

	return nullptr;
}

CState<CInfected>* CInfected_State::Attack_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	CState<CInfected>* pState = { nullptr };

	pState = Attack(pActor, fTimeDelta, _iAnimIndex);
	if (pState)	return pState;


	return nullptr;
}

CState<CInfected>* CInfected_State::Hit_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{ 
	if (pActor->Is_Animation_End())
	{
		return new CInfected_IdlePose();
	}

	return nullptr;
}

CState<CInfected>* CInfected_State::Knock_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	if (pActor->Is_Animation_End())
	{
		return Normal_State(pActor, fTimeDelta, _iAnimIndex);
	}

	return nullptr;
}

CState<CInfected>* CInfected_State::Dodge_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	if (pActor->Is_Animation_End())
	{

		return Normal_State(pActor, fTimeDelta, _iAnimIndex);
	}

	return nullptr;
}

CState<CInfected>* CInfected_State::Spawn_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	/* ���� Init���� ������ Spawn Animation�� ������ �����ϴ°� */
	if (pActor->Is_Animation_End()) 
	{
		_int iRandom = SMath::Random(1, 6);

		switch (iRandom)
		{
		case 1:
			return new CInfected_RelaxedIdleAct_03();
			break;
		case 2:
			return new CInfected_IdleAct_01();
			break;
		case 3:
			return new CInfected_IdleAct_02();
			break;
		case 4:
			return new CInfected_IdleAct_03();
			break;
		case 5:
			return new CInfected_RelaxedIdleAct_01();
			break;
		case 6:
			return new CInfected_RelaxedIdleAct_02();
			break;
		default:
			return new CInfected_IdleAct_01();
			break;
		}
	}

	return nullptr;
}

CState<CInfected>* CInfected_State::Death_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	if (pActor->Is_Animation_End())
	{
		if (false == m_bFlags[0] )
		{
			CBody_Infected* pBody = dynamic_cast<CBody_Infected*>(pActor->Get_Body());
			pBody->Collider_Off(); // �ٵ� �ݶ��̴� off 

			CData_Manager::GetInstance()->Add_CurEXP(15); // �÷��̾� 15 ����ġ ���� 
			m_bFlags[0] = true;
		}	

		pActor->Set_Dead(true);
		return nullptr;
	}

	return nullptr;
}

CState<CInfected>* CInfected_State::Stun_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return nullptr;
}

CState<CInfected>* CInfected_State::Finisher_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return nullptr;
}

/* �߾����� */
CState<CInfected>* CInfected_State::Normal(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	_float WalkDistance = pActor->Get_Info().fWalk_Distance;	 // 10.f
	_float AttackDistance = pActor->Get_Info().fAttack_Distance; // 3.5f

	CState<CInfected>* pState = { nullptr };

	pState = Dodge(pActor, fTimeDelta, _iAnimIndex);
	if (pState)	return pState;

	_float fDist = pActor->Calc_Distance(m_pGameInstance->Get_Player());	

	// 0 ~ Attack ~ Walk  
	if (fDist > WalkDistance)
	{
		pState = Run(pActor, fTimeDelta, _iAnimIndex);
		if (pState)	return pState;
	}
	else if (AttackDistance < fDist && fDist <= WalkDistance)
	{
		pState = Walk(pActor, fTimeDelta, _iAnimIndex);
		if (pState)	return pState;
	}
	else 
	{
		pState = Attack(pActor, fTimeDelta, _iAnimIndex);
		if (pState)	return pState;
	}
	

	if (pActor->Is_Animation_End())
	{
		return new CInfected_Idle();
	}

	return nullptr; /* ������ ���� */
}

CState<CInfected>* CInfected_State::Walk(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	_float fDist = pActor->Calc_Distance(m_pGameInstance->Get_Player());

	CInfected::INFECTED_TYPE Type = pActor->Get_Info().Get_Type();

	switch (Type)
	{
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_A:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_B:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_C:
		return new CInfected_Walk_F();
		break;

	case Client::CInfected::INFECTED_TYPE::INFECTED_PROTEUS:
		break;
	case Client::CInfected::INFECTED_TYPE::INFECTED_WASTER:
		break;
	}

	return nullptr;
}

CState<CInfected>* CInfected_State::Run(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	_float fDist = pActor->Calc_Distance(m_pGameInstance->Get_Player());

	CInfected::INFECTED_TYPE Type = pActor->Get_Info().Get_Type();

	switch (Type)
	{
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_A:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_B:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_C:
			return new CInfected_Run_F();
		break;
	case Client::CInfected::INFECTED_TYPE::INFECTED_PROTEUS:
		break;
	case Client::CInfected::INFECTED_TYPE::INFECTED_WASTER:
		break;
	}

	return nullptr;
}

CState<CInfected>* CInfected_State::Attack(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	/* ���� �÷��̾���� �Ÿ� */
	_float fDist = pActor->Calc_Distance(m_pGameInstance->Get_Player());

	/* ������ Info Desc */
	CInfected::INFECTED_DESC Info = pActor->Get_Info();
	_int iActNumber = Info.RandomNumber % 4;
	pActor->Look_At_Target();

	/* Ÿ�Ժ�, �Ÿ��� �����ؾ��� */
	switch (Info.eType)
	{
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_A:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_B:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_C:

		if (0.f <= fDist && fDist < Info.fAttack_Distance - 1.5f) // 0 ~ ���ݻ�Ÿ� - 0.5
		{
			switch (iActNumber)
			{
			case 1:
				return new CInfected_Melee_RD_01(); /* ���ڸ� ���� */
				break;
			case 2:
				return new CInfected_Melee_RM_01();
				break;
			case 3:
				return new CInfected_Melee_RM_02();
				break;
			default:
				return new CInfected_Melee_RU_02();
				break;
			}
		}
		else if (Info.fAttack_Distance - 2.f <= fDist && fDist <= Info.fAttack_Distance) // ���ݻ�Ÿ� - 0.5 ~ ���ݻ�Ÿ� + 0.5 
		{
			switch (iActNumber)
			{
			case 1:
				return new CInfected_MeleeDynamic_RU_01(); /* �ɾ�鼭 ���� */
				break;
			case 2:
				//return new CInfected_MeleeDynamic_RU_01();
				return new CInfected_MeleeDynamic_RU_02();
				break;
			case 3:
				//return new CInfected_MeleeDynamic_RU_01();
				return new CInfected_MeleeDynamic_RU_03();
				break;
			default:
				return new CInfected_MeleeDynamic_RU_01();
				break;
			}
		}
		break;

	case Client::CInfected::INFECTED_TYPE::INFECTED_PROTEUS:
		break;
	case Client::CInfected::INFECTED_TYPE::INFECTED_WASTER:
		break;
	}

	///* ���� ������ �ڷ� ������������ */
	//if (pActor->Is_Animation_End())
	//{
	//	return new CInfected_Idle();
	//}

	return nullptr;
}


CState<CInfected>* CInfected_State::Dodge(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	if (CInfected::INFECTED_TYPE::INFECTED_PROTEUS == pActor->Get_Info().Get_Type())
	{
		return nullptr; /* Ŭ���� ���ô� ���� ���ڷ��� */
	}
	else
		return nullptr;

	//if (pActor->Is_Animation_End())
	//{
	//	return new CInfected_Idle();
	//}

	return nullptr;
}

CState<CInfected>* CInfected_State::Patrol(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return nullptr;
}

//if (fDist >= pActor->Get_Info().fWalk_Distance)
//{
//	pState = Run(pActor, fTimeDelta, _iAnimIndex);
//	if (pState)	return pState;
//}
//else if (pActor->Get_Info().fAttack_Distance <= fDist && fDist < pActor->Get_Info().fWalk_Distance)
//{
//	pState = Walk(pActor, fTimeDelta, _iAnimIndex);
//	if (pState)	return pState;
//}
//else if (0 <= fDist && fDist < pActor->Get_Info().fAttack_Distance)
//{
//	pState = Attack(pActor, fTimeDelta, _iAnimIndex);
//	if (pState)	return pState;
//}
