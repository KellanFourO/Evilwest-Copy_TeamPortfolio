#include "..\Public\Player_Sniper_WalkAim_F.h"

void CPlayer_Sniper_WalkAim_F::Initialize(CPlayer* pActor)
{
	__super::Initialize(pActor);

	//pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true, false);
}

CState<CPlayer>* CPlayer_Sniper_WalkAim_F::Update(CPlayer* pActor, _float fTimeDelta)
{
	__super::Update(pActor, fTimeDelta);

	pActor->Go_Straight(fTimeDelta * 0.5f);

	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CPlayer_Sniper_WalkAim_F::Release(CPlayer* pActor)
{
	__super::Release(pActor);
}