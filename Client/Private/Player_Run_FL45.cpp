#include "..\Public\Player_Run_FL45.h"

void CPlayer_Run_FL45::Initialize(CPlayer* pActor)
{
	__super::Initialize(pActor);

	//pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true);
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true, false);
	

}

CState<CPlayer>* CPlayer_Run_FL45::Update(CPlayer* pActor, _float fTimeDelta)
{
	__super::Update(pActor, fTimeDelta);

	pActor->Go_Straight_L45(fTimeDelta);

	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CPlayer_Run_FL45::Release(CPlayer* pActor)
{
	__super::Release(pActor);
}