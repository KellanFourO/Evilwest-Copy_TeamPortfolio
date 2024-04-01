#include "stdafx.h"
#include "Player_Weapon_Whip.h"
#include "GameInstance.h"
#include "MasterCamera.h"
#include "SpringCamera.h"
#include "Data_Manager.h"

CPlayer_Weapon_Whip::CPlayer_Weapon_Whip(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CWeapon_Player(pDevice, pContext, strPrototypeTag)
{

}

CPlayer_Weapon_Whip::CPlayer_Weapon_Whip(const CPlayer_Weapon_Whip& rhs)
	: CWeapon_Player(rhs)
{
}

HRESULT CPlayer_Weapon_Whip::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer_Weapon_Whip::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	string path = "../Bin/DataFiles/Data_Weapon/Player/Whip/Whip.json";
	json In_Json;
	CJson_Utility::Load_Json(path.c_str(), In_Json);
	m_pTransformCom->Load_FromJson(In_Json);

	return S_OK;
}

void CPlayer_Weapon_Whip::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CPlayer_Weapon_Whip::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (true == m_pGameInstance->isIn_WorldPlanes(m_pParentTransform->Get_State(CTransform::STATE_POSITION), 2.f))
	{
		m_pModelCom->Play_Animation(fTimeDelta, _float3(0.f, 0.f, 0.f));
	}

	if (m_pGameInstance->Key_Down(DIK_C))
	{
		string path = "../Bin/DataFiles/Data_Weapon/Player/Whip/Whip.json";
		{
			json Out_Json;
			m_pTransformCom->Write_Json(Out_Json);
			CJson_Utility::Save_Json(path.c_str(), Out_Json);
		}
	}
}

void CPlayer_Weapon_Whip::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CPlayer_Weapon_Whip::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}


HRESULT CPlayer_Weapon_Whip::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	//if (FAILED(__super::Ready_Components()))
	//	return E_FAIL;


	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Model_Player_Weapon_Whip"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

CPlayer_Weapon_Whip* CPlayer_Weapon_Whip::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CPlayer_Weapon_Whip* pInstance = new CPlayer_Weapon_Whip(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Weapon_Whip");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Weapon_Whip::Clone(void* pArg)
{
	CPlayer_Weapon_Whip* pInstance = new CPlayer_Weapon_Whip(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Weapon_Whip");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CPlayer_Weapon_Whip::Pool()
{
	return new CPlayer_Weapon_Whip(*this);
}

void CPlayer_Weapon_Whip::Free()
{
	__super::Free();
}
