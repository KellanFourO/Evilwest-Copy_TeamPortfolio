#include "stdafx.h"
#include "UI_Player_Skill_Icon.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_Player_Skill_Icon::CUI_Player_Skill_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Player_Skill_Icon::CUI_Player_Skill_Icon(const CUI_Player_Skill_Icon& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_Skill_Icon::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Player_Skill_Icon::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fScaleX = 55.0f;
	m_tUIInfo.fScaleY = 55.0f;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_Player_Skill_Icon::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Player_Skill_Icon::Tick(_float fTimeDelta)
{

}

void CUI_Player_Skill_Icon::Late_Tick(_float fTimeDelta)
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

HRESULT CUI_Player_Skill_Icon::Render()
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

HRESULT CUI_Player_Skill_Icon::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	
	//! For.Com_Texture1 // ���
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_icons_hud_locked"),
		TEXT("Com_Texture_Lock"), reinterpret_cast<CComponent**>(&m_pTextureCom[ICON_LOCK]))))
		return E_FAIL;

#pragma region �������� �ؽ�ó�� ��� ���� �� �ְ� ������Ѵ�.
	/* ù Create�� �ƹ��ų� �⺻ ���������� Initialize���� �ױ׸� �������ְ�, ���õ� �ؽ�ó�� �Ľ��ϸ� �Ľ̵� �ؽ�ó�� ����ϰ� ������. */
	wstring strPrototag;
	m_pGameInstance->String_To_WString(m_tUIInfo.strProtoTag, strPrototag);
	wstring strPrototagCooldown;
	
	// �κ� ���ڿ� �˻�
	size_t found = strPrototag.find(L"ui_icon");

	// �߰ߵ� ���
	if (found != std::wstring::npos) 
	{
		strPrototagCooldown = strPrototag + L"_cooldown";

		//! For.Com_Texture2 // ��Ÿ��	
		if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototagCooldown,
			TEXT("Com_Texture_Cooldown"), reinterpret_cast<CComponent**>(&m_pTextureCom[ICON_COOLDOWN]))))
			return E_FAIL;
	}
	else //�߰ߵ��� ���� ���
	{

	}
	
	//! For.Com_Texture2 // Ȱ��ȭ
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
		TEXT("Com_Texture_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ICON_ACTIVE]))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CUI_Player_Skill_Icon::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	string TestName = m_tUIInfo.strObjectName;
	for (_int i = (_int)0; i < (_int)TEXTURE_END; ++i)
	{
		switch (i)
		{
		case CUI_Player_Skill_Icon::ICON_LOCK:
		{
			if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
			break;
		}
		case CUI_Player_Skill_Icon::ICON_COOLDOWN:
		{
			if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture_Second")))
				return E_FAIL;

			break;
		}
		case CUI_Player_Skill_Icon::ICON_ACTIVE:
		{
			//if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture_Third")))
			//	return E_FAIL;
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

json CUI_Player_Skill_Icon::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_Player_Skill_Icon::Load_Desc()
{

}

CUI_Player_Skill_Icon* CUI_Player_Skill_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Player_Skill_Icon* pInstance = new CUI_Player_Skill_Icon(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_Skill_Icon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_Skill_Icon::Pool()
{
	return new CUI_Player_Skill_Icon(*this);
}

CGameObject* CUI_Player_Skill_Icon::Clone(void* pArg)
{
	CUI_Player_Skill_Icon* pInstance = new CUI_Player_Skill_Icon(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_Skill_Icon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Player_Skill_Icon::Free()
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
