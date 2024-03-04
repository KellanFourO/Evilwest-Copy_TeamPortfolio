#include "..\Public\Sniper_WalkAim_B.h"

void CSniper_WalkAim_B::Initialize(CBandit_Sniper* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
}

CState<CBandit_Sniper>* CSniper_WalkAim_B::Update(CBandit_Sniper* pActor, _float fTimeDelta)
{
	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CSniper_WalkAim_B::Release(CBandit_Sniper* pActor)
{
	__super::Release(pActor);
}