#include "stdafx.h"
#include "GameInstance.h"
#include "Infected_Walk_B.h"
#include "Infected_Sprint_F_Melee_03.h"

void CInfected_Sprint_F_Melee_03::Initialize(CInfected* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CInfected>* CInfected_Sprint_F_Melee_03::Update(CInfected* pActor, _float fTimeDelta)
{
	/* �� �Ÿ� ���� */
	if (pActor->Is_Animation_End())
	{
		/* �޸��� �������� ���� ���ǵ� ���� */
		pActor->Get_Transform()->Set_Speed(10.f);
		return new CInfected_Walk_B();
	}

	return nullptr;
}

void CInfected_Sprint_F_Melee_03::Release(CInfected* pActor)
{
	__super::Release(pActor);
}
