#include "..\Public\Player_MeleeCombo_02.h"

#include "Player_Empowered_Idle.h"

void CPlayer_MeleeCombo_02::Initialize(CPlayer* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);


	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_Punch_L"));

	pWeapon
		->Set_Damage(0.f)
		->Set_Direction(Direction::Left)
		->Set_Power(Power::Light)
		->Set_Force(0.5f);

	pWeapon->Set_Enable(true);
}

CState<CPlayer>* CPlayer_MeleeCombo_02::Update(CPlayer* pActor, _float fTimeDelta)
{
	__super::Update(pActor, fTimeDelta);

	return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
}

void CPlayer_MeleeCombo_02::Release(CPlayer* pActor)
{
	__super::Release(pActor);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_Punch_L"));
	pWeapon->Set_Enable(false);
}
