#include "..\Public\Edgar_Sniper_Walk_FL.h"

void CEdgar_Sniper_Walk_FL::Initialize(CEdgar* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);

}

CState<CEdgar>* CEdgar_Sniper_Walk_FL::Update(CEdgar* pActor, _float fTimeDelta)
{
	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CEdgar_Sniper_Walk_FL::Release(CEdgar* pActor)
{
	__super::Release(pActor);
}
