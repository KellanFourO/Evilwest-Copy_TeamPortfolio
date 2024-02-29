#include "..\Public\Infected_MeleeDynamic_RU_03.h"

void CInfected_MeleeDynamic_RU_03::Initialize(CInfected* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CInfected>* CInfected_MeleeDynamic_RU_03::Update(CInfected* pActor, _float fTimeDelta)
{
	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CInfected_MeleeDynamic_RU_03::Release(CInfected* pActor)
{
	__super::Release(pActor);
}