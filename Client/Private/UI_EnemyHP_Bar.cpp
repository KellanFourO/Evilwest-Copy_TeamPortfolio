#include "stdafx.h"
#include "UI_EnemyHP_Bar.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Texture.h"
#include "Character.h"

CUI_EnemyHP_Bar::CUI_EnemyHP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_EnemyHP_Bar::CUI_EnemyHP_Bar(const CUI_EnemyHP_Bar& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_EnemyHP_Bar::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_EnemyHP_Bar::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fScaleX = 169.f;
	m_tUIInfo.fScaleY = 17.f;

	m_tUIInfo.fPositionZ = 0.0f;

	m_iShaderNum = 1;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	/*
	#include "Data_Manager.h" ��Ŭ��� ���ְ�,
	CData_Manager::GetInstance()->Get_Player(); �� �޾��ְ�
	�Լ� �״�� ����ϰų�, ������ ��Ƽ� ����ϱ�.
	�ش� ��ü�� ���ϴ� �Լ��� ���� ���� �ҷ�����.
	*/

	// Setting_Owner(); ���� �� �����̴ϱ� ���� �ϴ°����� �Լ� ȣ���ؼ� �������ֱ�
	m_bActive = true;

	return S_OK;
}

void CUI_EnemyHP_Bar::Priority_Tick(_float fTimeDelta)
{

}

void CUI_EnemyHP_Bar::Tick(_float fTimeDelta)
{
	/* Owner HP */
	if(m_pCharacterOwner != nullptr)
		m_fCurHP = m_pCharacterOwner->Get_CurHP();

	__super::Tick(fTimeDelta);

	// 0. ȸ��
	if (m_fPreHP < m_fCurHP)
		m_fPreHP = m_fCurHP;

	/* 1. ��� ü�� ���� */
	if (m_fCurHP <= 0.f)
	{
		/* Dead */
		m_fCurHP = 0.f;
		Set_Dead(true);
	}

	/* 2. ���� ü�� -> �ƽ� ü�� �ʰ� */
	if (m_fCurHP >= m_fMaxHP)
	{
		m_fCurHP = m_fMaxHP;
	}

	if (m_bActive == true)
	{
		m_fTimeAcc += fTimeDelta * 0.1f;

		if (m_fCurHP < m_fPreHP)
			m_bLerp = false;

		if (!m_bLerp && m_fPreHP > m_fCurHP)
		{
			m_fPreHP -= fTimeDelta * m_fVariationSpeed * (m_fMaxHP / 6.f);

			if (m_fPreHP <= m_fCurHP)
			{
				m_fPreHP = m_fCurHP;
				m_bLerp = true;
			}
		}
	}

}

void CUI_EnemyHP_Bar::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
		return;
}

HRESULT CUI_EnemyHP_Bar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//! �� ���̴��� 0��° �н��� �׸���.
	m_pShaderCom->Begin(m_iShaderNum); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

	//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
	m_pVIBufferCom->Bind_VIBuffers();

	//! ���ε��� ����, �ε����� �׷�
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_EnemyHP_Bar::UI_Ready(_float fTimeDelta)
{
}

void CUI_EnemyHP_Bar::UI_Enter(_float fTimeDelta)
{
}

void CUI_EnemyHP_Bar::UI_Loop(_float fTimeDelta)
{
}

void CUI_EnemyHP_Bar::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_EnemyHP_Bar::Ready_Components()
{
	//! For.Com_Texture1 // ��� ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_defence_white"),
		TEXT("Com_Texture_WhiteBar"), reinterpret_cast<CComponent**>(&m_pTextureCom[HPBAR_WHITE]))))
		return E_FAIL;

	//! For.Com_Texture2 // ������ ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_boss_hp_bar"),
		TEXT("Com_Texture_RedBar"), reinterpret_cast<CComponent**>(&m_pTextureCom[HPBAR_RED]))))
		return E_FAIL;

	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* ȿ���� �ʿ��� �༮�� Map�ؽ��ĵ� �߰����ֱ� */
	return S_OK;
}

HRESULT CUI_EnemyHP_Bar::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_LerpHP", &m_fPreHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_CurrentHP", &m_fCurHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_MaxHP", &m_fMaxHP, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pTextureCom[HPBAR_WHITE]->Bind_ShaderResource(m_pShaderCom, "g_HpBarWhite_Texture")))	// Hp White
		return E_FAIL;
	if (FAILED(m_pTextureCom[HPBAR_RED]->Bind_ShaderResource(m_pShaderCom, "g_HpBarRed_Texture")))		// Hp Red
		return E_FAIL;
	//if (FAILED(m_pTextureCom[HP_DECAL]->Bind_ShaderResource(m_pShaderCom, "g_HpBarDecal_Texture")))		// Hp Decal
	//	return E_FAIL;

	return S_OK;
}

void CUI_EnemyHP_Bar::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

HRESULT CUI_EnemyHP_Bar::Setting_Owner()
{
	if (m_pOwner != nullptr) // Owner ��������� �մϴ�.
	{
		m_pCharacterOwner = dynamic_cast<CCharacter*>(m_pOwner);

		m_fCurHP = m_pCharacterOwner->Get_CurHP();
		m_fPreHP = m_fCurHP;
		m_fMaxHP = m_pCharacterOwner->Get_MaxHP();
	}
	else
	{
		m_fCurHP = 100.f;
		m_fPreHP = 100.f;
		m_fMaxHP = 100.f;
	}

	return S_OK;
}

_bool CUI_EnemyHP_Bar::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_EnemyHP_Bar::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);


	/* �߰����� ���� */


	return out_json;
}

void CUI_EnemyHP_Bar::Load_Desc()
{

}

CUI_EnemyHP_Bar* CUI_EnemyHP_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_EnemyHP_Bar* pInstance = new CUI_EnemyHP_Bar(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_EnemyHP_Bar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_EnemyHP_Bar::Clone(void* pArg)
{
	CUI_EnemyHP_Bar* pInstance = new CUI_EnemyHP_Bar(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_EnemyHP_Bar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_EnemyHP_Bar::Pool()
{
	return new CUI_EnemyHP_Bar(*this);
}

void CUI_EnemyHP_Bar::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
