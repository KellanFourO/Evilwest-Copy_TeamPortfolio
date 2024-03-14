#include "stdafx.h"
#include "Data_Manager.h"
#include "GameInstance.h"
#include "Bullet_Bandit_Sniper.h"

CBullet_Bandit_Sniper::CBullet_Bandit_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CProjectile(pDevice, pContext, strPrototypeTag)
{
	/* �̰� �־�� pool�� ����Ѱ��� ����� �� ����. */
	m_bIsPoolObject = true;
}

CBullet_Bandit_Sniper::CBullet_Bandit_Sniper(const CBullet_Bandit_Sniper& rhs)
	: CProjectile(rhs)
{
}

HRESULT CBullet_Bandit_Sniper::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet_Bandit_Sniper::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC		GameObjectDesc = {};

	GameObjectDesc.fSpeedPerSec = 60.f;
	GameObjectDesc.fRotationPerSec = 0.f;
	FAILED_CHECK(__super::Initialize(&GameObjectDesc)); /* ������Ʈ ȣ�� */	

	m_iDamage = 0;
	m_fLifeTime = 1.5f;	

	return S_OK;
}

HRESULT CBullet_Bandit_Sniper::Ready_Components()
{
	_int iCurrentLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC		BoundingDesc = {};
	{
		BoundingDesc.iLayer = ECast(COLLISION_LAYER::MONSTER_ATTACK);
		BoundingDesc.fRadius = 0.3f;
		BoundingDesc.vCenter = _float3(0.f, BoundingDesc.fRadius, 0.f);

		FAILED_CHECK(__super::Add_Component(iCurrentLevel, TEXT("Prototype_Component_Collider_Sphere"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pCollider), &BoundingDesc));
	}

	return S_OK;
}

void CBullet_Bandit_Sniper::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CBullet_Bandit_Sniper::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	
	/* Super���� m_fLifeTime �� fTimeDelta �� ���ҽ�Ű�鼭 0 ���Ϸ� �������� �˾Ƽ� Ǯ�� �ݳ��ϵ����� + Collider Update */
	/* -> �̵� �ڵ� ��ü�� �����ؾ��� */
	//m_pCollider->Update(m_pTransformCom->Get_WorldMatrix());

	m_pTransformCom->Go_Straight(fTimeDelta);
}

void CBullet_Bandit_Sniper::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CBullet_Bandit_Sniper::Render()
{
	FAILED_CHECK(__super::Render());

	return S_OK;
}

void CBullet_Bandit_Sniper::OnCollisionEnter(CCollider* other)
{
}

void CBullet_Bandit_Sniper::OnCollisionStay(CCollider* other)
{
}

void CBullet_Bandit_Sniper::OnCollisionExit(CCollider* other)
{
}


HRESULT CBullet_Bandit_Sniper::Bind_ShaderResources()
{
	return S_OK;
}

#pragma region Create, Clone, Pool, Free

CBullet_Bandit_Sniper* CBullet_Bandit_Sniper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CBullet_Bandit_Sniper* pInstance = new CBullet_Bandit_Sniper(pDevice, pContext, strPrototypeTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Bullet_Bandit_Sniper");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_Bandit_Sniper::Clone(void* pArg)
{
	CBullet_Bandit_Sniper* pInstance = new CBullet_Bandit_Sniper(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Bullet_Bandit_Sniper");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBullet_Bandit_Sniper::Pool()
{
	return new CBullet_Bandit_Sniper(*this);
}

void CBullet_Bandit_Sniper::Free()
{
	__super::Free();

	//Safe_Release(m_pCollider);
}

#pragma endregion