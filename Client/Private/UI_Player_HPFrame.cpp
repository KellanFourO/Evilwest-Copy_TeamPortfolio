#include "stdafx.h"
#include "UI_Player_HPFrame.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_Player_HPFrame::CUI_Player_HPFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Player_HPFrame::CUI_Player_HPFrame(const CUI_Player_HPFrame& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_HPFrame::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Player_HPFrame::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_Player_HPFrame::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Player_HPFrame::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Player_HPFrame::Late_Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return;
	}
}

HRESULT CUI_Player_HPFrame::Render()
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

void CUI_Player_HPFrame::UI_Ready(_float fTimeDelta)
{
}

void CUI_Player_HPFrame::UI_Enter(_float fTimeDelta)
{
}

void CUI_Player_HPFrame::UI_Loop(_float fTimeDelta)
{
}

void CUI_Player_HPFrame::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_Player_HPFrame::Ready_Components()
{
	FAILED_CHECK(__super::Ready_Components()); // Ready : Texture / MapTexture

	//! For.Com_Texture_1 - HP_Bar_Red
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_health_bar_bg"), TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom[FRAME])));

	//! For.Com_Shader
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom)));

	//! For.Com_VIBuffer
	FAILED_CHECK(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom)));

	return S_OK;
}

HRESULT CUI_Player_HPFrame::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	
	for (auto& iter : m_pTextureCom)
	{
		if (FAILED(iter->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}


	return S_OK;
}

void CUI_Player_HPFrame::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_Player_HPFrame::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_Player_HPFrame::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);


	/* �߰����� ���� */


	return out_json;
}

void CUI_Player_HPFrame::Load_Desc()
{

}

CUI_Player_HPFrame* CUI_Player_HPFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Player_HPFrame* pInstance = new CUI_Player_HPFrame(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_HPFrame");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_HPFrame::Clone(void* pArg)
{
	CUI_Player_HPFrame* pInstance = new CUI_Player_HPFrame(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_HPFrame");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_HPFrame::Pool()
{
	return new CUI_Player_HPFrame(*this);
}

void CUI_Player_HPFrame::Free()
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
