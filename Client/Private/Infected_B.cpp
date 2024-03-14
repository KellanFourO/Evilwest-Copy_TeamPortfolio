#include "SMath.h"
#include "stdafx.h"
#include "Infected_B.h"
#include "GameInstance.h"

/* Spawn */
#include "Infected_SpawnClimb_01.h"
#include "Infected_SpawnClimb_02.h"
#include "Infected_SpawnCrawl_01.h"
#include "Infected_SpawnCrawl_02.h"
#include "Infected_SpawnFromCoffin0.h"
#include "Infected_SpawnGround.h"
#include "Infected_SpawnFromEgg_02.h"

CInfected_B::CInfected_B(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CInfected(pDevice, pContext, strPrototypeTag)
{
}

CInfected_B::CInfected_B(const CInfected_B& rhs)
	: CInfected(rhs)
{
}

HRESULT CInfected_B::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype());

	return S_OK;
}

HRESULT CInfected_B::Initialize(void* pArg)
{
	/* Ready_Component, Ready_PartObjects�� Initialize�� Ÿ�� �ö󰡴ٺ��� CCharacterŬ�������� �����ϰ�����.*/

	FAILED_CHECK(__super::Initialize(pArg));

	FAILED_CHECK(Ready_Option());

	return S_OK;
}

void CInfected_B::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CInfected_B::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pTarget = m_pGameInstance->Get_Player();

	if (m_pActor)
	{
		m_pActor->Update_State(fTimeDelta);
	}
}

void CInfected_B::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CInfected_B::Render()
{
	FAILED_CHECK(__super::Render());

	return S_OK;
}

HRESULT CInfected_B::Update_RenderState(_int _iHP)
{
	return E_NOTIMPL;
}

#pragma region Base Setting

HRESULT CInfected_B::Ready_Components()
{
	FAILED_CHECK(__super::Ready_Components());

	return S_OK;
}

HRESULT CInfected_B::Ready_Option()
{
	// 0		~	fAttack : �Ϲݰ��� �Ÿ� - �������Ŀ��� ��Ÿ���̶� �ް�����ġ�°� 
	// fAttack	~	fWalk	: �ɾ���� �Ÿ� 
	// fWalk	~			: �پ���°Ÿ� -> �Ϲݰ��ݿ� �����Ҷ����� �پ�� -> �ٸ鼭 �����ϴ� �̵��������� �� 
	m_eInfo.eType = INFECTED_TYPE::INFECTED_VESSEL_B;
	m_eInfo.RandomNumber = SMath::Random(1, 10);
	m_eInfo.fAttack_Distance = 5.f;
	m_eInfo.fWalk_Distance = 10.f;

	m_iHp = 30;
	m_iMaxHp = 30;
	m_pTarget = m_pGameInstance->Get_Player();

	/* ���� �Ҵ� & ������ �ִϸ��̼� ���� */
	if (m_pGameInstance->Get_NextLevel() != ECast(LEVEL::LEVEL_TOOL))
	{
		m_pActor = new CActor<CInfected>(this);

		switch (m_eInfo.RandomNumber >> 1)
		{
		case 1:
			//m_pActor->Set_State(new CInfected_SpawnClimb_01());
			m_pActor->Set_State(new CInfected_SpawnFromEgg_02());
			break;
		case 2:
			//m_pActor->Set_State(new CInfected_SpawnClimb_02());
			m_pActor->Set_State(new CInfected_SpawnFromEgg_02());
			break;
		case 3:
			//m_pActor->Set_State(new CInfected_SpawnCrawl_01());
			m_pActor->Set_State(new CInfected_SpawnFromEgg_02());
			break;
		case 4:
			//m_pActor->Set_State(new CInfected_SpawnCrawl_02());
			m_pActor->Set_State(new CInfected_SpawnFromEgg_02());
			break;
		case 5:
			//m_pActor->Set_State(new CInfected_SpawnFromCoffin0());
			m_pActor->Set_State(new CInfected_SpawnFromEgg_02());
			break;
		default:
			//m_pActor->Set_State(new CInfected_SpawnGround());
			m_pActor->Set_State(new CInfected_SpawnFromEgg_02());
			break;
		}
	}

	return S_OK;
}

HRESULT CInfected_B::Ready_PartObjects()
{
	/* For. Body */
	{
		CBody::BODY_DESC		BodyDesc = {};
		FAILED_CHECK(Add_Body(TEXT("Prototype_GameObject_Body_Infected_B"), BodyDesc));
	}

	/* For. Weapon */
	{
		CWeapon::WEAPON_DESC		WeaponDesc = {};
		WeaponDesc.m_pSocketBone = m_pBody->Get_BonePtr("RightHandIK");
		WeaponDesc.m_pParentTransform = m_pTransformCom;
		FAILED_CHECK(Add_Weapon(TEXT("Prototype_GameObject_Weapon_Infected_B"), "RightHandIK", WeaponDesc, TEXT("Weapon_Punch")));

		CWeapon* m_pWeapon = Get_Weapon(TEXT("Weapon_Punch"));
		m_pWeapon->Set_Enable(false);
	}
	return S_OK;

	return S_OK;
}

#pragma endregion


#pragma region Create, Clone, Free, Pool

CInfected_B* CInfected_B::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CInfected_B* pInstance = new CInfected_B(pDevice, pContext, strPrototypeTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInfected_B");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInfected_B::Clone(void* pArg)
{
	CInfected_B* pInstance = new CInfected_B(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInfected_B");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInfected_B::Pool()
{
	return new CInfected_B(*this);
}

void CInfected_B::Free()
{
	__super::Free();
}

#pragma endregion
