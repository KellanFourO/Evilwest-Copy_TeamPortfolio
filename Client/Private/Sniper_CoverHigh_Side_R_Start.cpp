#include "stdafx.h"
#include "Sniper_CoverHigh_Side_R_Start.h"
#include "Sniper_CoverHigh_Side_R_AimPose.h"

void CSniper_CoverHigh_Side_R_Start::Initialize(CBandit_Sniper* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);
}

CState<CBandit_Sniper>* CSniper_CoverHigh_Side_R_Start::Update(CBandit_Sniper* pActor, _float fTimeDelta)
{
	if (pActor->Is_Animation_End()) /* 리스폰 하는 애니메이션이 끝난다면 */
	{
		cout << "Move " << endl;
		return new CSniper_CoverHigh_Side_R_AimPose();
	}
	return nullptr;
}

void CSniper_CoverHigh_Side_R_Start::Release(CBandit_Sniper* pActor)
{
	__super::Release(pActor);
}
