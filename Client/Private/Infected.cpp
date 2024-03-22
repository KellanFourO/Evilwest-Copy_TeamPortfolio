#include "stdafx.h"
#include "Infected.h"
#include "GameInstance.h"
#include "Body_Infected.h"
#include "Infected_Idle.h"
#include "Infected_SpawnGround.h"
#include "Data_Manager.h"

//#include "Infected_HitLight_F_01_NEW.h"
//#include "Infected_HitLight_FL_01_NEW.h"
//#include "Infected_HitLight_FR_01_NEW.h"
#include "Infected_HitNormal_FR90_01_NEW.h"
#include "Infected_Scared_02.h"
#include "Infected_Scared_03.h"
#include "Infected_HitNormalToStun_01_NEW.h"
#include "Infected_HitHeavyToStun_01_NEW.h"

#include "Infected_HitNormal_F_01_NEW.h"
#include "Infected_HitNormal_F_02_NEW.h"
#include "Infected_HitNormal_F_03_NEW.h"
#include "Infected_HitNormal_FL_01_NEW.h"
#include "Infected_HitNormal_FR_01_NEW.h"

#include "Infected_HitHeavy_F_01_NEW.h"
#include "Infected_HitHeavy_F_02_NEW.h"
#include "Infected_HitHeavy_FL_01_NEW.h"
#include "Infected_HitHeavy_FL_02_NEW.h"
#include "Infected_HitHeavy_FR_01_NEW.h"
#include "Infected_HitHeavy_FR_02_NEW.h"

#include "Infected_KnockFrontLight_B_01_NEW.h"
#include "Infected_KnockFrontLight_F_01_NEW.h"
#include "Infected_KnockFrontCannonball_F_01_TEMP.h"

#include "Infected_DeathLight_B_01_NEW.h"
#include "Infected_DeathLight_F_01_NEW.h"
#include "Infected_DeathHeavy_F_01_NEW.h"
#include "Infected_SpawnCrawl_01.h"

#include "Infected_Electrocute_Loop.h"
#include "Infected_HitLightOpened_F_01_NEW.h"
#include "Infected_HitLightOpened_L_01.h"
#include "Infected_HitLightOpened_R_01.h"
#include "Infected_OpenStatePull_F_01.h"
#include "Infected_OpenStateTimeout.h"


CInfected::CInfected(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CMonster_Character(pDevice, pContext, strPrototypeTag)
{
}

CInfected::CInfected(const CInfected& rhs)
	: CMonster_Character(rhs)
{
}

HRESULT CInfected::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype());

	return S_OK;
}

HRESULT CInfected::Initialize(void* pArg)
{
	if (pArg == nullptr)
	{
		CGameObject::GAMEOBJECT_DESC		GameObjectDesc = {};

		GameObjectDesc.fSpeedPerSec = 10.f;
		GameObjectDesc.fRotationPerSec = XMConvertToRadians(90.0f);
		FAILED_CHECK(__super::Initialize(&GameObjectDesc));
	}
	else 
	{
		FAILED_CHECK(__super::Initialize(pArg));
	}

	/* Actor 설정 */
	if (m_pGameInstance->Get_NextLevel() != ECast(LEVEL::LEVEL_TOOL))
	{
		m_pActor = new CActor<CInfected>(this);
		m_pActor->Set_State(new CInfected_SpawnCrawl_01());
	}

	/* Target 설정 */
	m_pTarget = m_pGameInstance->Get_Player();
	return S_OK;
}

void CInfected::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CInfected::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_pActor)
	{
		m_pActor->Update_State(fTimeDelta);
	}
}

void CInfected::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	//m_pGameInstance->Add_DebugRender(m_pNavigationCom); /* 아 테스트로 넣었었음 */ 
}

HRESULT CInfected::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CInfected::Ready_Components()
{
	return S_OK;
}

void CInfected::Hitted_Left(Power ePower)
{
	/* 무기 강도 */
	switch (ePower)
	{
	case Engine::Light:
		m_pActor->Set_State(new CInfected_Scared_03());
		break;
	case Engine::Medium:
		m_pActor->Set_State(new CInfected_HitNormal_FL_01_NEW());
		break;
	case Engine::Heavy:
		m_pActor->Set_State(new CInfected_HitHeavy_FL_01_NEW());
		break;
	default:
		m_pActor->Set_State(new CInfected_HitNormal_FL_01_NEW());
		break;
	}
}

void CInfected::Hitted_Right(Power ePower)
{
	switch (ePower)
	{
	case Engine::Light:
		m_pActor->Set_State(new CInfected_Scared_02());
		break;
	case Engine::Medium:
		m_pActor->Set_State(new CInfected_HitNormal_FR_01_NEW());
		break;
	case Engine::Heavy:
		m_pActor->Set_State(new CInfected_HitHeavy_FR_01_NEW());
		break;
	default:
		m_pActor->Set_State(new CInfected_HitNormal_FR_01_NEW());
		break;
	}
}

void CInfected::Hitted_Front(Power ePower)
{
	switch (ePower)
	{
	case Engine::Light:
		m_pActor->Set_State(new CInfected_HitNormalToStun_01_NEW());
		break;
	case Engine::Medium:
		m_pActor->Set_State(new CInfected_HitNormal_F_01_NEW());
		break;
	case Engine::Heavy:
		m_pActor->Set_State(new CInfected_HitHeavy_F_01_NEW());
		break;
	default:
		m_pActor->Set_State(new CInfected_HitNormal_F_01_NEW());
		break;
	}
}

void CInfected::Hitted_Knock(_bool bIsCannonball)
{
	if (bIsCannonball)
	{
		m_pActor->Set_State(new CInfected_KnockFrontCannonball_F_01_TEMP());
	}
	else 
	{
		m_pActor->Set_State(new CInfected_KnockFrontLight_F_01_NEW());
	}
}

void CInfected::Hitted_Dead(Power ePower)
{
	switch (ePower)
	{
	case Engine::Light:
		m_pActor->Set_State(new CInfected_DeathLight_F_01_NEW());
		break;
	case Engine::Medium:
		m_pActor->Set_State(new CInfected_DeathLight_B_01_NEW());
		break;
	case Engine::Heavy:
		m_pActor->Set_State(new CInfected_DeathHeavy_F_01_NEW());
		break;

	default:
		break;
	}
}

void CInfected::Hitted_Electrocute()
{
	m_pActor->Set_State(new CInfected_Electrocute_Loop());
}

void CInfected::Hitted_OpenState_Pull()
{
	m_pActor->Set_State(new CInfected_OpenStatePull_F_01());
}

void CInfected::Hitted_Opened(Direction eDirection)
{
	switch (eDirection)
	{
	case Engine::Left:
		m_pActor->Set_State(new CInfected_HitLightOpened_L_01());
		break;
	case Engine::Right:
		m_pActor->Set_State(new CInfected_HitLightOpened_R_01());
		break;
	case Engine::Front:
		m_pActor->Set_State(new CInfected_HitLightOpened_F_01_NEW());
		break;
	}
}

void CInfected::Free()
{
	__super::Free();

	if (m_pActor)
	{
		Safe_Delete(m_pActor);
	}

}
