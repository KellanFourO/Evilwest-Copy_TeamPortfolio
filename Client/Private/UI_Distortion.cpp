#include "stdafx.h"
#include "UI_Distortion.h"
#include "GameInstance.h"
#include "Json_Utility.h"

#include "Data_Manager.h"

CUI_Distortion::CUI_Distortion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Distortion::CUI_Distortion(const CUI_Distortion& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Distortion::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Distortion::Initialize(void* pArg)
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

	return S_OK;
}

void CUI_Distortion::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Distortion::Tick(_float fTimeDelta)
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

		if (m_bRestore == true)
		{
			//if (m_tUIInfo.fTimeAcc > 0.f)
			//	m_tUIInfo.fTimeAcc -= fTimeDelta;

			if (m_tUIInfo.fSequenceTerm > 0.f)
				m_tUIInfo.fSequenceTerm -= fTimeDelta;
			else
				m_tUIInfo.fSequenceTerm = 0.f;

			if (m_tUIInfo.vScrollSpeeds.x > 0.f)
				m_tUIInfo.vScrollSpeeds.x -= fTimeDelta;
			else
				m_tUIInfo.vScrollSpeeds.x = 0.f;

			if (m_tUIInfo.vScales.x > 0.f)
				m_tUIInfo.vScales.x -= fTimeDelta;
			else
				m_tUIInfo.vScales.x = 0.f;

			if (m_tUIInfo.vDistortion1.x > 0.f)
				m_tUIInfo.vDistortion1.x -= fTimeDelta;
			else
				m_tUIInfo.vDistortion1.x = 0.f;

			if (m_tUIInfo.vDistortion1.y > 0.f)
				m_tUIInfo.vDistortion1.y -= fTimeDelta;
			else
				m_tUIInfo.vDistortion1.y = 0.f;

			if (m_tUIInfo.vDistortion2.x > 0.f)
				m_tUIInfo.vDistortion2.x -= fTimeDelta;
			else
				m_tUIInfo.vDistortion2.x = 0.f;

			if (m_tUIInfo.vDistortion2.y > 0.f)
				m_tUIInfo.vDistortion2.y -= fTimeDelta;
			else
				m_tUIInfo.vDistortion2.y = 0.f;

			if (m_tUIInfo.vDistortion3.x > 0.f)
				m_tUIInfo.vDistortion3.x -= fTimeDelta;
			else
				m_tUIInfo.vDistortion3.x = 0.f;

			if (m_tUIInfo.vDistortion3.y > 0.f)
				m_tUIInfo.vDistortion3.y -= fTimeDelta;
			else
				m_tUIInfo.vDistortion3.y = 0.f;

			if (m_tUIInfo.fDistortionScale > 0.f)
				m_tUIInfo.fDistortionScale -= fTimeDelta;
			else
				m_tUIInfo.fDistortionScale = 0.f;

			if (m_tUIInfo.fDistortionBias > 0.f)
				m_tUIInfo.fDistortionBias -= fTimeDelta;
			else
				m_tUIInfo.fDistortionBias = 0.f;

		}
	}
}

void CUI_Distortion::Late_Tick(_float fTimeDelta)
{
	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
			return;
	}
}

HRESULT CUI_Distortion::Render()
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

void CUI_Distortion::UI_Ready(_float fTimeDelta)
{
}

void CUI_Distortion::UI_Enter(_float fTimeDelta)
{
}

void CUI_Distortion::UI_Loop(_float fTimeDelta)
{
}

void CUI_Distortion::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_Distortion::Ready_Components()
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

	////! For.Com_Texture_Mask
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mask"),
	//	TEXT("Com_Texture_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[MASK]))))
	//	return E_FAIL;

	////! For.Com_Texture_Noise
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
	//	TEXT("Com_Texture_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom[NOISE]))))
	//	return E_FAIL;

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

HRESULT CUI_Distortion::Bind_ShaderResources()
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

		if (FAILED(m_pDistortionCom[MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_iMaskNum)))
			return E_FAIL;

		if (FAILED(m_pDistortionCom[NOISE]->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", m_iNoiseNum)))
			return E_FAIL;

	}
	return S_OK;
}

void CUI_Distortion::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_Distortion::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

void CUI_Distortion::Set_OwnerHp(/*CPlayer pPlayer*/)
{

}

json CUI_Distortion::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);


	/* �߰����� ���� */


	return out_json;
}

void CUI_Distortion::Load_Desc()
{

}

CUI_Distortion* CUI_Distortion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Distortion* pInstance = new CUI_Distortion(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Distortion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Distortion::Clone(void* pArg)
{
	CUI_Distortion* pInstance = new CUI_Distortion(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Distortion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Distortion::Pool()
{
	return new CUI_Distortion(*this);
}

void CUI_Distortion::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
