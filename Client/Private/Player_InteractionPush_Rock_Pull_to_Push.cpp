#include "..\Public\Player_InteractionPush_Rock_Pull_to_Push.h"

void CPlayer_InteractionPush_Rock_Pull_to_Push::Initialize(CPlayer* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CPlayer>* CPlayer_InteractionPush_Rock_Pull_to_Push::Update(CPlayer* pActor, _float fTimeDelta)
{
	__super::Update(pActor, fTimeDelta);

	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CPlayer_InteractionPush_Rock_Pull_to_Push::Release(CPlayer* pActor)
{
	__super::Release(pActor);
}
