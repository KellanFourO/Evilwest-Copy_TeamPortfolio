#include "VampireCommander_Melee1.h"
#include "VampireCommander_Idle.h"
#include "Body_VampireCommander.h"

void CVampireCommander_Melee1::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_hand_R"));

	pWeapon
		->Set_Damage(30)
		->Set_Direction(Direction::Right)
		->Set_Power(Power::Medium)
		->Set_Force(0.5f);
	CBody_VampireCommander* pBody = dynamic_cast<CBody_VampireCommander*>(pActor->Get_Body());
	pBody->Set_RenderState(CBody_VampireCommander::RENDER_STATE::ATTACK);
	
}

CState<CVampireCommander>* CVampireCommander_Melee1::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_hand_R"));
	if (m_bFlags[1] == false)
	{
		pActor->m_bLookAt = true;
	}

	if (pActor->Is_Inputable_Front(24))
	{
		m_bFlags[1] = true;
		pWeapon->Set_Enable(true);

	}
	else if (pActor->Is_Inputable_Front(25))
	{
		m_bFlags[1] = false;
		pWeapon->Set_Enable(false);
	}
	if (pActor->Is_Animation_End())
	{
		return new CVampireCommander_Idle();
	}

	return nullptr;
}

void CVampireCommander_Melee1::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_hand_R"));
	pWeapon->Set_Enable(false);
}
