#include "stdafx.h"
#include "UI_Player_ExpBar.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Data_Manager.h"
#include "UI_Manager.h"
#include "Player.h"

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

	/* ���� �ر� */
	//m_pUIManager->Set_WeaponLevel("Revolver", 1);         // ������

	return S_OK;
}

void CUI_Player_ExpBar::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Player_ExpBar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bActive)
	{
		if (m_pGameInstance->Key_Down(DIK_M))
		{
			m_pData_Manager->Add_CurEXP(50.f);
		}

		if (m_fPreEXP > m_pData_Manager->Get_MaxEXP())
		{
			m_pData_Manager->Add_CurLevel(1);

			m_fPreEXP = m_pData_Manager->Get_CurEXP();
		}

		// Level������ ���� ���
		if (m_iPreLevel == m_pData_Manager->Get_CurLevel()/*m_pPlayer->Get_Stat().iLevel*/)
		{
			// �ð� ��ø
			m_fTimeAcc += fTimeDelta * 0.5f;
			// ���� Exp�� �޾ƿ´�. (Ű�Է����� Test��)
			//m_fCurEXP = m_fTestPlusEXP/*_float(m_pPlayer->Get_Stat().iExp)*/;

			// ���� EXP�� PreEXP���� ��������
			if (m_pData_Manager->Get_CurEXP() > m_fPreEXP)
				m_bLerp = false;	// ���� ����

			// CurEXP�� PreEXP���� Ŀ���� �������� �������� ���
			if (false == m_bLerp && m_fPreEXP < m_pData_Manager->Get_CurEXP())
			{
				// �� ���̸� Ratio�� ��� (0~1)
				_float m_fRatio = m_pData_Manager->Get_CurEXP() / m_pData_Manager->Get_MaxEXP();

				// PreEXP�� ��ø�� �ð��� �����ش�.
				m_fPreEXP += m_fTimeAcc;

				// �� ���̸� PreEXP�� ��� (0~1)
				m_fProgress = m_fPreEXP / m_pData_Manager->Get_MaxEXP();

				// =>PreEXP�� Cur�� ���ų� Ŀ���ٸ� ����ؼ�, ���� EXP�� �� ���̸� ���� �� ���. =>PreEXP�� Cur���� ���� ���ٸ� PreEXP�� �� ���̸� ���
				if (m_fPreEXP >= m_pData_Manager->Get_CurEXP())
				{
					m_fTimeAcc = 0.f; // ���� �ð��� �ʱ�ȭ (PreEXP�� CurEXP���� ������������ �����Ǵ� ��)
					m_fPreEXP = m_pData_Manager->Get_CurEXP();	// PreEXP�� CurEXP�� �����ش�.
					m_fProgress = m_fRatio; // �� ���̸� CurEXP�� �����ش�.
					m_bLerp = true;	// Lerp�� �ٽ� ���ش�.
				}
			}
		}
		else if (m_iPreLevel < m_pData_Manager->Get_CurLevel()/*m_pPlayer->Get_Stat().iLevel*/)
		{
			// EXPBar�� �����ߴ� Level�� Player�� ���� ������ �ٸ� �� (���� ��)
			m_fTimeAcc += fTimeDelta * 0.5f;
			m_fPreEXP += m_fTimeAcc;

			if (m_fPreEXP >= m_pData_Manager->Get_MaxEXP())
			{
				m_fRemainEXP = m_fPreEXP - m_pData_Manager->Get_MaxEXP(); // �������� �ʰ��� �� ����
				m_fPreEXP = m_pData_Manager->Get_MaxEXP();
			}

			m_fProgress = m_fPreEXP / m_pData_Manager->Get_MaxEXP();

			if (m_fProgress >= 1.f) // �ٰ� �� ���� ������ ����
			{
				m_iPreLevel = m_pData_Manager->Get_CurLevel();/*m_pPlayer->Get_Stat().iLevel*/;
				m_pData_Manager->NextLevl_MaxEXP(); /*m_pPlayer->Get_Stat().iMaxExp*/;
				m_pData_Manager->Set_CurEXP(m_fRemainEXP);/*m_pPlayer->Get_Stat().iExp*/; // �������ϰ� ���� ��
				m_fPreEXP = 0.f; // PreEXP�� �ʱ�ȭ

				//// ��� ���� �ȳ��޼���
				//{
				//	m_pUIManager->Change_TutorialText(TUTORIAL_TEXT::REWARD); // �޼��� ����
				//	m_pUIManager->Active_TutorialBox(); // Active Ȱ��ȭ
				//}

				// ������ ����
				{
					m_pData_Manager->Add_Money(1000);
					m_pData_Manager->Add_SkillPoint(3);

					// �⺻ ����
					/*if (m_tUIInfo.strUIName == "Kick")
					{
					   m_eUI_PreLevel = m_eUI_Level;
					}
					else if (m_tUIInfo.strUIName == "UpperCut")
					{
					   m_eUI_PreLevel = m_eUI_Level;
					}
					else if (m_tUIInfo.strUIName == "Heal")
					{
					   m_eUI_PreLevel = m_eUI_Level;
					}
					else if (m_tUIInfo.strUIName == "Punch")
					{
					   m_eUI_PreLevel = m_eUI_Level;
					}*/

					if (m_pData_Manager->Get_CurLevel() == 2)
					{
						/* ��ų �ر� */
						m_pUIManager->Set_SkillLevel("OneTouch", 1);      // 1��ġ
						m_pUIManager->Set_SkillLevel("ElectricDash", 1);   // �뽬
						m_pUIManager->Set_SkillLevel("SuperChargeMod", 1);

						/* ���� �ر� */
						m_pUIManager->Set_WeaponLevel("Rifle_Skill1", 1);      // ������ ��ų
					}

					if (m_pData_Manager->Get_CurLevel() == 3)
					{
						/* ��ų �ر� */
						m_pUIManager->Set_SkillLevel("TwoTouch", 1);      // 2��ġ
						m_pUIManager->Set_SkillLevel("DashShock", 1);      // �뽬���� ȿ��
						m_pUIManager->Set_SkillLevel("TeleportPunch", 1);
						m_pUIManager->Set_SkillLevel("ElectricCord", 1);   // ������ ����
						/* ��ų �ر� */

						/* ���� �ر� */
						m_pUIManager->Set_WeaponLevel("Revolver_Skill1", 1);   // ������ ��ų
					}

					if (m_pData_Manager->Get_CurLevel() == 4)
					{
						/* ��ų �ر� */
						m_pUIManager->Set_SkillLevel("ThreeTouch", 1);      // 3��ġ
						m_pUIManager->Set_SkillLevel("ComboPunch", 1);      // �޺���ġ
						m_pUIManager->Set_SkillLevel("NPCPowerUP", 1);
						m_pUIManager->Set_SkillLevel("IncreaseEXP", 1);

					}

					if (m_pData_Manager->Get_CurLevel() == 5)
					{

						/* ��ų �ر� */
						m_pUIManager->Set_SkillLevel("IncreaseHP", 1);
						m_pUIManager->Set_SkillLevel("IncreaseEnergy", 1);
						m_pUIManager->Set_SkillLevel("RecoveryEnergy", 1);
						m_pUIManager->Set_SkillLevel("MaxHP", 1);

						/* ���� �ر� */
						m_pUIManager->Set_WeaponLevel("Shotgun", 1);         // ����
						/* ���⽺ų �ر� */
						m_pUIManager->Set_WeaponLevel("Shotgun_Skill1", 1);      // ���� ��ų
					}

					if (m_pData_Manager->Get_CurLevel() == 6)
					{
						/* ��ų �ر� */

					}

					if (m_pData_Manager->Get_CurLevel() == 7)
					{

					}

					if (m_pData_Manager->Get_CurLevel() == 8)
					{
						/* ��ų �ر� */
					}

					if (m_pData_Manager->Get_CurLevel() == 9)
					{

					}

					//m_pData_Manager->Set_ShowLevelBox(true);
					m_pUIManager->Active_LevelUp();
					m_fTimeAcc = 0.f;
					m_bLerp = true;
				}
			}
		}
	}
}

void CUI_Player_ExpBar::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	if (m_bActive)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_Player_ExpBar::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		//! �� ���̴��� 0��° �н��� �׸���.
		m_pShaderCom->Begin(m_iShaderNum); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

		//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
		m_pVIBufferCom->Bind_VIBuffers();

		//! ���ε��� ����, �ε����� �׷�
		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_Player_ExpBar::UI_Ready(_float fTimeDelta)
{
}

void CUI_Player_ExpBar::UI_Enter(_float fTimeDelta)
{
}

void CUI_Player_ExpBar::UI_Loop(_float fTimeDelta)
{
}

void CUI_Player_ExpBar::UI_Exit(_float fTimeDelta)
{
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
