#include "Infected_Electrocute_Loop.h"
#include "Infected_Idle.h"
#include "Body_Infected_D.h"
#include "Weapon_Infected_D.h"

#include "Effect_Manager.h"
#include "Effect.h"

void CInfected_Electrocute_Loop::Initialize(CInfected* pActor)
{
	__super::Initialize(pActor);

	if (CInfected::INFECTED_TYPE::INFECTED_WASTER == pActor->Get_Info().Get_Type())
		pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true, false);
	else
		pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_LOOP, true, false);

	pActor->Set_MonsterAttackState(true);

	// 5��¥�� ��������Ʈ ����
	m_pEffect = EFFECT_MANAGER->Play_Effect("Hit/", "Hit_Electrocute_01.json", pActor, false);
}

CState<CInfected>* CInfected_Electrocute_Loop::Update(CInfected* pActor, _float fTimeDelta)
{
	if (CInfected::INFECTED_TYPE::INFECTED_WASTER == pActor->Get_Info().Get_Type())
	{
		// �����Ϸ� �� - �ִϸ��̼� �������� ���� || �׾ �� 
		if (pActor->Is_Animation_End() || 0 >= pActor->Get_CurHP()) 
		{
			CBody_Infected_D* pBody = dynamic_cast<CBody_Infected_D*>(pActor->Get_Body());
			pBody->Set_DeadState(false); // ������ ��Ȱ��ȭ 
			pBody->Collider_Off(); // �ٵ��ݶ��̴� ��Ȱ��ȭ 

			//CWeapon_Infected_D* m_pWeapon = dynamic_cast<CWeapon_Infected_D*>(pActor->Get_Weapon(TEXT("Weapon_Bomb"))); /* �̸� �̰ɷ� ����������� �о���� */
			//m_pWeapon->Set_Enable(true);		// �����ݶ��̴� Ȱ��ȭ = ���� �ݶ��̴� 

			//pActor->Get_Weapon(TEXT("Weapon_Bomb"))->Set_Enable(true);

			if (false == m_bFlags[0])
			{
				pActor->Explosion();
				pActor->Set_EnemyHUD_Dead();
				EFFECT_MANAGER->Play_Effect("Parasiter", "Monster_ExplosionNonLoop.json", nullptr, pActor->Get_Position());

				m_bFlags[0] = true;
			}

			__super::Bomb_State(pActor, fTimeDelta, g_iAnimIndex);
		}
	}
	else
	{
		// �ٸ����ʹ� �����̹Ƿ� �����ð� 
		if (false == pActor->Is_ElectrocuteTime())
		{
			return new CInfected_Idle();
		}

	}
	return nullptr;
}

void CInfected_Electrocute_Loop::Release(CInfected* pActor)
{
	__super::Release(pActor);


	if (m_pEffect != nullptr)	// ����Ʈ ����
	{
		EFFECT_MANAGER->Return_ToPool(m_pEffect);
		m_pEffect = nullptr;
	}
}
