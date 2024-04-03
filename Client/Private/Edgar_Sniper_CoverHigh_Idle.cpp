#include "..\Public\Edgar_Sniper_CoverHigh_Idle.h"

void CEdgar_Sniper_CoverHigh_Idle::Initialize(CEdgar* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);

}

CState<CEdgar>* CEdgar_Sniper_CoverHigh_Idle::Update(CEdgar* pActor, _float fTimeDelta)
{
	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CEdgar_Sniper_CoverHigh_Idle::Release(CEdgar* pActor)
{
	__super::Release(pActor);
}
