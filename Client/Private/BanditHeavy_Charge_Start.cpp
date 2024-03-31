#include "..\Public\BanditHeavy_Charge_Start.h"

void CBanditHeavy_Charge_Start::Initialize(CBandit_Heavy* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);

	//CWeapon* pWeapon = pActor->Set_Weapon_Enable(BANDIT_HEAVY_WEAPON, true);
	//pWeapon
	//	->Set_Damage(10)
	//	->Set_Direction(Direction::Front)
	//	->Set_Power(Power::Medium)
	//	->Set_Force(0.3f)
	//	;

	//pWeapon->Set_Enable_Collisions(false);
}

CState<CBandit_Heavy>* CBanditHeavy_Charge_Start::Update(CBandit_Heavy* pActor, _float fTimeDelta)
{
	pActor->Look_At_Target_Lerp(fTimeDelta);

	//if (false == m_bFlags[0])
	//{
	//	m_bFlags[0] = pActor->Is_Inputable_Front(35);
	//	if (true == m_bFlags[0])
	//	{
	//		pActor->Set_Weapon_Collisions_Enable(BANDIT_HEAVY_WEAPON, true);
	//	}
	//}
	//else if (false == m_bFlags[1])
	//{
	//	m_bFlags[1] = pActor->Is_Inputable_Front(60);
	//	if (true == m_bFlags[1])
	//	{
	//		pActor->Set_Weapon_Collisions_Enable(BANDIT_HEAVY_WEAPON, false);
	//	}
	//}

	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CBanditHeavy_Charge_Start::Release(CBandit_Heavy* pActor)
{
	__super::Release(pActor);
}