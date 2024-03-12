#include "..\Public\Player_Screamer_Finisher_ZapperPull_01.h"

void CPlayer_Screamer_Finisher_ZapperPull_01::Initialize(CPlayer* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CPlayer>* CPlayer_Screamer_Finisher_ZapperPull_01::Update(CPlayer* pActor, _float fTimeDelta)
{
	__super::Update(pActor, fTimeDelta);

	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CPlayer_Screamer_Finisher_ZapperPull_01::Release(CPlayer* pActor)
{
	__super::Release(pActor);
}