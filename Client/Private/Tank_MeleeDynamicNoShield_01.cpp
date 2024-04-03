#include "..\Public\Tank_MeleeDynamicNoShield_01.h"

void CTank_MeleeDynamicNoShield_01::Initialize(CTank* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);

	CWeapon* pWeapon = pActor->Set_Weapon_Enable(TANK_WEAPON_PUNCH_R, true);
	pWeapon
		->Set_Damage(10)
		->Set_Direction(Direction::Front)
		->Set_Power(Power::Medium)
		->Set_Force(0.5f)
		;

	pWeapon->Set_Enable_Collisions(false);

}

CState<CTank>* CTank_MeleeDynamicNoShield_01::Update(CTank* pActor, _float fTimeDelta)
{
	if (false == m_bFlags[0])
	{
		m_bFlags[0] = pActor->Is_Inputable_Front(23);
		if (true == m_bFlags[0])
		{
			CWeapon* pWeapon = pActor->Set_Weapon_Collisions_Enable(TANK_WEAPON_PUNCH_R, true);
		}
	}
	else if (false == m_bFlags[1])
	{
		m_bFlags[1] = pActor->Is_Inputable_Front(44);
		if (true == m_bFlags[1]) 
		{
			CWeapon* pWeapon = pActor->Set_Weapon_Collisions_Enable(TANK_WEAPON_PUNCH_R, false);
		}
	}
	else
	{
		pActor->Look_At_Target_Lerp(fTimeDelta);
	}

	if (pActor->Is_Animation_End())
	{
		return Idle(pActor, fTimeDelta, g_iAnimIndex);
	}

	return nullptr;

	//return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CTank_MeleeDynamicNoShield_01::Release(CTank* pActor)
{
	__super::Release(pActor);
	CWeapon* pWeapon = pActor->Set_Weapon_Enable(TANK_WEAPON_PUNCH_R, false);
}
