#include "stdafx.h"
#include "UI_Player_Skill_Guige.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_Player_Skill_Guige::CUI_Player_Skill_Guige(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Player_Skill_Guige::CUI_Player_Skill_Guige(const CUI_Player_Skill_Guige& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_Skill_Guige::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Player_Skill_Guige::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fScaleX = 76;
	m_tUIInfo.fScaleY = 76;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	m_vCenter.x = m_pTransformCom->Get_Position().x;
	m_vCenter.y = m_pTransformCom->Get_Position().y;

	m_bActive = true;

	return S_OK;
}

void CUI_Player_Skill_Guige::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Player_Skill_Guige::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_pGameInstance->Key_Down(DIK_Y))
		{
			m_fRadius -= 0.1f;
		}

		if (m_pGameInstance->Key_Down(DIK_U))
		{
			m_fRadius += 0.1f;
		}

		__super::Tick(fTimeDelta);
	}

}

void CUI_Player_Skill_Guige::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_Player_Skill_Guige::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//! �� ���̴��� 0��° �н��� �׸��ž�.
	m_pShaderCom->Begin(5); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

	//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
	m_pVIBufferCom->Bind_VIBuffers();

	//! ���ε��� ����, �ε����� �׷�
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Player_Skill_Guige::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	//! For.Com_Texture1
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_cooldown_inactive"),
		TEXT("Com_Texture_Inactive"), reinterpret_cast<CComponent**>(&m_pTextureCom[INACTIVE]))))
		return E_FAIL;

	//! For.Com_Texture2
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_cooldown_active_big"),
		TEXT("Com_Texture_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	// error : ���̴� ���Ͽ� �ִ°Ŷ� Ÿ�� �ȸ¾���.
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Center", &m_vCenter, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Radius", &m_fRadius, sizeof(_float))))
		return E_FAIL;
	
	////! For.Com_Texture3
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CUI_Player_Skill_Guige::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[INACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_CoolDownTexture")))
		return E_FAIL;


	return S_OK;
}


json CUI_Player_Skill_Guige::Save_Desc(json& out_json)
{
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_Player_Skill_Guige::Load_Desc()
{

}

CUI_Player_Skill_Guige* CUI_Player_Skill_Guige::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Player_Skill_Guige* pInstance = new CUI_Player_Skill_Guige(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_Skill_Guige");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_Skill_Guige::Pool()
{
	return new CUI_Player_Skill_Guige(*this);
}

CGameObject* CUI_Player_Skill_Guige::Clone(void* pArg)
{
	CUI_Player_Skill_Guige* pInstance = new CUI_Player_Skill_Guige(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_Skill_Guige");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Player_Skill_Guige::Free()
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
