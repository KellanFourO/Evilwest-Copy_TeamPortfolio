#include "..\Public\Player_Interaction_Destroy.h"

void CPlayer_Interaction_Destroy::Initialize(CPlayer* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CPlayer>* CPlayer_Interaction_Destroy::Update(CPlayer* pActor, _float fTimeDelta)
{
	__super::Update(pActor, fTimeDelta);

	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CPlayer_Interaction_Destroy::Release(CPlayer* pActor)
{
	__super::Release(pActor);
}
