#include "stdafx.h"
#include "GameInstance.h"
#include "Infected_Run_F.h"
#include "Infected_Sprint_F_Melee_02.h"
#include "Infected_Sprint_F_Melee_03.h"

void CInfected_Run_F::Initialize(CInfected* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true, false); /* ��Ʈ�ִϸ��̼� ���x */
	//pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true,false, 30); // ��Ʈ�ִϸ��̼� ��� false, 30�� Ű�����Ӻ��� �ִϸ��̼� ���� 
	
	pActor->Get_Transform()->Set_Speed(2.5f);	/* �����̴� �ӵ� ���� */
	pActor->Set_StiffnessRate_Upper(3.f);		/* �ִϸ��̼� ����ӵ� ���� */
}

CState<CInfected>* CInfected_Run_F::Update(CInfected* pActor, _float fTimeDelta)
{
	pActor->Look_At_Target();
	pActor->Go_Straight(fTimeDelta);

	switch (pActor->Get_Info().eType)
	{
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_A:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_B:
	case Client::CInfected::INFECTED_TYPE::INFECTED_VESSEL_C:
	{
		_float fDist = pActor->Calc_Distance(m_pGameInstance->Get_Player());

		if (fDist <= pActor->Get_Info().fAttack_Distance + 2.f)
		{
			_int iRemain = pActor->Get_Info().RandomNumber % 2;	

			pActor->Get_Transform()->Set_Speed(10.f);	/* �����̴� �ӵ� ���� */
			pActor->Set_StiffnessRate_Upper(1.f);		/* �ִϸ��̼� ����ӵ� ���� */
			pActor->Set_MonsterAttackState(true);

			if ( iRemain == 0 )
				return new CInfected_Sprint_F_Melee_02();
			else 
				return new CInfected_Sprint_F_Melee_03();
		}
	}
	break;

	case Client::CInfected::INFECTED_TYPE::INFECTED_PROTEUS:
		break;
	case Client::CInfected::INFECTED_TYPE::INFECTED_WASTER:
		break;
	}

	//if (pActor->Is_Inputable_Front(20))
	//	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);

	return nullptr;
}

void CInfected_Run_F::Release(CInfected* pActor)
{
	__super::Release(pActor);
}
