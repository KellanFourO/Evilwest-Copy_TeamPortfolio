#include "stdafx.h"
#include "Player_Weapon_Revolver.h"
#include "GameInstance.h"

CPlayer_Weapon_Revolver::CPlayer_Weapon_Revolver(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CWeapon_Player(pDevice, pContext, strPrototypeTag)
{

}

CPlayer_Weapon_Revolver::CPlayer_Weapon_Revolver(const CPlayer_Weapon_Revolver& rhs)
	:CWeapon_Player(rhs)
{
}

HRESULT CPlayer_Weapon_Revolver::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype())) 
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Weapon_Revolver::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	string path = "../Bin/DataFiles/Data_Weapon/Player/Revolver/Revolver.json";
	json In_Json;
	CJson_Utility::Load_Json(path.c_str(), In_Json);
	m_pTransformCom->Load_FromJson(In_Json);

	return S_OK;
}

void CPlayer_Weapon_Revolver::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CPlayer_Weapon_Revolver::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//if (m_pGameInstance->Key_Down(DIK_F))
	//{
	//	string path = "../Bin/DataFiles/Data_Weapon/Player/Revolver/Reload.json";

	//	{
	//		json Out_Json;
	//		m_pTransformCom->Write_Json(Out_Json);
	//		CJson_Utility::Save_Json(path.c_str(), Out_Json);
	//	}
	//	//{
	//	//   json In_Json;
	//	//   CJson_Utility::Load_Json(path.c_str(), In_Json);
	//	//   m_pTransformCom->Load_FromJson(In_Json);
	//	//}

	//}
}

void CPlayer_Weapon_Revolver::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CPlayer_Weapon_Revolver::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Weapon_Revolver::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Model_Player_Weapon_Revolver"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

CPlayer_Weapon_Revolver* CPlayer_Weapon_Revolver::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CPlayer_Weapon_Revolver* pInstance = new CPlayer_Weapon_Revolver(pDevice, pContext, strPrototypeTag);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Weapon_Revolver");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Weapon_Revolver::Clone(void* pArg)
{
	CPlayer_Weapon_Revolver* pInstance = new CPlayer_Weapon_Revolver(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Weapon_Revolver");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Weapon_Revolver::Pool()
{
	return new CPlayer_Weapon_Revolver(*this);
}

void CPlayer_Weapon_Revolver::Free()
{
	__super::Free();
}
