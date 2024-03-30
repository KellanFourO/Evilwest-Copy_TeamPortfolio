#include "stdafx.h"
#include "UI_MainMenuList.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Renderer.h"
#include "Data_Manager.h"
#include "UI_Manager.h"

CUI_MainMenuList::CUI_MainMenuList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_MainMenuList::CUI_MainMenuList(const CUI_MainMenuList& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MainMenuList::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_MainMenuList::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	m_bButtonUI = true;

	if (m_tUIInfo.strUIName == "GAMEPLAY")
	{
		m_bActive = true;
		m_bPlayAnim = true;
	}
	else if (m_tUIInfo.strUIName == "INTRO")
		m_bActive = false;
	else if (m_tUIInfo.strUIName == "INTROBOSS")
		m_bActive = false;
	else if (m_tUIInfo.strUIName == "SNOWMOUNTAIN")
		m_bActive = false;
	else if (m_tUIInfo.strUIName == "SNOWMOUNTAINBOSS")
		m_bActive = false;
	else if (m_tUIInfo.strUIName == "TOOL")
	{
		m_bActive = true;
		m_bPlayAnim = true;
	}
	else if (m_tUIInfo.strUIName == "EXIT")
	{
		m_bActive = true;
		m_bPlayAnim = true;
	}

	return S_OK;
}

void CUI_MainMenuList::Priority_Tick(_float fTimeDelta)
{

}

void CUI_MainMenuList::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bActive == true)
	{
		Check_Picking(fTimeDelta);
	}
}

void CUI_MainMenuList::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_MainMenuList::Render()
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

void CUI_MainMenuList::UI_Ready(_float fTimeDelta)
{
}

void CUI_MainMenuList::UI_Enter(_float fTimeDelta)
{
}

void CUI_MainMenuList::UI_Loop(_float fTimeDelta)
{
}

void CUI_MainMenuList::UI_Exit(_float fTimeDelta)
{
}

void CUI_MainMenuList::Check_Picking(_float fTimeDelta)
{
	if (m_bPick == true)
	{
		if (m_bSelect == true || m_bSelectPressing == true)
		{
			if (m_tUIInfo.strUIName == "GAMEPLAY")
			{
				m_pUIManager->Active_LevelList();
				m_pUIManager->NonActive_MainList();
				m_bPick = false;
				m_bSelect = false;
				m_bSelectPressing = false;
			}
			else if (m_tUIInfo.strUIName == "INTRO")
			{
				m_pData_Manager->Set_SelectLevel(LEVEL_INTRO);
			}
			else if (m_tUIInfo.strUIName == "INTROBOSS")
			{
				m_pData_Manager->Set_SelectLevel(LEVEL_INTRO_BOSS);
			}
			else if (m_tUIInfo.strUIName == "SNOWMOUNTAIN")
			{
				m_pData_Manager->Set_SelectLevel(LEVEL_SNOWMOUNTAIN);
			}
			else if (m_tUIInfo.strUIName == "SNOWMOUNTAINBOSS")
			{
				m_pData_Manager->Set_SelectLevel(LEVEL_SNOWMOUNTAINBOSS);
			}
			else if (m_tUIInfo.strUIName == "TOOL")
			{
				m_pData_Manager->Set_SelectLevel(LEVEL_TOOL);
			}
			else if (m_tUIInfo.strUIName == "EXIT")
			{
				//g_CloseWindow = true; // Close Window
			}
		}
	}
}

HRESULT CUI_MainMenuList::Ready_Components()
{
	_uint iLevel = 0;

	if (m_pGameInstance->Get_NextLevel() == (_uint)LEVEL_TOOL)
		iLevel = (_uint)LEVEL_TOOL;
	if (m_pGameInstance->Get_NextLevel() == (_uint)LEVEL_LOGO)
		iLevel = (_uint)LEVEL_LOGO;

	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

#pragma region
	//! For.Com_Texture2 // 
	if (FAILED(__super::Add_Component(iLevel, TEXT("MainMenu_GamePlay"),
		TEXT("Com_Texture_MainMenu_GamePlay"), reinterpret_cast<CComponent**>(&m_pTextureCom[GAMEPLAY]))))
		return E_FAIL;

	//! For.Com_Texture2 // 
	if (FAILED(__super::Add_Component(iLevel, TEXT("MainMenu_Intro"),
		TEXT("Com_Texture_MainMenu_Intro"), reinterpret_cast<CComponent**>(&m_pTextureCom[INTRO]))))
		return E_FAIL;

	//! For.Com_Texture2 // 
	if (FAILED(__super::Add_Component(iLevel, TEXT("MainMenu_IntroBoss"),
		TEXT("Com_Texture_MainMenu_IntroBoss"), reinterpret_cast<CComponent**>(&m_pTextureCom[INTRO_BOSS]))))
		return E_FAIL;

	//! For.Com_Texture2 // 
	if (FAILED(__super::Add_Component(iLevel, TEXT("MainMenu_SnowMountain"),
		TEXT("Com_Texture_MainMenu_SnowMountain"), reinterpret_cast<CComponent**>(&m_pTextureCom[SNOWMOUNTAIN]))))
		return E_FAIL;

	//! For.Com_Texture2 // 
	if (FAILED(__super::Add_Component(iLevel, TEXT("MainMenu_SnowMountainBoss"),
		TEXT("Com_Texture_MainMenu_SnowMountainBoss"), reinterpret_cast<CComponent**>(&m_pTextureCom[SNOWMOUNTAIN_BOSS]))))
		return E_FAIL;
	
	//! For.Com_Texture2 // 
	if (FAILED(__super::Add_Component(iLevel, TEXT("MainMenu_Tool"),
		TEXT("Com_Texture_MainMenu_Tool"), reinterpret_cast<CComponent**>(&m_pTextureCom[TOOL]))))
		return E_FAIL;

	//! For.Com_Texture2 // 
	if (FAILED(__super::Add_Component(iLevel, TEXT("MainMenu_Exit"),
		TEXT("Com_Texture_MainMenu_Exit"), reinterpret_cast<CComponent**>(&m_pTextureCom[EXIT]))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CUI_MainMenuList::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	/* �� ��ü�� �ְ��Ǵ� �༮�̴�, ������ ���� ������� ��ŷ�� �����ϰ�, �� ��ü�� ��ŷ�ƴ��� �˾ƾ��ϴ� ��ü�� �Ŵ����� ���� �˰�����. */
	if (m_tUIInfo.strUIName == "GAMEPLAY") // ���õ� �༮�� ������ ���� �̸��� ���ؼ� ������Ѵ�.
	{
		if (FAILED(m_pTextureCom[GAMEPLAY]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else if (m_tUIInfo.strUIName == "INTRO")
	{
		if (FAILED(m_pTextureCom[INTRO]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else if (m_tUIInfo.strUIName == "INTROBOSS")
	{
		if (FAILED(m_pTextureCom[INTRO_BOSS]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else if (m_tUIInfo.strUIName == "SNOWMOUNTAIN")
	{
		if (FAILED(m_pTextureCom[SNOWMOUNTAIN]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else if (m_tUIInfo.strUIName == "SNOWMOUNTAINBOSS")
	{
		if (FAILED(m_pTextureCom[SNOWMOUNTAIN_BOSS]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else if (m_tUIInfo.strUIName == "TOOL")
	{
		if (FAILED(m_pTextureCom[TOOL]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else if (m_tUIInfo.strUIName == "EXIT")
	{
		if (FAILED(m_pTextureCom[EXIT]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	
	return S_OK;
}

json CUI_MainMenuList::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_MainMenuList::Load_Desc()
{

}

CUI_MainMenuList* CUI_MainMenuList::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_MainMenuList* pInstance = new CUI_MainMenuList(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_MainMenuList");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_MainMenuList::Pool()
{
	return new CUI_MainMenuList(*this);
}

CGameObject* CUI_MainMenuList::Clone(void* pArg)
{
	CUI_MainMenuList* pInstance = new CUI_MainMenuList(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_MainMenuList");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_MainMenuList::Free()
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
