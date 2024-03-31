#include "stdafx.h"
#include "UI_SelectList.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Renderer.h"
#include "UI_Manager.h"

CUI_SelectList::CUI_SelectList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_SelectList::CUI_SelectList(const CUI_SelectList& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SelectList::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_SelectList::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_SelectList::Priority_Tick(_float fTimeDelta)
{

}

void CUI_SelectList::Tick(_float fTimeDelta)
{

}

void CUI_SelectList::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	__super::Tick(fTimeDelta);

	Check_Picking(fTimeDelta);

	if (m_bActive)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_SelectList::Render()
{
	if (m_bActive)
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

void CUI_SelectList::UI_Ready(_float fTimeDelta)
{
}

void CUI_SelectList::UI_Enter(_float fTimeDelta)
{
}

void CUI_SelectList::UI_Loop(_float fTimeDelta)
{
}

void CUI_SelectList::UI_Exit(_float fTimeDelta)
{
}

void CUI_SelectList::Check_Picking(_float fTimeDelta)
{
	if (m_pUIManager->Get_Select() || m_pUIManager->Get_SelectPressing())
	{
		if (m_pUIManager->Get_strSelectUI() == "HBAO")
		{
			/* Position */
			if (m_tUIInfo.strUIName == "SELECT_HBAO")
			{
				m_bHBAO_Active = !m_bHBAO_Active;
				m_pTransformCom->Set_Position(m_pUIManager->Get_CurrentUI()->Get_Position());
			}
		}
		else if (m_pUIManager->Get_strSelectUI() == "FOG")
		{
			/* Position */
			if (m_tUIInfo.strUIName == "SELECT_FOG")
			{
				m_bFOG_Active = !m_bFOG_Active;
				m_pTransformCom->Set_Position(m_pUIManager->Get_CurrentUI()->Get_Position());
			}
		}
		else if (m_pUIManager->Get_strSelectUI() == "RADIAL_BLUR")
		{
			/* Position */
			if (m_tUIInfo.strUIName == "SELECT_RADIAL_BLUR")
			{
				m_bRadial_Blur_Active = !m_bRadial_Blur_Active;
				m_pTransformCom->Set_Position(m_pUIManager->Get_CurrentUI()->Get_Position());
			}
		}
		else if (m_pUIManager->Get_strSelectUI() == "DOF")
		{
			/* Position */
			if (m_tUIInfo.strUIName == "SELECT_DOF")
			{
				m_bDof_Active = !m_bDof_Active;
				m_pTransformCom->Set_Position(m_pUIManager->Get_CurrentUI()->Get_Position());
			}
		}
		else if (m_pUIManager->Get_strSelectUI() == "HDR")
		{
			/* Position */
			if (m_tUIInfo.strUIName == "SELECT_HDR")
			{
				m_bHDR_Active = !m_bHDR_Active;
				m_pTransformCom->Set_Position(m_pUIManager->Get_CurrentUI()->Get_Position());
			}
		}
		else if (m_pUIManager->Get_strSelectUI() == "SHADOW")
		{
			/* Position */
			if (m_tUIInfo.strUIName == "SELECT_SHADOW")
			{
				m_bShadow_Active = !m_bShadow_Active;
				m_pTransformCom->Set_Position(m_pUIManager->Get_CurrentUI()->Get_Position());
			}
		}
		else if (m_pUIManager->Get_strSelectUI() == "PBR")
		{
			
			/* Position */
			if (m_tUIInfo.strUIName == "SELECT_PBR")
			{
				m_bPBR_Active = !m_bPBR_Active;
				m_pTransformCom->Set_Position(m_pUIManager->Get_CurrentUI()->Get_Position());
			}
		}


	}
}

HRESULT CUI_SelectList::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	//! For.Com_Texture2 // Select
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_mm_selection"),
		TEXT("ui_element_mm_selection"), reinterpret_cast<CComponent**>(&m_pTextureCom[SELECT_LIST]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SelectList::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (m_bHBAO_Active == true || m_bFOG_Active == true || m_bRadial_Blur_Active == true ||
		m_bDof_Active == true || m_bHDR_Active == true || m_bShadow_Active == true || m_bPBR_Active == true)
	{
		if (FAILED(m_pTextureCom[SELECT_LIST]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}


	return S_OK;
}

json CUI_SelectList::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_SelectList::Load_Desc()
{

}

CUI_SelectList* CUI_SelectList::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_SelectList* pInstance = new CUI_SelectList(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_SelectList");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_SelectList::Pool()
{
	return new CUI_SelectList(*this);
}

CGameObject* CUI_SelectList::Clone(void* pArg)
{
	CUI_SelectList* pInstance = new CUI_SelectList(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_SelectList");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_SelectList::Free()
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
