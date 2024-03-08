#include "..\Public\BanditHeavy_HitLight_F_02.h"

void CBanditHeavy_HitLight_F_02::Initialize(CBandit_Heavy* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);

}

CState<CBandit_Heavy>* CBanditHeavy_HitLight_F_02::Update(CBandit_Heavy* pActor, _float fTimeDelta)
{
	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CBanditHeavy_HitLight_F_02::Release(CBandit_Heavy* pActor)
{
	__super::Release(pActor);
}