#include "Player_EnergyWhip_Leap.h"
#include "Weapon.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Bone.h"
#include "Data_Manager.h"

void CPlayer_EnergyWhip_Leap::Initialize(CPlayer* pActor)
{
	__super::Initialize(pActor);
	pActor->Set_Animation(g_iAnimIndex, CModel::ANIM_STATE_NORMAL, true, true, 15);

	// 이펙트 생성 테스트
	EFFECT_MANAGER->Play_Effect("Zapper_Dash_30.json", pActor);
	//EFFECT_MANAGER->Create_Effect("Player/Zapper_Dash/", "Zapper_Dash_29.json", pActor);
}

CState<CPlayer>* CPlayer_EnergyWhip_Leap::Update(CPlayer* pActor, _float fTimeDelta)
{	
	__super::Update(pActor, fTimeDelta);

	if (false == m_bFlags[0])
	{
		pActor->Chasing_Attack(fTimeDelta, 30.f, 8);
		m_bFlags[0] = pActor->Is_Inputable_Front(22);
	}

	if (false == m_bFlags[1] && pActor->Is_Inputable_Front(15))
	{
		pActor->Apply_Shake_And_Blur(Power::Heavy);

		CCharacter* pTarget = pActor->Get_Target();
		pTarget->Look_At_OnLand(pActor->Get_Position_Vector());

		if (CData_Manager::GetInstance()->Is_AdditionalSkill_Learned(Additional_Skill::ELECTROCUTE))
		{
			pTarget->Hitted_Electrocute();
		}
		else 
		{
			pTarget->Hitted_Front(Power::Medium);
		}
		
		m_bFlags[1] = true;
	}


	if (pActor->Is_Animation_End())
	{
		return __super::Update_State(pActor, fTimeDelta, g_iAnimIndex);
	}

	return nullptr;
}

void CPlayer_EnergyWhip_Leap::Release(CPlayer* pActor)
{
	__super::Release(pActor);
	pActor->Set_Target(nullptr);
}
