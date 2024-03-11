#include "stdafx.h"
#include "Infected_SpawnClimb_01.h"
#include "Infected_IdleAct_03.h"

void CInfected_SpawnClimb_01::Initialize(CInfected* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CInfected>* CInfected_SpawnClimb_01::Update(CInfected* pActor, _float fTimeDelta)
{
	//if (pActor->Is_Animation_End()) /* ������ �ϴ� �ִϸ��̼��� �����ٸ� */
	//{
	//	return new CInfected_IdleAct_03();
	//}
	//return nullptr;
	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CInfected_SpawnClimb_01::Release(CInfected* pActor)
{
	__super::Release(pActor);
}
