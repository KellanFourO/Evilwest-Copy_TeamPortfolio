#include "..\Public\Sniper_Melee_03.h"

void CSniper_Melee_03::Initialize(CBandit_Sniper* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CBandit_Sniper>* CSniper_Melee_03::Update(CBandit_Sniper* pActor, _float fTimeDelta)
{
	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CSniper_Melee_03::Release(CBandit_Sniper* pActor)
{
	__super::Release(pActor);
}