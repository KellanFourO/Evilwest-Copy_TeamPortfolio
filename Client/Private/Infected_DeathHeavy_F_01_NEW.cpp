#include "..\Public\Infected_DeathHeavy_F_01_NEW.h"

void CInfected_DeathHeavy_F_01_NEW::Initialize(CInfected* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CInfected>* CInfected_DeathHeavy_F_01_NEW::Update(CInfected* pActor, _float fTimeDelta)
{
	if (pActor->Is_Animation_End())
	{
		return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
	}

	return nullptr;
}

void CInfected_DeathHeavy_F_01_NEW::Release(CInfected* pActor)
{	
	__super::Release(pActor);
}
