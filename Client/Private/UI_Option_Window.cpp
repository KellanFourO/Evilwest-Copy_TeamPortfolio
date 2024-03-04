#include "stdafx.h"
#include "UI_Option_Window.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_Option_Window::CUI_Option_Window(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Option_Window::CUI_Option_Window(const CUI_Option_Window& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Option_Window::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Option_Window::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fScaleX = g_iWinsizeX;
	m_tUIInfo.fScaleY = g_iWinsizeY;
	m_tUIInfo.fPositionZ = 0.9;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;


	return S_OK;
}

void CUI_Option_Window::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Option_Window::Tick(_float fTimeDelta)
{

}

void CUI_Option_Window::Late_Tick(_float fTimeDelta)
{

	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_Option_Window::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//! �� ���̴��� 0��° �н��� �׸��ž�.
	m_pShaderCom->Begin(0); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

	//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
	m_pVIBufferCom->Bind_VIBuffers();

	//! ���ε��� ����, �ε����� �׷�
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Option_Window::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	//! For.Com_Texture OptionBackGround
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Inventory_Background"),
		TEXT("Com_Texture_OptionBackGround"), reinterpret_cast<CComponent**>(&m_pTextureCom[BACKGROUND]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Option_Window::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	for (_int i = (_int)0; i < (_int)TEXTURE_END; ++i)
	{
		switch (i)
		{
		case CUI_Option_Window::BACKGROUND:
		{
			if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
			break;
		}
		default:
			break;
		}
	}

	return S_OK;
}

json CUI_Option_Window::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_Option_Window::Load_Desc()
{

}

CUI_Option_Window* CUI_Option_Window::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Option_Window* pInstance = new CUI_Option_Window(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Option_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Option_Window::Pool()
{
	return new CUI_Option_Window(*this);
}

CGameObject* CUI_Option_Window::Clone(void* pArg)
{
	CUI_Option_Window* pInstance = new CUI_Option_Window(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Option_Window");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Option_Window::Free()
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
