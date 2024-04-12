#include "stdafx.h"
#include "UI_Icon_SelectAnim.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "UI_Manager.h"

CUI_Icon_SelectAnim::CUI_Icon_SelectAnim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Icon_SelectAnim::CUI_Icon_SelectAnim(const CUI_Icon_SelectAnim& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Icon_SelectAnim::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Icon_SelectAnim::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	//m_eType = UITYPE::LEVEL_UP;
	m_bActive = false;
	m_fAlpha = 0.f;
	//m_fLifeTime = 8000.f;
	m_vAxis = { 0.f, 0.f, 1.f, 0.f };
	m_fTime = (_float)GetTickCount64();

	return S_OK;
}

void CUI_Icon_SelectAnim::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Icon_SelectAnim::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Check_Disappear(fTimeDelta);

	// Picking -> Pos Update
	CUI* pTargetUI = m_pUIManager->Get_Select_Frame();

	_float3 vNotTarget = { -1.0f, -1.0f, -1.0f };

	if (pTargetUI != nullptr)
	{
		m_bActive = true;

		if (pTargetUI->Get_UIDesc().strUIName == "Kick")
		{
			_float3 vTargetPos = { pTargetUI->Get_Position().x + 5.f, pTargetUI->Get_Position().y, pTargetUI->Get_Position().z };
			m_pTransformCom->Set_Position(vTargetPos);
			m_pTransformCom->Set_Scaling(pTargetUI->Get_Transform()->Get_Scaled().x, pTargetUI->Get_Transform()->Get_Scaled().y, pTargetUI->Get_Transform()->Get_Scaled().z);
		}
		else if (pTargetUI->Get_UIDesc().strUIName == "UpperCut")
		{
			_float3 vTargetPos = { pTargetUI->Get_Position().x + 5.f, pTargetUI->Get_Position().y, pTargetUI->Get_Position().z };
			m_pTransformCom->Set_Position(vTargetPos);
			m_pTransformCom->Set_Scaling(pTargetUI->Get_Transform()->Get_Scaled().x, pTargetUI->Get_Transform()->Get_Scaled().y, pTargetUI->Get_Transform()->Get_Scaled().z);
		}
		else if (pTargetUI->Get_UIDesc().strUIName == "Punch")
		{
			_float3 vTargetPos = { pTargetUI->Get_Position().x + 5.f, pTargetUI->Get_Position().y, pTargetUI->Get_Position().z };
			m_pTransformCom->Set_Position(vTargetPos);
			m_pTransformCom->Set_Scaling(pTargetUI->Get_Transform()->Get_Scaled().x, pTargetUI->Get_Transform()->Get_Scaled().y, pTargetUI->Get_Transform()->Get_Scaled().z);
		}
		else if (pTargetUI->Get_UIDesc().strUIName == "Heal")
		{
			_float3 vTargetPos = { pTargetUI->Get_Position().x + 5.f, pTargetUI->Get_Position().y, pTargetUI->Get_Position().z };
			m_pTransformCom->Set_Position(vTargetPos);
			m_pTransformCom->Set_Scaling(pTargetUI->Get_Transform()->Get_Scaled().x, pTargetUI->Get_Transform()->Get_Scaled().y, pTargetUI->Get_Transform()->Get_Scaled().z);
		}
		else if (pTargetUI->Get_UIDesc().strUIName == "Revolver")
		{
			_float3 vTargetPos = { pTargetUI->Get_Position().x, pTargetUI->Get_Position().y, pTargetUI->Get_Position().z };
			m_pTransformCom->Set_Position(vTargetPos);
			m_pTransformCom->Set_Scaling(pTargetUI->Get_Transform()->Get_Scaled().x, pTargetUI->Get_Transform()->Get_Scaled().y, pTargetUI->Get_Transform()->Get_Scaled().z);
		}
		else if (pTargetUI->Get_UIDesc().strUIName == "Rifle")
		{
			_float3 vTargetPos = { pTargetUI->Get_Position().x, pTargetUI->Get_Position().y, pTargetUI->Get_Position().z };
			m_pTransformCom->Set_Position(vTargetPos);
			m_pTransformCom->Set_Scaling(pTargetUI->Get_Transform()->Get_Scaled().x, pTargetUI->Get_Transform()->Get_Scaled().y, pTargetUI->Get_Transform()->Get_Scaled().z);
		}
		else if (pTargetUI->Get_UIDesc().strUIName == "Shotgun")
		{
			_float3 vTargetPos = { pTargetUI->Get_Position().x, pTargetUI->Get_Position().y, pTargetUI->Get_Position().z };
			m_pTransformCom->Set_Position(vTargetPos);
			m_pTransformCom->Set_Scaling(pTargetUI->Get_Transform()->Get_Scaled().x, pTargetUI->Get_Transform()->Get_Scaled().y, pTargetUI->Get_Transform()->Get_Scaled().z);
		}
		else
		{
			m_pTransformCom->Set_Position(pTargetUI->Get_Position());
			m_pTransformCom->Set_Scaling(pTargetUI->Get_Transform()->Get_Scaled().x, pTargetUI->Get_Transform()->Get_Scaled().y, pTargetUI->Get_Transform()->Get_Scaled().z);
		}

		
	}
	else
	{
		m_bActive = false;
	}



	if (m_bActive == true)
	{
		// Rotation
		m_pTransformCom->RotationZaxis(fTimeDelta);
	}
}

void CUI_Icon_SelectAnim::Late_Tick(_float fTimeDelta)
{
	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_Icon_SelectAnim::Render()
{
	if (m_bActive == true)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		//! �� ���̴��� 0��° �н��� �׸��ž�.
		m_pShaderCom->Begin(0); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

		//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
		m_pVIBufferCom->Bind_VIBuffers();

		//! ���ε��� ����, �ε����� �׷�
		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Icon_SelectAnim::UI_Ready(_float fTimeDelta)
{
}

void CUI_Icon_SelectAnim::UI_Enter(_float fTimeDelta)
{
}

void CUI_Icon_SelectAnim::UI_Loop(_float fTimeDelta)
{
}

void CUI_Icon_SelectAnim::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_Icon_SelectAnim::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	//! For.Com_Texture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_perks_details_selection"),
		TEXT("Com_Texture_MagicFrame"), reinterpret_cast<CComponent**>(&m_pTextureCom[SELECT]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Icon_SelectAnim::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[SELECT]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

void CUI_Icon_SelectAnim::Check_Disappear(_float fTimeDelta)
{
	if (m_bDisappear == true)
	{
		m_bActive = Alpha_Plus(fTimeDelta);
	}
}

json CUI_Icon_SelectAnim::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_Icon_SelectAnim::Load_Desc()
{

}

CUI_Icon_SelectAnim* CUI_Icon_SelectAnim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Icon_SelectAnim* pInstance = new CUI_Icon_SelectAnim(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Icon_SelectAnim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Icon_SelectAnim::Pool()
{
	return new CUI_Icon_SelectAnim(*this);
}

CGameObject* CUI_Icon_SelectAnim::Clone(void* pArg)
{
	CUI_Icon_SelectAnim* pInstance = new CUI_Icon_SelectAnim(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Icon_SelectAnim");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Icon_SelectAnim::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
