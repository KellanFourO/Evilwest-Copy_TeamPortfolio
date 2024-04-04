#include "stdafx.h"
#include "UI_SkillIcon.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Texture.h"

CUI_SkillIcon::CUI_SkillIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_SkillIcon::CUI_SkillIcon(const CUI_SkillIcon& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SkillIcon::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_SkillIcon::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_SkillIcon::Priority_Tick(_float fTimeDelta)
{

}

void CUI_SkillIcon::Tick(_float fTimeDelta)
{
	//if(!m_vecAnimation.empty())
	//	m_bActive = m_vecAnimation[iFrameIndex].bDisappear;

	__super::Tick(fTimeDelta);

	if (m_bActive == true)
	{
		if (m_bAnimChange == true)
		{
			switch (m_eAnimType)
			{
				case CUI_SkillIcon::UNLOCK:
					Change_Animation("../Bin/DataFiles/Data_UI/Animation/IconUnlock.json");
					break;
				case CUI_SkillIcon::JUMP:
					Change_Animation("../Bin/DataFiles/Data_UI/Animation/IconJump.json");
					break;
				case CUI_SkillIcon::UIANIM_END:
					break;
				default:
					break;
			}

			m_bAnimChange = false;
		}
#pragma region 1
		if (m_tUIInfo.strUIName == "Kick")
		{

		}
		else if(m_tUIInfo.strUIName == "ElectricDash")
		{

		}
		else if(m_tUIInfo.strUIName == "DashShock")
		{

		}
		else if(m_tUIInfo.strUIName == "ElectricCord")
		{

		}
		else if(m_tUIInfo.strUIName == "PowerUP")
		{

		}
#pragma region 2
		else if(m_tUIInfo.strUIName == "UpperCut")
		{

		}
		else if(m_tUIInfo.strUIName == "OneTouch")
		{

		}
		else if(m_tUIInfo.strUIName == "TwoTouch")
		{

		}
		else if(m_tUIInfo.strUIName == "ThreeTouch")
		{

		}
		else if(m_tUIInfo.strUIName == "ComboPunch")
		{

		}
#pragma region 3
		else if(m_tUIInfo.strUIName == "Punch")
		{

		}
		else if(m_tUIInfo.strUIName == "SuperChargeMod")
		{

		}
		else if(m_tUIInfo.strUIName == "TeleportPunch")
		{

		}
		else if(m_tUIInfo.strUIName == "IncreaseEXP")
		{

		}
		else if(m_tUIInfo.strUIName == "NPCPowerUP")
		{

		}
#pragma region 4
		else if(m_tUIInfo.strUIName == "Heal")
		{

		}
		else if(m_tUIInfo.strUIName == "RecoveryEnergy")
		{

		}
		else if(m_tUIInfo.strUIName == "IncreaseHP")
		{

		}
		else if(m_tUIInfo.strUIName == "IncreaseEnergy")
		{

		}
		else if (m_tUIInfo.strUIName == "MaxHP")
		{

		}

	}
}

void CUI_SkillIcon::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();


	//if (m_tUIInfo.pParentTransformCom != nullptr &&
	//	m_tUIInfo.bParent == false)
	//}Render_UI_MRT

	//	/* Parent */
	//	_vector vPosition = m_tUIInfo.pParentTransformCom->Get_State(CTransform::STATE_POSITION);
	//	XMMATRIX ParentMat = m_tUIInfo.pParentTransformCom->Get_WorldMatrix();
	//	/* Child */
	//	XMMATRIX ChildMat = m_pTransformCom->Get_WorldMatrix();

	//	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * m_tUIInfo.pParentTransformCom->Get_WorldMatrix());

	//	m_pTransformCom->Set_WorldMatrix(m_WorldMatrix);
	//}

	if (m_bActive == true)
	{
		if (m_bRenderOut == false) // �Ⱥ��̰� �ϴ°� �������� ��� ����
		{
			if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
				return;
		}
	}
}

HRESULT CUI_SkillIcon::Render()
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

void CUI_SkillIcon::UI_Ready(_float fTimeDelta)
{
}

void CUI_SkillIcon::UI_Enter(_float fTimeDelta)
{
}

void CUI_SkillIcon::UI_Loop(_float fTimeDelta)
{
}

void CUI_SkillIcon::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_SkillIcon::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

#pragma region �������� �ؽ�ó�� ��� ���� �� �ְ� ������Ѵ�.
	/* ù Create�� �ƹ��ų� �⺻ ���������� Initialize���� �ױ׸� �������ְ�, ���õ� �ؽ�ó�� �Ľ��ϸ� �Ľ̵� �ؽ�ó�� ����ϰ� ������. */
	wstring strPrototag;
	m_pGameInstance->String_To_WString(m_tUIInfo.strProtoTag, strPrototag);

	//! For.Com_Texture2 // ��Ȱ��ȭ
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag, // error : �ױ׵� �߸°� ���� �Լ����� �ؽ�ó�� �� ã�µ� nullptr�� ���� -> m_pTextureCom�� ����� �迭������ ����ߴµ�, ��������� �����Ҷ� �迭�� ���� ������.. 
		TEXT("Com_Texture_SkillIcon_NonActive"), reinterpret_cast<CComponent**>(&m_pTextureCom[NONACTIVE]))))
		return E_FAIL;

	strPrototag = strPrototag + L"_active";

	//! For.Com_Texture2 // Ȱ��ȭ
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
		TEXT("Com_Texture_SkillIcon_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CUI_SkillIcon::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


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
		case Client::CUI::STAGE_END:
			break;
		default:
			break;
	}

	return S_OK;
}

HRESULT CUI_SkillIcon::Set_ParentTransform(CTransform* pParentTransformCom)
{
	m_tUIInfo.pParentTransformCom = pParentTransformCom;
	return S_OK;
}

void CUI_SkillIcon::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_SkillIcon::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_SkillIcon::Save_Desc(json& out_json)
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

void CUI_SkillIcon::Load_Desc()
{

}

CUI_SkillIcon* CUI_SkillIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_SkillIcon* pInstance = new CUI_SkillIcon(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_SkillIcon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_SkillIcon::Clone(void* pArg)
{
	CUI_SkillIcon* pInstance = new CUI_SkillIcon(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_SkillIcon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_SkillIcon::Pool()
{
	return new CUI_SkillIcon(*this);
}

void CUI_SkillIcon::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}


