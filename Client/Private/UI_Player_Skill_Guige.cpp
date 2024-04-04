#include "stdafx.h"
#include "UI_Player_Skill_Guige.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "UI_Manager.h"

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

	//m_vCenter.x = m_pTransformCom->Get_Position().x;
	//m_vCenter.y = m_pTransformCom->Get_Position().y;

	m_vCenter.x = 0.5;
	m_vCenter.y = 0.5;

	//m_fMaxCoolTime = m_fMaxCoolTime;

	m_bActive = true;

	m_iMaskNum = 128;
	//m_fCoolTime = m_fMaxCoolTime;

	return S_OK;
}

void CUI_Player_Skill_Guige::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Player_Skill_Guige::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bActive == true)
	{
		m_fCoolTime -= fTimeDelta; // ���ҽ�ų���� �������� ������ (�ؽ�ó�� ������)
		m_iShaderNum = 5; // ���� ������ pass

		if (m_fCoolTime <= 0.f) // ���� á�� �� (0)
		{
			// Ȱ��ȭ
			Check_SkillActive(fTimeDelta, SKILLSTATE::ACTIVE);
			m_fCoolTime = 0.f;
		}
		else
		{
			// ��Ȱ��ȭ
			Check_SkillActive(fTimeDelta, SKILLSTATE::COOLDOWN);
		}
	}

}

void CUI_Player_Skill_Guige::Late_Tick(_float fTimeDelta)
{
	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_Player_Skill_Guige::Render()
{
	if (m_bActive == true)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		//! �� ���̴��� 0��° �н��� �׸��ž�.
		m_pShaderCom->Begin(5); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

		//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
		m_pVIBufferCom->Bind_VIBuffers();

		//! ���ε��� ����, �ε����� �׷�
		m_pVIBufferCom->Render();
	}
	return S_OK;
}

void CUI_Player_Skill_Guige::UI_Ready(_float fTimeDelta)
{
}

void CUI_Player_Skill_Guige::UI_Enter(_float fTimeDelta)
{
}

void CUI_Player_Skill_Guige::UI_Loop(_float fTimeDelta)
{
}

void CUI_Player_Skill_Guige::UI_Exit(_float fTimeDelta)
{
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
	
	//! For.Com_Distortion_Mask
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mask"),
		TEXT("Com_Distortion_Mask"), reinterpret_cast<CComponent**>(&m_pDistortionCom[MASK]))))
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

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	// error : ���̴� ���Ͽ� �ִ°Ŷ� Ÿ�� �ȸ¾���.
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Center", &m_vCenter, sizeof(_float2))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Radius", &m_fRadius, sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CoolTime", &m_fCoolTime, sizeof(_float))))
		return E_FAIL;

	// ���� ��Ÿ���� ����ؼ� �����ش�.
	_float fRatio = (m_fMaxCoolTime - m_fCoolTime) / m_fMaxCoolTime;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &fRatio, sizeof(_float))))
		return E_FAIL;

	//if (FAILED(m_pTextureCom[INACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
	//	return E_FAIL;
	if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pDistortionCom[MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_iMaskNum)))
		return E_FAIL;

	//if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_CoolDownTexture")))
	//	return E_FAIL;

	return S_OK;
}

void CUI_Player_Skill_Guige::Check_SkillActive(_float fTimeDelta, SKILLSTATE eState)
{
	/* Left */
	if (m_tUIInfo.strUIName == "LeftHUD_Top")
		m_pUIManager->LeftSkillState("LeftHUD_Top", eState);

	if (m_tUIInfo.strUIName == "LeftHUD_Right")
		m_pUIManager->LeftSkillState("LeftHUD_Right", eState);

	if (m_tUIInfo.strUIName == "LeftHUD_Bottom")
		m_pUIManager->LeftSkillState("LeftHUD_Bottom", eState);

	if (m_tUIInfo.strUIName == "LeftHUD_Left")
		m_pUIManager->LeftSkillState("LeftHUD_Left", eState);

	/* Right */
	if (m_tUIInfo.strUIName == "RightHUD_Top")
		m_pUIManager->LeftSkillState("RightHUD_Top", eState);

	if (m_tUIInfo.strUIName == "RightHUD_Right")
		m_pUIManager->LeftSkillState("RightHUD_Right", eState);

	if (m_tUIInfo.strUIName == "RightHUD_Bottom")
		m_pUIManager->LeftSkillState("RightHUD_Bottom", eState);

	if (m_tUIInfo.strUIName == "RightHUD_Left")
		m_pUIManager->LeftSkillState("RightHUD_Left", eState);

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
