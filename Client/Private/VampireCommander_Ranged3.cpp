#include "VampireCommander_Ranged3.h"
#include "VampireCommander_Idle.h"
#include "GameInstance.h"
#include "Body_VampireCommander.h"

#include "VampireCommander_Weapon.h"

void CVampireCommander_Ranged3::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);

	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true);

	CBody_VampireCommander* pBody = dynamic_cast<CBody_VampireCommander*>(pActor->Get_Body());
	pBody->Set_RenderState(CBody_VampireCommander::RENDER_STATE::ATTACK);

	pActor->m_bLookAt = false;


	CWeapon* pWeapon_R = pActor->Get_Weapon(TEXT("Weapon_hand_R"));
	dynamic_cast<CVampireCommander_Weapon*>(pWeapon_R)->Play_Trail(true);	// Ʈ���� �ѱ�

	CWeapon* pWeapon_L = pActor->Get_Weapon(TEXT("Weapon_hand_L"));
	dynamic_cast<CVampireCommander_Weapon*>(pWeapon_L)->Play_Trail(true);	// Ʈ���� �ѱ�

}

CState<CVampireCommander>* CVampireCommander_Ranged3::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	CWeapon* pWeapon_R = pActor->Get_Weapon(TEXT("Weapon_hand_R"));
	CWeapon* pWeapon_L = pActor->Get_Weapon(TEXT("Weapon_hand_L"));

	if (m_bFlags[1] == false)
	{
		//pActor->m_bLookAt = true;
	}

	if (pActor->Is_Inputable_Front(66))
	{
		m_bFlags[1] = true;
		if (m_bfirstCheck)
		{
			m_pGameInstance->Add_CloneObject(LEVEL_INTRO_BOSS, L"Layer_Monster", L"Prototype_GameObject_VampireCommander_Projectile_Range3");
			m_bfirstCheck = false;
		}
	}
	else if (pActor->Is_Inputable_Front(68))
	{
		m_bFlags[1] = false;
	}

	if (pActor->Is_Animation_End())
	{
		dynamic_cast<CVampireCommander_Weapon*>(pWeapon_R)->Play_Trail(false);	// Ʈ���� ����
		dynamic_cast<CVampireCommander_Weapon*>(pWeapon_L)->Play_Trail(false);	// Ʈ���� ����

		return new CVampireCommander_Idle();
	}

	return nullptr;
}

void CVampireCommander_Ranged3::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);

	pActor->m_bLookAt = true;

}
