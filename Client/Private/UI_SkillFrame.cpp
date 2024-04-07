#include "stdafx.h"
#include "UI_SkillFrame.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Texture.h"
#include "UI_Manager.h"

CUI_SkillFrame::CUI_SkillFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_SkillFrame::CUI_SkillFrame(const CUI_SkillFrame& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SkillFrame::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_SkillFrame::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_SkillFrame::Priority_Tick(_float fTimeDelta)
{

}

void CUI_SkillFrame::Tick(_float fTimeDelta)
{

	__super::Tick(fTimeDelta);

	if (m_bActive == true)
	{
		if (m_pGameInstance->Key_Down(DIK_7))
			m_eUI_Level = LEVEL0;
		if (m_pGameInstance->Key_Down(DIK_8))
			m_eUI_Level = LEVEL1;
		if (m_pGameInstance->Key_Down(DIK_9))
			m_eUI_Level = LEVEL2;

		Check_Picking(fTimeDelta);
		Check_State(fTimeDelta);
		Check_LevelChange(fTimeDelta);

		m_eUI_PreLevel = m_eUI_Level;
	}


}

void CUI_SkillFrame::Late_Tick(_float fTimeDelta)
{

	if (m_bActive == true)
	{
		if (m_bRenderOut == false) // �Ⱥ��̰� �ϴ°� �������� ��� ����
		{
			if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
				return;
		}
	}
}

HRESULT CUI_SkillFrame::Render()
{
	//TODO ���̴����� ����� �����ִ� ������ �ݵ�� ���̴��� ����Լ��� ȣ���ϱ� ������ �ؾ��Ѵ�.
	//! �� ������, ���̴��� ����Լ� ������ pPass->Apply(0, m_prContext); �ڵ带 �����Ѵ�.
	//! Apply ȣ�� ��X ����� �����൵ ������ ���� ������, �������� ��������.
	//! Apply ȣ�� �Ŀ� ����� �����ָ�, � ������ ���������� ����ǰ�, � ������ ���� ����� �� �Ѿ�� ��찡 �ִ�.


	if (m_bActive == true)
	{
		if (m_bRenderOut == false) // �Ⱥ��̰� �ϴ°� �������� ��� ����
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
	}

	return S_OK;
}

void CUI_SkillFrame::UI_Ready(_float fTimeDelta)
{
}

void CUI_SkillFrame::UI_Enter(_float fTimeDelta)
{
}

void CUI_SkillFrame::UI_Loop(_float fTimeDelta)
{
}

void CUI_SkillFrame::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_SkillFrame::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	///* ù Create�� �ƹ��ų� �⺻ ���������� Initialize���� �ױ׸� �������ְ�, ���õ� �ؽ�ó�� �Ľ��ϸ� �Ľ̵� �ؽ�ó�� ����ϰ� ������. */
	//wstring strUIName;
	//m_pGameInstance->String_To_WString(m_tUIInfo.strUIName, strUIName);

	//// �κ� ���ڿ� �˻�
	//size_t found = strUIName.find(L"First");

	//// �߰ߵ� ���
	//if (found != std::wstring::npos)

	/* ù Create�� �ƹ��ų� �⺻ ���������� Initialize���� �ױ׸� �������ְ�, ���õ� �ؽ�ó�� �Ľ��ϸ� �Ľ̵� �ؽ�ó�� ����ϰ� ������. */
	wstring strUIName;
	m_pGameInstance->String_To_WString(m_tUIInfo.strProtoTag, strUIName);

	// �κ� ���ڿ� �˻�
	size_t found = strUIName.find(L"Inventory");

	// �߰ߵ� ���
	if (found != std::wstring::npos)
	{
		m_bFirstFrame = true;

		//! For.Com_Texture // ��Ȱ��ȭ
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_perks_star_slot_inactive"),
			TEXT("Com_Texture_SkillIcon_NonActive"), reinterpret_cast<CComponent**>(&m_pTextureCom[NONACTIVE]))))
			return E_FAIL;

		//! For.Com_Texture // Ȱ��ȭ
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_perks_starting_boint_slot"),
			TEXT("Com_Texture_SkillIcon_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
			return E_FAIL;
	}
	else
	{
		//! For.Com_Texture // ���
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_perks_details_locked"),
			TEXT("Com_Texture_SkillIcon_Lock"), reinterpret_cast<CComponent**>(&m_pTextureCom[LOCK]))))
			return E_FAIL;

		//! For.Com_Texture // ��Ȱ��ȭ
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_perks_details_ready"),
			TEXT("Com_Texture_SkillIcon_NonActive"), reinterpret_cast<CComponent**>(&m_pTextureCom[NONACTIVE]))))
			return E_FAIL;

		//! For.Com_Texture // Ȱ��ȭ
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_perks_details_active"),
			TEXT("Com_Texture_SkillIcon_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CUI_SkillFrame::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (m_bFirstFrame == false)
	{
		/* UI_Level */
		switch (m_eUI_Level)
		{
		case Client::CUI::LEVEL0: /* => Lock/NonActive <= */
			if (FAILED(m_pTextureCom[LOCK]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
			break;
		case Client::CUI::LEVEL1: /* => Unlock/Active <= */
			if (FAILED(m_pTextureCom[NONACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
			break;
		case Client::CUI::LEVEL2:
			if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
			break;
		case Client::CUI::LEVEL3:
			break;
		case Client::CUI::LEVEL4:
			break;
		case Client::CUI::LEVEL5:
			break;
		case Client::CUI::LEVEL_END:
			break;
		default:
			break;
		}
	}
	else
	{
		/* UI_Level */
		switch (m_eUI_Level)
		{
		case Client::CUI::LEVEL0: /* => Lock/NonActive <= */
			if (FAILED(m_pTextureCom[NONACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
			break;
		case Client::CUI::LEVEL1: /* => Unlock/Active <= */
			if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
			break;
		case Client::CUI::LEVEL2:
			break;
		case Client::CUI::LEVEL3:
			break;
		case Client::CUI::LEVEL4:
			break;
		case Client::CUI::LEVEL5:
			break;
		case Client::CUI::LEVEL_END:
			break;
		default:
			break;
		}
	}

	return S_OK;
}

HRESULT CUI_SkillFrame::Set_ParentTransform(CTransform* pParentTransformCom)
{
	m_tUIInfo.pParentTransformCom = pParentTransformCom;
	return S_OK;
}

void CUI_SkillFrame::Check_Picking(_float fTimeDelta)
{
	if (m_bPick == true)
	{
		if (g_UIMouseDownLB == true)
		{
#pragma region 1
			if (m_tUIInfo.strUIName == "Kick")
			{
				m_pUIManager->Change_SkillPreview("Kick");
				m_pUIManager->Select_Skill("Kick");
			}
			else if (m_tUIInfo.strUIName == "ElectricDash")
			{
				m_pUIManager->Change_SkillPreview("ElectricDash");
				m_pUIManager->Select_Skill("ElectricDash");
			}
			else if (m_tUIInfo.strUIName == "DashShock")
			{
				m_pUIManager->Change_SkillPreview("DashShock");
				m_pUIManager->Select_Skill("DashShock");
			}
			else if (m_tUIInfo.strUIName == "ElectricCord")
			{
				m_pUIManager->Change_SkillPreview("ElectricCord");
				m_pUIManager->Select_Skill("ElectricCord");
			}
			else if (m_tUIInfo.strUIName == "PowerUP")
			{
				m_pUIManager->Change_SkillPreview("PowerUP");
				m_pUIManager->Select_Skill("PowerUP");
			}
#pragma region 2
			else if (m_tUIInfo.strUIName == "UpperCut")
			{
				m_pUIManager->Change_SkillPreview("UpperCut");
				m_pUIManager->Select_Skill("UpperCut");
			}
			else if (m_tUIInfo.strUIName == "OneTouch")
			{
				m_pUIManager->Change_SkillPreview("OneTouch");
				m_pUIManager->Select_Skill("OneTouch");
			}
			else if (m_tUIInfo.strUIName == "TwoTouch")
			{
				m_pUIManager->Change_SkillPreview("TwoTouch");
				m_pUIManager->Select_Skill("TwoTouch");
			}
			else if (m_tUIInfo.strUIName == "ThreeTouch")
			{
				m_pUIManager->Change_SkillPreview("ThreeTouch");
				m_pUIManager->Select_Skill("ThreeTouch");
			}
			else if (m_tUIInfo.strUIName == "ComboPunch")
			{
				m_pUIManager->Change_SkillPreview("ComboPunch");
				m_pUIManager->Select_Skill("ComboPunch");
			}
#pragma region 3
			else if (m_tUIInfo.strUIName == "Punch")
			{
				m_pUIManager->Change_SkillPreview("Punch");
				m_pUIManager->Select_Skill("Punch");
			}
			else if (m_tUIInfo.strUIName == "SuperChargeMod")
			{
				m_pUIManager->Change_SkillPreview("SuperChargeMod");
				m_pUIManager->Select_Skill("SuperChargeMod");
			}
			else if (m_tUIInfo.strUIName == "TeleportPunch")
			{
				m_pUIManager->Change_SkillPreview("TeleportPunch");
				m_pUIManager->Select_Skill("TeleportPunch");
			}
			else if (m_tUIInfo.strUIName == "IncreaseEXP")
			{
				m_pUIManager->Change_SkillPreview("IncreaseEXP");
				m_pUIManager->Select_Skill("IncreaseEXP");
			}
			else if (m_tUIInfo.strUIName == "NPCPowerUP")
			{
				m_pUIManager->Change_SkillPreview("NPCPowerUP");
				m_pUIManager->Select_Skill("NPCPowerUP");
			}
#pragma region 4
			else if (m_tUIInfo.strUIName == "Heal")
			{
				m_pUIManager->Change_SkillPreview("Heal");
				m_pUIManager->Select_Skill("Heal");
			}
			else if (m_tUIInfo.strUIName == "RecoveryEnergy")
			{
				m_pUIManager->Change_SkillPreview("RecoveryEnergy");
				m_pUIManager->Select_Skill("RecoveryEnergy");
			}
			else if (m_tUIInfo.strUIName == "IncreaseHP")
			{
				m_pUIManager->Change_SkillPreview("IncreaseHP");
				m_pUIManager->Select_Skill("IncreaseHP");
			}
			else if (m_tUIInfo.strUIName == "IncreaseEnergy")
			{
				m_pUIManager->Change_SkillPreview("IncreaseEnergy");
				m_pUIManager->Select_Skill("IncreaseEnergy");
			}
			else if (m_tUIInfo.strUIName == "MaxHP")
			{
				m_pUIManager->Change_SkillPreview("MaxHP");
				m_pUIManager->Select_Skill("MaxHP");
			}
		}
	}
}

void CUI_SkillFrame::Check_State(_float fTimeDelta)
{
#pragma region 1
	if (m_tUIInfo.strUIName == "Kick")
	{

	}
	else if (m_tUIInfo.strUIName == "ElectricDash")
	{

	}
	else if (m_tUIInfo.strUIName == "DashShock")
	{

	}
	else if (m_tUIInfo.strUIName == "ElectricCord")
	{

	}
	else if (m_tUIInfo.strUIName == "PowerUP")
	{

	}
#pragma region 2
	else if (m_tUIInfo.strUIName == "UpperCut")
	{

	}
	else if (m_tUIInfo.strUIName == "OneTouch")
	{

	}
	else if (m_tUIInfo.strUIName == "TwoTouch")
	{

	}
	else if (m_tUIInfo.strUIName == "ThreeTouch")
	{

	}
	else if (m_tUIInfo.strUIName == "ComboPunch")
	{

	}
#pragma region 3
	else if (m_tUIInfo.strUIName == "Punch")
	{

	}
	else if (m_tUIInfo.strUIName == "SuperChargeMod")
	{

	}
	else if (m_tUIInfo.strUIName == "TeleportPunch")
	{

	}
	else if (m_tUIInfo.strUIName == "IncreaseEXP")
	{

	}
	else if (m_tUIInfo.strUIName == "NPCPowerUP")
	{

	}
#pragma region 4
	else if (m_tUIInfo.strUIName == "Heal")
	{

	}
	else if (m_tUIInfo.strUIName == "RecoveryEnergy")
	{

	}
	else if (m_tUIInfo.strUIName == "IncreaseHP")
	{

	}
	else if (m_tUIInfo.strUIName == "IncreaseEnergy")
	{

	}
	else if (m_tUIInfo.strUIName == "MaxHP")
	{

	}
}

void CUI_SkillFrame::Check_LevelChange(_float fTimeDelta)
{
	if (m_eUI_PreLevel != m_eUI_Level)
	{
#pragma region 1
		if (m_tUIInfo.strUIName == "Kick")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "ElectricDash")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "DashShock")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "ElectricCord")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "PowerUP")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
#pragma region 2
		else if (m_tUIInfo.strUIName == "UpperCut")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "OneTouch")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "TwoTouch")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "ThreeTouch")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "ComboPunch")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
#pragma region 3
		else if (m_tUIInfo.strUIName == "Punch")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "SuperChargeMod")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "TeleportPunch")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "IncreaseEXP")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "NPCPowerUP")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
#pragma region 4
		else if (m_tUIInfo.strUIName == "Heal")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "RecoveryEnergy")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "IncreaseHP")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "IncreaseEnergy")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
		else if (m_tUIInfo.strUIName == "MaxHP")
		{
			m_pUIManager->Change_SkillIcon_Level(m_tUIInfo.strUIName, m_eUI_Level);
		}
	}
}

void CUI_SkillFrame::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_SkillFrame::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_SkillFrame::Save_Desc(json& out_json)
{
	// Save error : ������ ���� �θ𿡼� �ٲ�µ� �� Ŭ�������� ���������.
	__super::Save_Desc(out_json);

	//_float fSizeX = 0.f;
	//_float fSizeY = 0.f;
	//_float fPositionX = 0.f;
	//_float fPositionY = 0.f;

	//_float fCurPosX = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0];
	//_float fCurPosY = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1];

	//fCurPosX = fCurPosX + (_float)g_iWinSizeX * 0.5f;
	//fCurPosY = (_float)g_iWinSizeY * 0.5f - fCurPosY;

	//out_json["CloneTag"] = m_tUIInfo.strCloneTag;

	//out_json["ProtoTag"] = m_tUIInfo.strProtoTag;

	//out_json["FilePath"] = m_tUIInfo.strFilePath;

	//m_pTransformCom->Write_Json(out_json);

	return out_json;
}

void CUI_SkillFrame::Load_Desc()
{

}

CUI_SkillFrame* CUI_SkillFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_SkillFrame* pInstance = new CUI_SkillFrame(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_SkillFrame");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_SkillFrame::Clone(void* pArg)
{
	CUI_SkillFrame* pInstance = new CUI_SkillFrame(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_SkillFrame");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_SkillFrame::Pool()
{
	return new CUI_SkillFrame(*this);
}

void CUI_SkillFrame::Free()
{
	__super::Free();

	// error : ù ��¼��, Texture->Release 2��° 3��° ��¼��.. �迭���� �ؽ�ó ��������ε�, Free���� for������ ��� �����°� �ƴ�, �׳� ���� Safe_Release����ؼ� ���� �߻�
	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}


