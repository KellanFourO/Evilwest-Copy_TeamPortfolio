#include "stdafx.h"
#include "UI_Player_HPBar.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_Player_HPBar::CUI_Player_HPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Player_HPBar::CUI_Player_HPBar(const CUI_Player_HPBar& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_HPBar::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Player_HPBar::Initialize(void* pArg)
{
	if(pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fScaleX = 169.f;
	m_tUIInfo.fScaleY = 17.f;

	m_tUIInfo.fPositionZ = 0.0f;

	m_iShaderNum = 0;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	Set_OwnerHp();

	return S_OK;
}

void CUI_Player_HPBar::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Player_HPBar::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_Z))
	{
		m_bActive = true;
	}

	if (m_pGameInstance->Key_Down(DIK_Q)) // �ǰ�
	{
		m_fCurHP -= 10.f;

		if (m_fCurHP <= 0.f)
		{
			/* Dead */
			m_fCurHP = 0.f;
		}
	}

	if (m_pGameInstance->Key_Down(DIK_E)) // ȸ��
	{
		m_fCurHP += 10.f;
		m_fPreHP += 10.f;

		if (m_fCurHP >= m_fMaxHP)
		{
			m_fCurHP = m_fMaxHP;
			m_fPreHP = m_fCurHP;
		}
	}

	if (m_pGameInstance->Key_Down(DIK_PGDN))
	{
		//Change_SizeRight(-5.f);
		--m_iShaderNum;
		if (m_iShaderNum <= 0)
			m_iShaderNum = 0;
	}

	if (m_pGameInstance->Key_Down(DIK_PGUP))
	{
		//Change_SizeRight(5.f);
		++m_iShaderNum;
		if (m_iShaderNum >= 3)
			m_iShaderNum = 3;																	// ���̴� �н� �ִ� ��ȣ ���� (���߿� ����)
	}

	if (m_bActive)
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

		__super::Tick(fTimeDelta);
	}
}

void CUI_Player_HPBar::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();


	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_Player_HPBar::Render()
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

HRESULT CUI_Player_HPBar::Ready_Components()
{
	if(FAILED(__super::Ready_Components())); // Ready : Texture / MapTexture

	//! For.Com_Texture1 // ��� ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_health_bar_damagel"),
		TEXT("Com_Texture_WhiteBar"), reinterpret_cast<CComponent**>(&m_pTextureCom[HPBAR_WHITE]))))
		return E_FAIL;

	//! For.Com_Texture2 // ������ ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_health_bar_bg_full"),
		TEXT("Com_Texture_RedBar"), reinterpret_cast<CComponent**>(&m_pTextureCom[HPBAR_RED]))))
		return E_FAIL;

	//! For.Com_Texture3 // ǥ�ü�
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("UI_PlayersHP_decal"),
		TEXT("Com_Texture_Decal"), reinterpret_cast<CComponent**>(&m_pTextureCom[HP_DECAL]))))
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

HRESULT CUI_Player_HPBar::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
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
	if (FAILED(m_pTextureCom[HP_DECAL]->Bind_ShaderResource(m_pShaderCom, "g_HpBarDecal_Texture")))		// Hp Decal
		return E_FAIL;

	//string TestName = m_tUIInfo.strObjectName;
	//for (_int i = (_int)0; i < (_int)TEXTURE_END; ++i)
	//{
	//	switch (i)
	//	{
	//	case CUI_Player_HPBar::HPBAR_WHITE:
	//	{
	//		if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
	//			return E_FAIL;
	//		break;
	//	}
	//	case CUI_Player_HPBar::HPBAR_RED:
	//	{
	//		if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture_Second")))
	//			return E_FAIL;

	//		break;
	//	}
	//	case CUI_Player_HPBar::HP_DECAL:
	//	{
	//		if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture_Third")))
	//			return E_FAIL;
	//		break;
	//	}
	//	case CUI_Player_HPBar::TEXTURE_END:
	//		break;
	//	default:
	//		break;
	//	}
	//}

	return S_OK;
}

void CUI_Player_HPBar::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_Player_HPBar::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

void CUI_Player_HPBar::Set_OwnerHp(/*CPlayer pPlayer*/)
{
	// ex)
	//if (nullptr == pPlayer)
	//	return;

	//m_pOwner = pPlayer;

	//CPlayer::PLAYER_STAT StatDesc = {};
	//ZeroMemory(&StatDesc, sizeof(CPlayer::PLAYER_STAT));

	//memcpy(&StatDesc, &(m_pOwner->Get_Stat()), sizeof(CPlayer::PLAYER_STAT));

	//// Texture ���� ����
	//if (TEXT("Texture Name1") == m_pOwner->Get_ObjectTag())
	//{

	//}
	//else if (TEXT("Texture Name2") == m_pOwner->Get_ObjectTag())
	//{

	//}
	//else if (TEXT("Texture Name3") == m_pOwner->Get_ObjectTag())
	//{

	//}

	//m_fMaxHP = StatDesc.fMaxHp;
	//m_fCurHP = StatDesc.fHp;
	//m_fPreHP = m_fCurHP;


	m_fMaxHP = 100.0f;
	m_fCurHP = m_fMaxHP;
	m_fPreHP = m_fCurHP;
}

json CUI_Player_HPBar::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	
	/* �߰����� ���� */


	return out_json;
}

void CUI_Player_HPBar::Load_Desc()
{

}

CUI_Player_HPBar* CUI_Player_HPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Player_HPBar* pInstance = new CUI_Player_HPBar(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_HPBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_HPBar::Clone(void* pArg)
{
	CUI_Player_HPBar* pInstance = new CUI_Player_HPBar(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_HPBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_HPBar::Pool()
{
	return new CUI_Player_HPBar(*this);
}

void CUI_Player_HPBar::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
