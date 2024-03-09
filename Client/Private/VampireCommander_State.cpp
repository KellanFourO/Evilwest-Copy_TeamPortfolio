#include "VampireCommander_State.h"
#include "GameInstance.h"

#include "VampireCommander_Idle.h"
#include "VampireCommander_BloodRange_Loop.h"
#include "VampireCommander_BloodRange_Start.h"
#include "VampireCommander_BloodRange_Stop.h"
#include "VampireCommander_BloodRange_Stun_Start.h"
#include "VampireCommander_Dead.h"
#include "VampireCommander_HitCenter.h"
#include "VampireCommander_HitLeft.h"
#include "VampireCommander_HitRight.h"
#include "VampireCommander_Leap_Loop.h"
#include "VampireCommander_Leap_Stop.h"
#include "VampireCommander_Leap_Strat.h"
#include "VampireCommander_Melee1.h"
#include "VampireCommander_Melee2.h"
#include "VampireCommander_Ranged1.h"
#include "VampireCommander_Ranged2.h"
#include "VampireCommander_Ranged3.h"
#include "VampireCommander_SyncedAttack.h"
#include "VampireCommander_SyncedAttack_Fail.h"
#include "VampireCommander_Taunt1.h"
#include "VampireCommander_Taunt2.h"
#include "VampireCommander_Taunt3.h"
#include "VampireCommander_Taunt4.h"
#include "VampireCommander_Taunt5.h"
#include "VampireCommander_Walk_F.h"

#include "SMath.h"
void CVampireCommander_State::Initialize(CVampireCommander* pActor)
{
	m_pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(m_pGameInstance);
}

CState<CVampireCommander>* CVampireCommander_State::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	return nullptr;
}

void CVampireCommander_State::Release(CVampireCommander* pActor)
{
	Safe_Release(m_pGameInstance);
}

CState<CVampireCommander>* CVampireCommander_State::Update_State(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return nullptr;
}

CState<CVampireCommander>* CVampireCommander_State::Normal_State(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return Normal(pActor, fTimeDelta, _iAnimIndex);
}

CState<CVampireCommander>* CVampireCommander_State::Walk_State(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	if (70.f < pActor->Calc_Distance())
	{
		return new CVampireCommander_Idle;
	}
	//return new CVampireCommander_Walk_F();
	return nullptr;
}

CState<CVampireCommander>* CVampireCommander_State::Attack_State(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	
	_int iAttackRandom = SMath::Random(0, 1); // ���ݰ� ü�� ȸ�� 
	_int iRandom =  SMath::Random(0,3); // ������ ���� 
	_int iRandomRange = SMath::Random(0, 2);//���Ÿ��� ���� 
	
	//���� ����!! 
	if (iAttackRandom == 0)
	{
		if (7.f > pActor->Calc_Distance() && 1.f < pActor->Calc_Distance())
		{
			switch (iRandom)
			{
			case 0:
				pActor->m_bLookAt = true;
				return new CVampireCommander_Melee1;
			case 1:
				pActor->m_bLookAt = true;
				return new CVampireCommander_Melee2;
			case 2:
				pActor->m_bLookAt = true;
				return new CVampireCommander_Ranged2;
			case 3:
				pActor->m_bLookAt = true;
				return new CVampireCommander_SyncedAttack_Fail;
			}
		}
		else if (50.f > pActor->Calc_Distance() && 7.f < pActor->Calc_Distance())
		{
			switch (iRandomRange)
			{
			case 0:
				pActor->m_bLookAt = true;
				return new CVampireCommander_Ranged1;
			case 1:
				pActor->m_bLookAt = true;
				return new CVampireCommander_Ranged3;
			case 2:
				pActor->m_bLookAt = true;
				return new CVampireCommander_Leap_Strat;
			}
		}
	}
	else if (iAttackRandom ==1)//ü���� 75% ���ϰ� �ȴٸ�! ������ �ɾ�� �� 
	{
		return new CVampireCommander_BloodRange_Start;
	}
	

	return nullptr;
}

CState<CVampireCommander>* CVampireCommander_State::HitNormal_State(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return nullptr;
}

CState<CVampireCommander>* CVampireCommander_State::Stun_State(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return nullptr;
}

CState<CVampireCommander>* CVampireCommander_State::Taunt_State(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	_int iRandomTaunt = SMath::Random(0, 4);
	switch (iRandomTaunt)
	{
	case 0:
		return new CVampireCommander_Taunt1;
		break;
	case 1:
		return new CVampireCommander_Taunt2;
		break;
	case 2:
		return new CVampireCommander_Taunt3;
		break;
	case 3:
		return new CVampireCommander_Taunt4;
		break;
	case 4:
		return new CVampireCommander_Taunt5;
		break;
	}
	

	return nullptr;
}

CState<CVampireCommander>* CVampireCommander_State::Spawn_State(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return nullptr;
}

CState<CVampireCommander>* CVampireCommander_State::Dead_State(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	return nullptr;
}

CState<CVampireCommander>* CVampireCommander_State::Normal(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	CState<CVampireCommander>* pState = { nullptr };

	_int iRandomTaunt = SMath::Random(0, 9); // ������ ���� 
	if (pActor->Is_Animation_End())
	{
		if (7 < iRandomTaunt)
			return Taunt_State(pActor, fTimeDelta, _iAnimIndex);
		
	}
	
	if (70.f < pActor->Calc_Distance())
	{
		pActor->m_bLookAt = true;
		pState = Idle(pActor, fTimeDelta, _iAnimIndex);
		if (pState)	return pState;
	}

	if (70.f > pActor->Calc_Distance() && 50.f < pActor->Calc_Distance())
	{
		pActor->m_bLookAt = true;
		pState = Walk_State(pActor, fTimeDelta, _iAnimIndex);
		if (pState)	return pState;
	}

 	if (50.f > pActor->Calc_Distance() && 1.f < pActor->Calc_Distance())
 	{
 
 		pState = Attack_State(pActor, fTimeDelta, _iAnimIndex);
 		if (pState)	return pState;
 	}
// 
// 	if (7.f > pActor->Calc_Distance() && 0.5f < pActor->Calc_Distance())
// 	{
// 
// 		pState = Attack_State(pActor, fTimeDelta, _iAnimIndex);
// 		if (pState)	return pState;
// 	}
	

	return nullptr;
}

CState<CVampireCommander>* CVampireCommander_State::Idle(CVampireCommander* pActor, _float fTimeDelta, _uint _iAnimIndex)
{
	pActor->m_bLookAt = true;
	return new CVampireCommander_Idle();
}
