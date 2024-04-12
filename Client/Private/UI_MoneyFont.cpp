#include "stdafx.h"
#include "UI_MoneyFont.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Data_Manager.h"

CUI_MoneyFont::CUI_MoneyFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CUI_Text(pDevice, pContext, strPrototypeTag)
{

}

CUI_MoneyFont::CUI_MoneyFont(const CUI_MoneyFont& rhs)
	: CUI_Text(rhs)
{
}

HRESULT CUI_MoneyFont::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_MoneyFont::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fPositionZ = 0.1f;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	if (FAILED(Ready_Text())) //! �ؽ�Ʈ ����
		return E_FAIL;

	if (FAILED(Find_Change("Level"))) //! �ؽ�Ʈ ����
		return E_FAIL;

	m_fScaleX = m_tUIInfo.fScaleX;
	m_fScaleY = m_tUIInfo.fScaleY;

	m_eType = UITYPE::LEVEL_UP;
	m_bActive = false;

	//m_strText = m_pData_Manager->Get_CurLevel();

	//m_fChangeScale = 4.f;
	m_fAlpha = 0.f;
	//m_fLifeTime = 8000.f;
	//m_fTime = (_float)GetTickCount64();

	m_strFontTag = L"Font_EvilWest";

	return S_OK;
}

void CUI_MoneyFont::Priority_Tick(_float fTimeDelta)
{

}

void CUI_MoneyFont::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Check_Disappear(fTimeDelta);

	if (m_bActive == true)
	{

	}
}

void CUI_MoneyFont::Late_Tick(_float fTimeDelta)
{
	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_MoneyFont::Render()
{
	if (m_bActive == true)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		//! �� ���̴��� 0��° �н��� �׸��ž�.
		m_pShaderCom->Begin(0); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

		////! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
		//m_pVIBufferCom->Bind_VIBuffers();

		////! ���ε��� ����, �ε����� �׷�
		//m_pVIBufferCom->Render();

		//if (m_bTrigger == true)
		{
			if (m_tUIInfo.strUIName == "SkillPoint")
			{
				m_strText = to_wstring(m_pData_Manager->Get_SkillPoint());
				m_fScale = 0.6f;
				m_fOffsetX = -10.f;
				m_fOffsetY = -30.f;
			}

			if (m_tUIInfo.strUIName == "Money")
			{
				m_strText = to_wstring(m_pData_Manager->Get_Money());
				m_fScale = 0.6f;
				m_fOffsetX = -15.f;
				m_fOffsetY = -35.f;
			}


			//m_strText = L"Test";
			//RenderTextWithLineBreak(m_pGameInstance->Convert_WString_To_String(m_strText), 10);
			m_pGameInstance->Render_Font(m_strFontTag, m_strText, _float2((m_pTransformCom->Get_Position().x + (_float)g_iWinSizeX * 0.5f) - 10.f, (-m_pTransformCom->Get_Position().y + (_float)g_iWinSizeY * 0.5f) - 30.f), m_vColor, m_fScale, m_vOrigin, m_fRotation);
		}
	}

	return S_OK;
}

void CUI_MoneyFont::UI_Ready(_float fTimeDelta)
{
}

void CUI_MoneyFont::UI_Enter(_float fTimeDelta)
{
}

void CUI_MoneyFont::UI_Loop(_float fTimeDelta)
{
}

void CUI_MoneyFont::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_MoneyFont::Ready_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("level_shield"),
		TEXT("Com_Texture_LeShield"), reinterpret_cast<CComponent**>(&m_pTextureCom[SHIELD_FRAME]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MoneyFont::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	//if (FAILED(m_pTextureCom[SHIELD_FRAME]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
	//	return E_FAIL;

	return S_OK;
}

json CUI_MoneyFont::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_MoneyFont::Load_Desc()
{

}

void CUI_MoneyFont::Add_Text(string strTextKey, string strFontTag, string strText, _float fPosX, _float fPosY, _vector vColor, _float fScale, _float2 vOrigin, _float fRotation)
{
	__super::Add_Text(strTextKey, strFontTag, strText, fPosX, fPosY, vColor, fScale, vOrigin, fRotation);
}

HRESULT CUI_MoneyFont::Find_Change(const string& strTextTag)
{
	__super::Find_Change(strTextTag);
	return S_OK;
}

void CUI_MoneyFont::Check_Disappear(_float fTimeDelta)
{
	if (m_bDisappear == true)
	{
		m_bActive = Alpha_Plus(fTimeDelta);
	}
}

HRESULT CUI_MoneyFont::Ready_Text()
{
	/* �Ľ� ���� �ޱ� */
	TEXTINFO* LoadInfo = new TEXTINFO;

	/* ���� �� (�� �� �ε��ؼ� �ޱ�) */
	LoadInfo->fPosX = 625.5f;
	LoadInfo->fPosY = 176.f;
	LoadInfo->fScale = 0.5f;
	LoadInfo->vOrigin.x = 0.f;
	LoadInfo->vOrigin.y = 0.f;
	LoadInfo->fRotation = 0.f;
	LoadInfo->strTextKey = "PlayerLevel";
	LoadInfo->strFontTag = "Font_EvilWest";
	LoadInfo->strText = to_string(1);
	LoadInfo->vColor.m128_f32[0] = 1.f;
	LoadInfo->vColor.m128_f32[1] = 1.f;
	LoadInfo->vColor.m128_f32[2] = 1.f;
	LoadInfo->vColor.m128_f32[3] = 1.f;

	m_mapText.emplace(LoadInfo->strTextKey, LoadInfo);

	TEXTINFO* pTextInfo = nullptr;

	return S_OK;
}

CUI_MoneyFont* CUI_MoneyFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_MoneyFont* pInstance = new CUI_MoneyFont(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_MoneyFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_MoneyFont::Pool()
{
	return new CUI_MoneyFont(*this);
}

CGameObject* CUI_MoneyFont::Clone(void* pArg)
{
	CUI_MoneyFont* pInstance = new CUI_MoneyFont(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_MoneyFont");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_MoneyFont::Free()
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
