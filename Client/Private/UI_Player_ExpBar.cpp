#include "stdafx.h"
#include "UI_Player_ExpBar.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Data_Manager.h"

CUI_Player_ExpBar::CUI_Player_ExpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Player_ExpBar::CUI_Player_ExpBar(const CUI_Player_ExpBar& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_ExpBar::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Player_ExpBar::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fScaleX = 169.f;
	m_tUIInfo.fScaleY = 4.f;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	Check_RectPos();
	m_fOrigin_Right = (_float)m_rcUI.right;

	Set_OwnerExp();

	m_iShaderNum = 2;

	m_pDataManager = CData_Manager::GetInstance();

	return S_OK;
}

void CUI_Player_ExpBar::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Player_ExpBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	if (m_pGameInstance->Key_Down(DIK_P))
	{
		m_pDataManager->Add_CurEXP(10.f);
	}

	if (m_pGameInstance->Key_Down(DIK_O))
	{
		m_pDataManager->Add_CurEXP(100.f);
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
			m_iShaderNum = 2;																	// ���̴� �н� �ִ� ��ȣ ���� (���߿� ����)
	}

	if (m_fPreEXP > m_pDataManager->Get_MaxEXP())
	{
		m_pDataManager->Add_CurLevel(1);
		
		m_fPreEXP = m_pDataManager->Get_CurEXP();
	}

	// Level������ ���� ���
	if (m_iPreLevel == m_pDataManager->Get_CurLevel()/*m_pPlayer->Get_Stat().iLevel*/)
	{
		// �ð� ��ø
		m_fTimeAcc += fTimeDelta * 0.5f;
		// ���� Exp�� �޾ƿ´�. (Ű�Է����� Test��)
		//m_fCurEXP = m_fTestPlusEXP/*_float(m_pPlayer->Get_Stat().iExp)*/;

		// ���� EXP�� PreEXP���� ��������
		if (m_pDataManager->Get_CurEXP() > m_fPreEXP)
			m_bLerp = false;	// ���� ����

		// CurEXP�� PreEXP���� Ŀ���� �������� �������� ���
		if (false == m_bLerp && m_fPreEXP < m_pDataManager->Get_CurEXP())
		{
			// �� ���̸� Ratio�� ��� (0~1)
			_float m_fRatio = m_pDataManager->Get_CurEXP() / m_pDataManager->Get_MaxEXP();

			// PreEXP�� ��ø�� �ð��� �����ش�.
			m_fPreEXP += m_fTimeAcc;

			// �� ���̸� PreEXP�� ��� (0~1)
			m_fProgress = m_fPreEXP / m_pDataManager->Get_MaxEXP();

			// =>PreEXP�� Cur�� ���ų� Ŀ���ٸ� ����ؼ�, ���� EXP�� �� ���̸� ���� �� ���. =>PreEXP�� Cur���� ���� ���ٸ� PreEXP�� �� ���̸� ���
			if (m_fPreEXP >= m_pDataManager->Get_CurEXP())
			{
				m_fTimeAcc = 0.f; // ���� �ð��� �ʱ�ȭ (PreEXP�� CurEXP���� ������������ �����Ǵ� ��)
				m_fPreEXP = m_pDataManager->Get_CurEXP();	// PreEXP�� CurEXP�� �����ش�.
				m_fProgress = m_fRatio; // �� ���̸� CurEXP�� �����ش�.
				m_bLerp = true;	// Lerp�� �ٽ� ���ش�.
			}
		}
	}
	else if (m_iPreLevel < m_pDataManager->Get_CurLevel()/*m_pPlayer->Get_Stat().iLevel*/)
	{
		// EXPBar�� �����ߴ� Level�� Player�� ���� ������ �ٸ� �� (���� ��)
		m_fTimeAcc += fTimeDelta * 0.5f;
		m_fPreEXP += m_fTimeAcc;

		if (m_fPreEXP >= m_pDataManager->Get_MaxEXP())
		{
			m_fRemainEXP = m_fPreEXP - m_pDataManager->Get_MaxEXP(); // �������� �ʰ��� �� ����
			m_fPreEXP = m_pDataManager->Get_MaxEXP();
		}

		m_fProgress = m_fPreEXP / m_pDataManager->Get_MaxEXP();

		if (m_fProgress >= 1.f) // �ٰ� �� ���� ������ ����
		{
			m_iPreLevel = m_pDataManager->Get_CurLevel();/*m_pPlayer->Get_Stat().iLevel*/;
			m_pDataManager->NextLevl_MaxEXP(); /*m_pPlayer->Get_Stat().iMaxExp*/;
			m_pDataManager->Set_CurEXP(m_fRemainEXP);/*m_pPlayer->Get_Stat().iExp*/; // �������ϰ� ���� ��
			m_fPreEXP = 0.f; // PreEXP�� �ʱ�ȭ
			m_pData_Manager->Set_ShowLevelBox(true);

			m_fTimeAcc = 0.f;
			m_bLerp = true;
		}
	}
}

void CUI_Player_ExpBar::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();


	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_Player_ExpBar::Render()
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

HRESULT CUI_Player_ExpBar::Ready_Components()
{
	FAILED_CHECK(__super::Ready_Components()); // Ready : Texture / MapTexture

	//! For.Com_Texture1 // ����ġ ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_exp_bar"),
		TEXT("Com_Texture_ExpBar"), reinterpret_cast<CComponent**>(&m_pTextureCom[EXPBAR]))))
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

HRESULT CUI_Player_ExpBar::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[EXPBAR]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_LoadingProgress", &m_fProgress, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CUI_Player_ExpBar::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_Player_ExpBar::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

void CUI_Player_ExpBar::Set_OwnerExp()
{
	m_fMaxEXP = 100.f;
	m_fCurEXP = 00.f;
	m_fPreEXP = m_fCurEXP;
	m_fProgress = 0.f;
}

json CUI_Player_ExpBar::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);


	/* �߰����� ���� */


	return out_json;
}

void CUI_Player_ExpBar::Load_Desc()
{

}

CUI_Player_ExpBar* CUI_Player_ExpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Player_ExpBar* pInstance = new CUI_Player_ExpBar(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_ExpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_ExpBar::Clone(void* pArg)
{
	CUI_Player_ExpBar* pInstance = new CUI_Player_ExpBar(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_ExpBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_ExpBar::Pool()
{
	return new CUI_Player_ExpBar(*this);
}

void CUI_Player_ExpBar::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
