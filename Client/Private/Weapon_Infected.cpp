#include "stdafx.h"
#include "GameInstance.h"
#include "Weapon_Infected.h"
#include "Bone.h"
#include "Character.h"

CWeapon_Infected::CWeapon_Infected(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CWeapon(pDevice, pContext, strPrototypeTag)
{
}

CWeapon_Infected::CWeapon_Infected(const CWeapon_Infected& rhs)
	: CWeapon(rhs)
{
}

HRESULT CWeapon_Infected::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype());

	return S_OK;
}

HRESULT CWeapon_Infected::Initialize(void* pArg)
{
	FAILED_CHECK(__super::Initialize(pArg));

	m_iDamage = 3.f;
	m_fForce = 0.1f;
	return S_OK;
}

void CWeapon_Infected::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CWeapon_Infected::Tick(_float fTimeDelta)
{
	m_vDir = m_pTransformCom->Get_Look();
	__super::Tick(fTimeDelta);
}

void CWeapon_Infected::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CWeapon_Infected::Render()
{
	FAILED_CHECK(__super::Render());

	return S_OK;
}

HRESULT CWeapon_Infected::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	return S_OK;
}

HRESULT CWeapon_Infected::Bind_ShaderResources()
{
	FAILED_CHECK(__super::Bind_ShaderResources());

	return S_OK;
}

void CWeapon_Infected::OnCollisionEnter(CCollider* other)
{
	CCharacter* pTarget_Character = Get_Target_Character(other);

	if (pTarget_Character != nullptr)
	{
		m_eHitDirection = Direction::Front;
		m_eHitPower = Power::Medium;
		m_fForce = 0.5f;

		//_uint iDamage, _vector vDir, _float fForce, _float fStiffnessRate, Direction eHitDirection, Power eHitPower, _bool bIsMelee)
		pTarget_Character->Set_Hitted(4.f, m_pTransformCom->Get_Look(), m_fForce, 1.f, m_eHitDirection, m_eHitPower);
	}

}

void CWeapon_Infected::OnCollisionStay(CCollider* other)
{
}

void CWeapon_Infected::OnCollisionExit(CCollider* other)
{
}

void CWeapon_Infected::Free()
{
	__super::Free();
}

