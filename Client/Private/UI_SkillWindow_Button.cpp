#include "stdafx.h"
#include "UI_SkillWindow_Button.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Renderer.h"
#include "UI_Manager.h"

CUI_SkillWindow_Button::CUI_SkillWindow_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_SkillWindow_Button::CUI_SkillWindow_Button(const CUI_SkillWindow_Button& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SkillWindow_Button::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_SkillWindow_Button::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	m_bButtonUI = true;

	m_fWithProgress = -1.f;
	m_fChangeScale = 2.f;

	m_fScaleX = 140.f;
	m_fScaleY = 70.f;

	return S_OK;
}

void CUI_SkillWindow_Button::Priority_Tick(_float fTimeDelta)
{

}

void CUI_SkillWindow_Button::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bActive == true)
	{
		if (m_fWithProgress < 1.f)
			m_fWithProgress += fTimeDelta;
		else
			m_fWithProgress = -1.f;

		Check_Picking(fTimeDelta);
		Check_State(fTimeDelta);
		
	}
}

void CUI_SkillWindow_Button::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();


	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_SkillWindow_Button::Render()
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

void CUI_SkillWindow_Button::UI_Ready(_float fTimeDelta)
{
}

void CUI_SkillWindow_Button::UI_Enter(_float fTimeDelta)
{
}

void CUI_SkillWindow_Button::UI_Loop(_float fTimeDelta)
{
}

void CUI_SkillWindow_Button::UI_Exit(_float fTimeDelta)
{
}

void CUI_SkillWindow_Button::Check_Picking(_float fTimeDelta)
{
	if (m_bPick == true)
	{
		if (g_UIMouseDownLB == true)
		{
			if (m_tUIInfo.strProtoTag == "WeaponButton" ||
				m_tUIInfo.strProtoTag == "WeaponButtonActive")
			{
				m_bSelectButton = true;
				m_pUIManager->Select_SkillWindowButton("WeaponButton", false); // ������ ��ư�� �����ϰ� ��� ���ش�.
				m_pUIManager->NonActive_SkillIcon();
				m_pUIManager->NonActive_SkillFrame();
			}
			else if (m_tUIInfo.strProtoTag == "SkillButton" ||
				m_tUIInfo.strProtoTag == "SkillButtonActive")
			{
				m_bSelectButton = true;
				m_pUIManager->Select_SkillWindowButton("SkillButton", false); // ������ ��ư�� �����ϰ� ��� ���ش�.
				m_pUIManager->Active_SkillIcon(true);
				m_pUIManager->Active_SkillFrame(true);
			}
		}
	}
}

void CUI_SkillWindow_Button::Check_State(_float fTimeDelta)
{
	if (m_tUIInfo.strProtoTag == "WeaponButton" || m_tUIInfo.strProtoTag == "WeaponButtonActive")
	{
		if (m_bSelectButton == true)
		{
			if (m_fScaleX < 170.f)
				Change_SizeX((+m_fChangeScale * 1.5f));

			if (m_fScaleY < 90.f)
				Change_SizeY((+m_fChangeScale * 1.5f));
		}
		else
		{
			if (m_fScaleX > 140.f)
				Change_SizeX((-m_fChangeScale * 1.5f));

			if (m_fScaleY > 70.f)
				Change_SizeY((-m_fChangeScale * 1.5f));
		}

	}
	if (m_tUIInfo.strProtoTag == "SkillButton" || m_tUIInfo.strProtoTag == "SkillButtonActive")
	{
		if (m_bSelectButton == true)
		{
			if (m_fScaleX < 170.f)
				Change_SizeX((+m_fChangeScale * 1.5f));

			if (m_fScaleY < 90.f)
				Change_SizeY((+m_fChangeScale * 1.5f));
		}
		else
		{
			if (m_fScaleX > 140.f)
				Change_SizeX((-m_fChangeScale * 1.5f));

			if (m_fScaleY > 70.f)
				Change_SizeY((-m_fChangeScale * 1.5f));
		}
	}
}

HRESULT CUI_SkillWindow_Button::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

#pragma region 
	if (m_tUIInfo.strProtoTag == "WeaponButton" ||
		m_tUIInfo.strProtoTag == "WeaponButtonActive")
	{
		//! For.Com_Texture2 // NonActive
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("WeaponButton"),
			TEXT("Com_Texture_SkillWindow_NonActive"), reinterpret_cast<CComponent**>(&m_pTextureCom[NONACTIVE]))))
			return E_FAIL;

		//! For.Com_Texture2 // Active
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("WeaponButtonActive"),
			TEXT("Com_Texture_SkillWindow_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
			return E_FAIL;
	}
	else if (m_tUIInfo.strProtoTag == "SkillButton" ||
			 m_tUIInfo.strProtoTag == "SkillButtonActive")
	{
		//! For.Com_Texture2 // NonActive
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("SkillButton"),
			TEXT("Com_Texture_SkillWindow_NonActive"), reinterpret_cast<CComponent**>(&m_pTextureCom[NONACTIVE]))))
			return E_FAIL;

		//! For.Com_Texture2 // Active
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("SkillButtonActive"),
			TEXT("Com_Texture_SkillWindow_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
			return E_FAIL;
	}
#pragma endregion

	return S_OK;
}

HRESULT CUI_SkillWindow_Button::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WithProgress", &m_fWithProgress, sizeof(_float))))
		return E_FAIL;

	if (m_tUIInfo.strProtoTag == "WeaponButton")
	{
		if (m_bSelectButton == true || m_bPick == true)
		{
			if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureCom[NONACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
		}
	}


	if (m_tUIInfo.strProtoTag == "SkillButton")
	{
		if (m_bSelectButton == true || m_bPick == true)
		{
			if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTextureCom[NONACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
		}

	}
	else
	{

	}

	return S_OK;
}

json CUI_SkillWindow_Button::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_SkillWindow_Button::Load_Desc()
{

}

CUI_SkillWindow_Button* CUI_SkillWindow_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_SkillWindow_Button* pInstance = new CUI_SkillWindow_Button(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_SkillWindow_Button");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_SkillWindow_Button::Pool()
{
	return new CUI_SkillWindow_Button(*this);
}

CGameObject* CUI_SkillWindow_Button::Clone(void* pArg)
{
	CUI_SkillWindow_Button* pInstance = new CUI_SkillWindow_Button(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_SkillWindow_Button");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_SkillWindow_Button::Free()
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
