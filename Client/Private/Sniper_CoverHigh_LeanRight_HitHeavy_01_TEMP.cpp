#include "..\Public\Sniper_CoverHigh_LeanRight_HitHeavy_01_TEMP.h"

void CSniper_CoverHigh_LeanRight_HitHeavy_01_TEMP::Initialize(CBandit_Sniper* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CBandit_Sniper>* CSniper_CoverHigh_LeanRight_HitHeavy_01_TEMP::Update(CBandit_Sniper* pActor, _float fTimeDelta)
{
	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CSniper_CoverHigh_LeanRight_HitHeavy_01_TEMP::Release(CBandit_Sniper* pActor)
{
	__super::Release(pActor);
}