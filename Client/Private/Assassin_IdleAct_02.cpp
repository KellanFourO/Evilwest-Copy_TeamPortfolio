#include "..\Public\Assassin_IdleAct_02.h"

void CAssassin_IdleAct_02::Initialize(CAssassin* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);

}

CState<CAssassin>* CAssassin_IdleAct_02::Update(CAssassin* pActor, _float fTimeDelta)
{
	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CAssassin_IdleAct_02::Release(CAssassin* pActor)
{
	__super::Release(pActor);
}