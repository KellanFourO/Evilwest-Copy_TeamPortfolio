#include "VampireCommander_SyncedAttack.h"
#include "VampireCommander_Idle.h"
#include "Body_VampireCommander.h"
#include "VampireCommander_Weapon.h"
#include "GameInstance.h"

void CVampireCommander_SyncedAttack::Initialize(CVampireCommander* pActor)
{
	__super::Initialize(pActor);
	//���⼭ Fail���� �б����� Trackposition �޾Ƽ� �����;� �� 
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true,true,48);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_hand_R"));
	dynamic_cast<CVampireCommander_Weapon*>(pWeapon)->Play_Trail(true);	// Ʈ���� �ѱ�
	CWeapon* pWeapon_L = pActor->Get_Weapon(TEXT("Weapon_hand_L"));
	dynamic_cast<CVampireCommander_Weapon*>(pWeapon_L)->Play_Trail(true);	// Ʈ���� �ѱ�
	pWeapon
		->Set_Damage(50)
		->Set_Direction(Direction::Right)
		->Set_Power(Power::Medium)
		->Set_Force(0.f);

	pWeapon->Set_Enable(true);

	m_pGameInstance->Play_Sound(L"VAMPIRE_ATTACK", L"commander_lesser_vo_attackB004.wav", SOUND_ENEMY_VOICE, 8.f);
	

	CBody_VampireCommander* pBody = dynamic_cast<CBody_VampireCommander*>(pActor->Get_Body());
	pBody->Set_RenderState(CBody_VampireCommander::RENDER_STATE::ATTACK);
}

CState<CVampireCommander>* CVampireCommander_SyncedAttack::Update(CVampireCommander* pActor, _float fTimeDelta)
{
	
	
	if (false == m_bFlags[0])
	{
		m_bFlags[0] = pActor->Is_Inputable_Front(51);
		if (true == m_bFlags[0])
		{
			m_pGameInstance->Play_Sound(L"VAMPIRE_ATTACK", L"william_attack_melee_whoosh001.wav", SOUND_ENEMY_ATTACK, 8.f);
		}
	}

	if (false == m_bFlags[1])
	{
		m_bFlags[1] = pActor->Is_Inputable_Front(96);
		if (true == m_bFlags[1])
		{
			m_pGameInstance->Play_Sound(L"VAMPIRE_BODYFALL", L"commander_lesser_mvm_bodyfall001.wav", SOUND_ENEMY_ATTACK2, 10.f);
		}
	}

	if (false == m_bFlags[2])
	{
		m_bFlags[2] = pActor->Is_Inputable_Front(135);
		if (true == m_bFlags[2])
		{
			m_pGameInstance->Play_Sound(L"VAMPIRE_TAUNT", L"commander_lesser_vo_taunt005.wav", SOUND_ENEMY_VOICE, 10.f);
		}
	}

	//  �̽�ų�� �÷��̾ ��ƴٰ� ���� ������ �Ŷ� �÷��̾�� �����Ǿ�� ��! 
	if (pActor->Is_Animation_End())
	{
		return new CVampireCommander_Idle();
	}


	return nullptr;
}

void CVampireCommander_SyncedAttack::Release(CVampireCommander* pActor)
{
	__super::Release(pActor);

	CWeapon* pWeapon = pActor->Get_Weapon(TEXT("Weapon_hand_R"));
	dynamic_cast<CVampireCommander_Weapon*>(pWeapon)->Play_Trail(false);	// Ʈ���� �ѱ�
	CWeapon* pWeapon_L = pActor->Get_Weapon(TEXT("Weapon_hand_L"));
	dynamic_cast<CVampireCommander_Weapon*>(pWeapon_L)->Play_Trail(false);	// Ʈ���� �ѱ�

	pWeapon->Set_Enable(false);


}
