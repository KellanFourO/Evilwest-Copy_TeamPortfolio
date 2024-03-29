#include "stdafx.h"
#include "UI_Option_Title_Button.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_Option_Title_Button::CUI_Option_Title_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Option_Title_Button::CUI_Option_Title_Button(const CUI_Option_Title_Button& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Option_Title_Button::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Option_Title_Button::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_Option_Title_Button::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Option_Title_Button::Tick(_float fTimeDelta)
{

}

void CUI_Option_Title_Button::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	__super::Tick(fTimeDelta);

	if (m_bActive)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_Option_Title_Button::Render()
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

void CUI_Option_Title_Button::UI_Ready(_float fTimeDelta)
{
}

void CUI_Option_Title_Button::UI_Enter(_float fTimeDelta)
{
}

void CUI_Option_Title_Button::UI_Loop(_float fTimeDelta)
{
}

void CUI_Option_Title_Button::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_Option_Title_Button::Ready_Components()
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
	/* ù Create�� �ƹ��ų� �⺻ ���������� Initialize���� �ױ׸� �������ְ�, ���õ� �ؽ�ó�� �Ľ��ϸ� �Ľ̵� �ؽ�ó�� ����ϰ� ������. */
	wstring strPrototag;
	m_pGameInstance->String_To_WString(m_tUIInfo.strProtoTag, strPrototag);
	wstring strPrototagCooldown;

	// �κ� ���ڿ� �˻�
	size_t found = strPrototag.find(L"ui_icons");

	// �߰ߵ� ���
	if (found != std::wstring::npos) // == �̸� ���°� nullptró�� ���
	{
		//! For.Com_Texture2 // NonActive
		if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
			TEXT("Com_Texture_NonActive"), reinterpret_cast<CComponent**>(&m_pTextureCom[NONACTIVE]))))
			return E_FAIL;

		// ������ "_" ��ġ ã��
		size_t Lastpos = strPrototag.find_last_of('_');

		// Lastpos "NA"�� "A"�� ����
		strPrototag.replace(Lastpos + 1, 2, L"A");

		//! For.Com_Texture2 // Active
		if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
			TEXT("Com_Texture_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
			return E_FAIL;
	}
	else //�߰ߵ��� ���� ���
	{
		//! For.Com_Texture2 // NonActive
		if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
			TEXT("Com_Texture_NonActive"), reinterpret_cast<CComponent**>(&m_pTextureCom[NONACTIVE]))))
			return E_FAIL;

		//! For.Com_Texture2 // Active
		if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
			TEXT("Com_Texture_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
			return E_FAIL;
	}
#pragma endregion

	return S_OK;
}

HRESULT CUI_Option_Title_Button::Bind_ShaderResources()
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

json CUI_Option_Title_Button::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_Option_Title_Button::Load_Desc()
{

}

CUI_Option_Title_Button* CUI_Option_Title_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Option_Title_Button* pInstance = new CUI_Option_Title_Button(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Option_Title_Button");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Option_Title_Button::Pool()
{
	return new CUI_Option_Title_Button(*this);
}

CGameObject* CUI_Option_Title_Button::Clone(void* pArg)
{
	CUI_Option_Title_Button* pInstance = new CUI_Option_Title_Button(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Option_Title_Button");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Option_Title_Button::Free()
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
