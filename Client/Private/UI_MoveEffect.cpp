#include "stdafx.h"
#include "UI_MoveEffect.h"
#include "GameInstance.h"
#include "Json_Utility.h"

#include "Data_Manager.h"

CUI_MoveEffect::CUI_MoveEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_MoveEffect::CUI_MoveEffect(const CUI_MoveEffect& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MoveEffect::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_MoveEffect::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	/* Distortion�� �ִ� UI */
	m_tUIInfo.bDistortionUI = true;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;


	//m_tUIInfo.vScales = { 1.f, 0.f, 0.f };
	//m_tUIInfo.fPositionZ = 0.0f;
	//m_tUIInfo.fDistortionScale = 1.f;
	//m_tUIInfo.vDistortion1.x = 1.f;
	//m_tUIInfo.vScrollSpeeds.x = 1.f;

	/*
	#include "Data_Manager.h" ��Ŭ��� ���ְ�,
	CData_Manager::GetInstance()->Get_Player(); �� �޾��ְ�
	�Լ� �״�� ����ϰų�, ������ ��Ƽ� ����ϱ�.
	�ش� ��ü�� ���ϴ� �Լ��� ���� ���� �ҷ�����.
	*/

	m_iMaskNum = m_tUIInfo.iMaskNum;
	m_iNoiseNum = m_tUIInfo.iNoiseNum;
	m_fLeftSize = 100.f;
	m_bBottomChange = true;
	m_bStart = true;

	return S_OK;
}

void CUI_MoveEffect::Priority_Tick(_float fTimeDelta)
{

}

void CUI_MoveEffect::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_iMaskNum = m_tUIInfo.iMaskNum;
	m_iNoiseNum = m_tUIInfo.iNoiseNum;

	if (m_bActive)
	{
		if (!m_vecAnimation.empty())
		{
			m_fTimeAcc += m_tUIInfo.tKeyframe.fTimeAcc * fTimeDelta;
		}
		else
		{
			m_fTimeAcc += m_tUIInfo.fTimeAcc * fTimeDelta;
		}

		if (m_bStart == true)
		{
			m_rcUI.top = (LONG)m_fOriginPoint;
			m_rcUI.bottom = (LONG)m_fOriginPoint;
			m_bStart = false;
		}

		if (m_rcUI.bottom >= m_rcUI.top && m_bTopChange == true)
		{
			Change_SizeTop(-m_fChangeValue);
			if (m_rcUI.bottom >= m_rcUI.top || m_rcUI.bottom == m_rcUI.top)
			{
				m_bTopChange = false;
				m_bBottomChange = true;
				m_bStart = true;
			}
		}


		if (m_rcUI.bottom <= m_fBottomSize && m_bBottomChange == true)
		{
			Change_SizeBottom(m_fChangeValue);
			if (m_rcUI.bottom >= m_fBottomSize || m_rcUI.bottom == m_fBottomSize)
			{
				m_bTopChange = true;
				m_bBottomChange = false;
			}
		}

	}
}

void CUI_MoveEffect::Late_Tick(_float fTimeDelta)
{
	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_MoveEffect::Render()
{
	if (m_bActive == true)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		//! �� ���̴��� 0��° �н��� �׸���.
		m_pShaderCom->Begin(6); // Distortion 6

		//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
		m_pVIBufferCom->Bind_VIBuffers();

		//! ���ε��� ����, �ε����� �׷�
		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_MoveEffect::UI_Ready(_float fTimeDelta)
{
}

void CUI_MoveEffect::UI_Enter(_float fTimeDelta)
{
}

void CUI_MoveEffect::UI_Loop(_float fTimeDelta)
{
}

void CUI_MoveEffect::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_MoveEffect::Ready_Components()
{
	/* ���� */
	if (FAILED(__super::Ready_Components())) // Ready : Texture / MapTexture
		return E_FAIL;

	wstring strPrototag;
	m_pGameInstance->String_To_WString(m_tUIInfo.strProtoTag, strPrototag);

	//! For.Com_Texture_Diffuse
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
		TEXT("Com_Texture_Diffuse"), reinterpret_cast<CComponent**>(&m_pTextureCom[DIFFUSE]))))
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

HRESULT CUI_MoveEffect::Bind_ShaderResources()
{
	/* ���� */
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFrameTime", &m_fTimeAcc, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vScrollSpeeds", &m_tUIInfo.vScrollSpeeds, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vScales", &m_tUIInfo.vScales, sizeof(_float3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDistortion1", &m_tUIInfo.vDistortion1, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDistortion2", &m_tUIInfo.vDistortion2, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vDistortion3", &m_tUIInfo.vDistortion3, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDistortionScale", &m_tUIInfo.fDistortionScale, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fDistortionBias", &m_tUIInfo.fDistortionBias, sizeof(_float))))
		return E_FAIL;

	/* For.Com_Texture */
	{
		if (FAILED(m_pTextureCom[DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	return S_OK;
}

void CUI_MoveEffect::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_MoveEffect::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

void CUI_MoveEffect::Set_OwnerHp(/*CPlayer pPlayer*/)
{

}

json CUI_MoveEffect::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);


	/* �߰����� ���� */


	return out_json;
}

void CUI_MoveEffect::Load_Desc()
{

}

CUI_MoveEffect* CUI_MoveEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_MoveEffect* pInstance = new CUI_MoveEffect(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_MoveEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_MoveEffect::Clone(void* pArg)
{
	CUI_MoveEffect* pInstance = new CUI_MoveEffect(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_MoveEffect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_MoveEffect::Pool()
{
	return new CUI_MoveEffect(*this);
}

void CUI_MoveEffect::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
