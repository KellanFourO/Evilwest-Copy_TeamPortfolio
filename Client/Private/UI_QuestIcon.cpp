#include "stdafx.h"
#include "UI_QuestIcon.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_QuestIcon::CUI_QuestIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_QuestIcon::CUI_QuestIcon(const CUI_QuestIcon& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_QuestIcon::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_QuestIcon::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fPositionZ = 0.1f;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_QuestIcon::Priority_Tick(_float fTimeDelta)
{

}

void CUI_QuestIcon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_QuestIcon::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();


	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_QuestIcon::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//! �� ���̴��� 0��° �н��� �׸���.
	m_pShaderCom->Begin(0); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

	//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
	m_pVIBufferCom->Bind_VIBuffers();

	//! ���ε��� ����, �ε����� �׷�
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_QuestIcon::Ready_Components()
{
	FAILED_CHECK(__super::Ready_Components()); // Ready : Texture / MapTexture
	
	wstring strPrototag;
	m_pGameInstance->String_To_WString(m_tUIInfo.strProtoTag, strPrototag);

	//! For.Com_Texture // Quest Icon
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
		TEXT("Com_Texture_QuestIcon"), reinterpret_cast<CComponent**>(&m_pTextureCom[QUEST_ICON]))))
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

HRESULT CUI_QuestIcon::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	for (_int i = (_int)0; i < (_int)TEXTURE_END; ++i)
	{
		switch (i)
		{
		case CUI_QuestIcon::QUEST_ICON:
		{
			if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
			break;
		}
		case CUI_QuestIcon::TEXTURE_END:
			break;
		default:
			break;
		}
	}

	return S_OK;
}

void CUI_QuestIcon::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_QuestIcon::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_QuestIcon::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);


	/* �߰����� ���� */


	return out_json;
}

void CUI_QuestIcon::Load_Desc()
{

}

CUI_QuestIcon* CUI_QuestIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_QuestIcon* pInstance = new CUI_QuestIcon(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_QuestIcon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_QuestIcon::Clone(void* pArg)
{
	CUI_QuestIcon* pInstance = new CUI_QuestIcon(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_QuestIcon");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_QuestIcon::Pool()
{
	return new CUI_QuestIcon(*this);
}

void CUI_QuestIcon::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
