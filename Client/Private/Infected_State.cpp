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

	pState = Normal(pActor, fTimeDelta, _iAnimIndex);
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
	//return Normal_State(pActor, fTimeDelta, _iAnimIndex);
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
		//cout << "Dodge_State end " << endl;

		return Normal_State(pActor, fTimeDelta, _iAnimIndex);
	}

	return nullptr;
}

CState<CInfected>* CInfected_State::Spawn_State(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	/* 몬스터 Init에서 셋팅한 Spawn Animation이 끝나면 도달하는곳 */
	if (pActor->Is_Animation_End()) 
	{
		//cout << "spawn end " << endl;

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
			cout << "Infected Dead " << endl;
			CBody_Infected* pBody = dynamic_cast<CBody_Infected*>(pActor->Get_Body());
			pBody->Collider_Off();

			CData_Manager::GetInstance()->Add_CurEXP(15);
			m_bFlags[0] = true;
		}	

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

/* 중앙제어 */
CState<CInfected>* CInfected_State::Normal(CInfected* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	CState<CInfected>* pState = { nullptr };

	pState = Dodge(pActor, fTimeDelta, _iAnimIndex);
	if (pState)	return pState;


	_float fDist = pActor->Calc_Distance(m_pGameInstance->Get_Player());
	
	cout << "Dist : " << fDist << endl;
	
	// 0 ~ Attack ~ Walk  
	if (fDist > 10.f)
	{
		pState = Run(pActor, fTimeDelta, _iAnimIndex);
		if (pState)	return pState;
	}
	else if (3.5f < fDist && fDist <= 10.f)
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

	return nullptr; /* 현상태 유지 */
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
			cout << "CInfected_Run_F " << endl;
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
	/* 현재 플레이어와의 거리 */
	_float fDist = pActor->Calc_Distance(m_pGameInstance->Get_Player());

	/* 몬스터의 Info Desc */
	CInfected::INFECTED_DESC Info = pActor->Get_Info();
	_int iActNumber = Info.RandomNumber % 4;
	pActor->Look_At_Target();

	/* 타입별, 거리별 조절해야함 */
	switch (Info.eType)
	{
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_A:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_B:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_C:

		if (0.f <= fDist && fDist < 1.f) // 0 ~ 공격사거리 - 0.5
		{
			switch (iActNumber)
			{
			case 1:
				return new CInfected_Melee_RD_01(); /* 제자리 공격 */
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
		else if ( 1.f <= fDist && fDist <= 3.5f ) // 공격사거리 - 0.5 ~ 공격사거리 + 0.5 
		{
			switch (iActNumber)
			{
			case 1:
				return new CInfected_MeleeDynamic_RU_01(); /* 걸어가면서 공격 */
				break;
			case 2:
				return new CInfected_MeleeDynamic_RU_02();
				break;
			case 3:
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

	///* 공격 끝나면 뒤로 물러나가야함 */
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
		return nullptr; /* 클래스 갑시다 ㅇㅋ 난자러감 */
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
