#include "stdafx.h"
#include "GameInstance.h"
#include "Body_Infected.h"

CBody_Infected::CBody_Infected(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CBody(pDevice, pContext, strPrototypeTag)
{
}

CBody_Infected::CBody_Infected(const CBody_Infected& rhs)
	: CBody(rhs)
{
}

HRESULT CBody_Infected::Initialize_Prototype()
{
	FAILED_CHECK(__super::Initialize_Prototype());

	return S_OK;
}

HRESULT CBody_Infected::Initialize(void* pArg)
{
	FAILED_CHECK(__super::Initialize(pArg));

	return S_OK;
}

void CBody_Infected::Priority_Tick(_float fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CBody_Infected::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBody_Infected::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CBody_Infected::Render()
{
	FAILED_CHECK(Bind_ShaderResources());

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		auto iter = m_vDiscardMesh.find(m_eRender_State); // ���� ���¿� �ش��ϴ� vector<int> ã�� 
		if (iter != m_vDiscardMesh.end())
		{
			auto& Discard = iter->second;
			if (find(Discard.begin(), Discard.end(), i) != Discard.end()) // ���� ������ ������ �޽��� ��ȣ�� vector<int>�� �������, 
			{
				if (m_eRender_State == CBody_Infected::RENDER_STATE::ATTACK)
				{
					m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i);
					m_pModelCom->Bind_MaterialResource(m_pShaderCom, (_uint)i);
					m_pShaderCom->Begin(ECast(MONSTER_SHADER::INFECTED_PUNCH));
					m_pModelCom->Render((_uint)i);
				}
				else
					continue;
			}
			else // ���� ������ ������ �޽��� ��ȣ�� vector<int>�� �ٸ� ���, 
			{
				m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i);
				m_pModelCom->Bind_MaterialResource(m_pShaderCom, (_uint)i);
				m_pShaderCom->Begin(ECast(MONSTER_SHADER::COMMON_ORIGIN));
				m_pModelCom->Render((_uint)i);
			}
		}
	}

	return S_OK;
}

HRESULT CBody_Infected::Render_Shadow()
{
	_float lightFarValue = m_pGameInstance->Get_ShadowLightFar(m_pGameInstance->Get_NextLevel());
	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fLightFar", &lightFarValue, sizeof(_float)));
	FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix));
	FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_ShadowLightViewMatrix(m_pGameInstance->Get_NextLevel())));
	FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_ShadowLightProjMatrix(m_pGameInstance->Get_NextLevel())));

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i);

		m_pModelCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", (_uint)i, aiTextureType_DIFFUSE);

		m_pShaderCom->Begin(2);

		m_pModelCom->Render((_uint)i);
	}

	return S_OK;
}

HRESULT CBody_Infected::Set_StateHit()
{
	/* Origin�� Discard�ϰ��ִ°� �� �޽ö� �ϳ��� ���ֹ����� �ǰ� �ϳ��� ��Ÿ���� ȿ���� ���ð���. */
	auto iter = m_vDiscardMesh.find(RENDER_STATE::ORIGIN);

	if (iter != m_vDiscardMesh.end()) 
	{
		if(iter->second.size() > 0 )
			m_vDiscardMesh[RENDER_STATE::ORIGIN].pop_back();
	}
	
	return S_OK;
}

HRESULT CBody_Infected::Set_StateDead()
{
	return E_NOTIMPL;
}

HRESULT CBody_Infected::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Shader */
	{
		FAILED_CHECK(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Shader_Monster"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom)));
	}
	
	/* For.Com_Collider */
	{
		CBounding_AABB::BOUNDING_AABB_DESC		BoundingDesc = {};
		BoundingDesc.iLayer = ECast(COLLISION_LAYER::MONSTER);
		BoundingDesc.vExtents = _float3(0.22f, 0.8f, 0.22f);
		BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);

		FAILED_CHECK(__super::Add_Component(m_pGameInstance->Get_NextLevel(), TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc));
	}

	return S_OK;
}

HRESULT CBody_Infected::Bind_ShaderResources()
{
	FAILED_CHECK(__super::Bind_ShaderResources());

	_float fCamFar = m_pGameInstance->Get_CamFar();
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fCamFar", &fCamFar, sizeof(_float)));

	return S_OK;
}

void CBody_Infected::Free()
{
	//for (auto& pair : m_vDiscardMesh)
	//{
	//	for (auto& element : pair.second) 
	//	{
	//		
	//	}
	//	// ������ �ڿ��� ��� ������ �Ŀ��� clear() �Լ��� ȣ���Ͽ� ���͸� ����ݴϴ�.
	//	pair.second.clear();
	//}
	//// ���� �ڿ��� �����մϴ�.
	//m_vDiscardMesh.clear();

	__super::Free();
}
