#include "stdafx.h"
#include "UI_ElementList.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_ElementList::CUI_ElementList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_ElementList::CUI_ElementList(const CUI_ElementList& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ElementList::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_ElementList::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_ElementList::Priority_Tick(_float fTimeDelta)
{

}

void CUI_ElementList::Tick(_float fTimeDelta)
{

}

void CUI_ElementList::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	__super::Tick(fTimeDelta);

	if (m_bActive)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return;
	}
}

HRESULT CUI_ElementList::Render()
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

void CUI_ElementList::UI_Ready(_float fTimeDelta)
{
}

void CUI_ElementList::UI_Enter(_float fTimeDelta)
{
}

void CUI_ElementList::UI_Loop(_float fTimeDelta)
{
}

void CUI_ElementList::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_ElementList::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

#pragma region �������� �ؽ�ó�� ��� ���� �� �ְ� ������Ѵ�.
	//! For.Com_Texture2 // NonActive
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_list_tab"),
		TEXT("Com_Texture_Element_List_NonActive"), reinterpret_cast<CComponent**>(&m_pTextureCom[NONACTIVE]))))
		return E_FAIL;

	//! For.Com_Texture2 // Active
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_list_navigation"),
		TEXT("Com_Texture_Element_List_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CUI_ElementList::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (m_bPick == true)
	{
		if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom[NONACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	return S_OK;
}

json CUI_ElementList::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_ElementList::Load_Desc()
{

}

CUI_ElementList* CUI_ElementList::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_ElementList* pInstance = new CUI_ElementList(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ElementList");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_ElementList::Pool()
{
	return new CUI_ElementList(*this);
}

CGameObject* CUI_ElementList::Clone(void* pArg)
{
	CUI_ElementList* pInstance = new CUI_ElementList(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ElementList");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_ElementList::Free()
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
