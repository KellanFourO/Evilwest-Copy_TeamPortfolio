#include "stdafx.h"
#include "UI_ItemShowcase.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_ItemShowcase::CUI_ItemShowcase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_ItemShowcase::CUI_ItemShowcase(const CUI_ItemShowcase& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ItemShowcase::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_ItemShowcase::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	//m_tUIInfo.fScaleX = 55.0f;
	//m_tUIInfo.fScaleY = 55.0f;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	m_eType = UITYPE::REWARD;

	return S_OK;
}

void CUI_ItemShowcase::Priority_Tick(_float fTimeDelta)
{

}

void CUI_ItemShowcase::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Check_Disappear(fTimeDelta);

	if (m_bActive == true)
	{

	}
}

void CUI_ItemShowcase::Late_Tick(_float fTimeDelta)
{
	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_ItemShowcase::Render()
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

void CUI_ItemShowcase::UI_Ready(_float fTimeDelta)
{
}

void CUI_ItemShowcase::UI_Enter(_float fTimeDelta)
{
}

void CUI_ItemShowcase::UI_Loop(_float fTimeDelta)
{
}

void CUI_ItemShowcase::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_ItemShowcase::Ready_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("rewards_item_showcase"),
		TEXT("Com_Texture_ItemShowcase"), reinterpret_cast<CComponent**>(&m_pTextureCom[ITEMSOWCASE]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ItemShowcase::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	string TestName = m_tUIInfo.strObjectName;
	for (_int i = (_int)0; i < (_int)TEXTURE_END; ++i)
	{
		switch (i)
		{
		case CUI_ItemShowcase::ITEMSOWCASE:
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

json CUI_ItemShowcase::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_ItemShowcase::Load_Desc()
{

}

CUI_ItemShowcase* CUI_ItemShowcase::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_ItemShowcase* pInstance = new CUI_ItemShowcase(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ItemShowcase");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_ItemShowcase::Pool()
{
	return new CUI_ItemShowcase(*this);
}

CGameObject* CUI_ItemShowcase::Clone(void* pArg)
{
	CUI_ItemShowcase* pInstance = new CUI_ItemShowcase(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ItemShowcase");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_ItemShowcase::Free()
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
