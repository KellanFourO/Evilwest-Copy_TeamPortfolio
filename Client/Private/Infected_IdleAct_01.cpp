#include "Infected_IdleAct_01.h"

void CInfected_IdleAct_01::Initialize(CInfected* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CInfected>* CInfected_IdleAct_01::Update(CInfected* pActor, _float fTimeDelta)
{
	//pActor->Is_Inputable_Front(30) /* 30Ű������ �̻� �Ѿ�� true */

	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CInfected_IdleAct_01::Release(CInfected* pActor)
{
	__super::Release(pActor);
}
