#include "stdafx.h"
#include "Body_Bandit_Sniper.h"
#include "GameInstance.h"

CBody_Bandit_Sniper::CBody_Bandit_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CBody(pDevice, pContext, strPrototypeTag)
{
}

CBody_Bandit_Sniper::CBody_Bandit_Sniper(const CBody_Bandit_Sniper& rhs)
	: CBody(rhs)
{
}

HRESULT CBody_Bandit_Sniper::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype());

	return S_OK;
}

HRESULT CBody_Bandit_Sniper::Initialize(void* pArg)
{
	FAILED_CHECK(__super::Initialize(pArg));

	FAILED_CHECK(Ready_ShadeValue());

	return S_OK;
}

HRESULT CBody_Bandit_Sniper::Ready_Components()
{
	/* For.Com_Shader */
	FAILED_CHECK(__super::Add_Component(m_iCurrnetLevel, TEXT("Prototype_Component_Shader_Monster"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom)));

	/* For.Com_Model */
	FAILED_CHECK(__super::Add_Component(m_iCurrnetLevel, TEXT("Prototype_Component_Model_Bandit_Sniper"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom)));

	/* For. Texture */
	FAILED_CHECK(__super::Add_Component(m_iCurrnetLevel, TEXT("Prototype_Component_Texture_Shader_Dissolve"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pDissolveTexture)));

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC		BoundingDesc = {};
	BoundingDesc.iLayer = ECast(COLLISION_LAYER::MONSTER);
	BoundingDesc.vExtents = _float3(0.4f, 0.9f, 0.4f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);
	FAILED_CHECK(__super::Add_Component(m_iCurrnetLevel, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc));

	return S_OK;
}
void CBody_Bandit_Sniper::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CBody_Bandit_Sniper::Tick(_float fTimeDelta)
{
	if (m_iRenderPass == ECast(MONSTER_SHADER::COMMON_DISSOLVE))
	{
		m_bDissolve = true;

		if (m_fDissolveWeight <= 3.f)
			m_fDissolveWeight += fTimeDelta * 0.5f;

	}

	//m_fTimeAcc += fTimeDelta * 0.5f;
	//
	//if (m_fTimeAcc > 1.f)
	//	m_fTimeAcc = 0.f;
	m_fTimeAcc = 1.f;

	__super::Tick(fTimeDelta);

}

void CBody_Bandit_Sniper::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	FAILED_CHECK_RETURN(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_OUTLINE, this), );
	FAILED_CHECK_RETURN(m_pGameInstance->Add_CascadeObject(1, this), );
	FAILED_CHECK_RETURN(m_pGameInstance->Add_CascadeObject(2, this), );
}

HRESULT CBody_Bandit_Sniper::Render()
{
	FAILED_CHECK(Bind_ShaderResources());

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i);
		m_pModelCom->Bind_MaterialResource(m_pShaderCom, (_uint)i, &m_bORM_Available, &m_bEmissive_Available);
		m_pShaderCom->Bind_RawValue("g_bORM_Available", &m_bORM_Available, sizeof(_bool));
		m_pShaderCom->Bind_RawValue("g_bEmissive_Available", &m_bEmissive_Available, sizeof(_bool));

		if (m_bDissolve)
		{
			m_pDissolveTexture->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture");
			m_pShaderCom->Bind_RawValue("g_Dissolve_Weight", &m_fDissolveWeight, sizeof(_float));
			m_pShaderCom->Bind_RawValue("g_Dissolve_feather", &m_fDissolve_feather, sizeof(_float));
			m_pShaderCom->Bind_RawValue("g_Dissolve_Color", &m_vDissolve_Color, sizeof(_float));
			m_pShaderCom->Bind_RawValue("g_Dissolve_ColorRange", &m_fDissolve_Discard, sizeof(_float));

			m_pShaderCom->Begin(ECast(MONSTER_SHADER::COMMON_DISSOLVE));
		}
		else
		{
			m_pShaderCom->Begin(ECast(MONSTER_SHADER::COMMON_ORIGIN));
		}
		m_pModelCom->Render((_uint)i);
	}

	return S_OK;
}

HRESULT CBody_Bandit_Sniper::Render_Shadow()
{
	_float lightFarValue = m_pGameInstance->Get_ShadowLightFar(m_iCurrnetLevel);
	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fLightFar", &lightFarValue, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix));
	FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_ShadowLightViewMatrix(m_pGameInstance->Get_NextLevel())));
	FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_ShadowLightProjMatrix(m_pGameInstance->Get_NextLevel())));

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pShaderCom->Begin(ECast(MONSTER_SHADER::COMMON_SHADOW));
		m_pModelCom->Render((_uint)i);
	}

	return S_OK;
}

HRESULT CBody_Bandit_Sniper::Render_CSM(_uint i)
{
	FAILED_CHECK(Bind_ShaderResources());

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	m_pShaderCom->Bind_Matrix("g_CascadeProj", &m_pGameInstance->Get_Shadow_Proj()[i]);	

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i);
		m_pShaderCom->Begin(ECast(MONSTER_SHADER::COMMON_CASCADE));
		m_pModelCom->Render((_uint)i);
	}

	return S_OK;
}

HRESULT CBody_Bandit_Sniper::Render_OutLine()
{
	FAILED_CHECK(Bind_ShaderResources());
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vLineColor", &m_vLineColor, sizeof(_float4)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_LineThick", &m_fLineThick, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_TimeDelta", &m_fTimeAcc, sizeof(_float)));

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i);
		m_pShaderCom->Begin(ECast(MONSTER_SHADER::COMMON_OUTLINE));
		m_pModelCom->Render((_uint)i);
	}

	return S_OK;
}


HRESULT CBody_Bandit_Sniper::Bind_ShaderResources()
{
	FAILED_CHECK(__super::Bind_ShaderResources());

	m_fCamFar = m_pGameInstance->Get_CamFar();
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fCamFar", &m_fCamFar, sizeof(_float)));

	return S_OK;
}

HRESULT CBody_Bandit_Sniper::Ready_ShadeValue()
{
	/* Dissolve */
	m_fDissolveWeight = 0.f;
	m_fDissolve_feather = 0.1f;
	m_vDissolve_Color = { 1.f, 0.f, 0.f };
	m_fDissolve_Discard = 0.2f;

	/* OutLine */
	m_vLineColor = { 1.f, 0.f, 0.f, 1.f };
	m_fLineThick = 0.3f;
	m_fTimeAcc = 0.f;

	return S_OK;
}

CBody_Bandit_Sniper* CBody_Bandit_Sniper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CBody_Bandit_Sniper* pInstance = new CBody_Bandit_Sniper(pDevice, pContext, strPrototypeTag);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBody_Bandit_Sniper");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBody_Bandit_Sniper::Clone(void* pArg)
{
	CBody_Bandit_Sniper* pInstance = new CBody_Bandit_Sniper(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBody_Bandit_Sniper");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CBody_Bandit_Sniper::Pool()
{
	return new CBody_Bandit_Sniper(*this);
}

void CBody_Bandit_Sniper::Free()
{
	__super::Free();
}
