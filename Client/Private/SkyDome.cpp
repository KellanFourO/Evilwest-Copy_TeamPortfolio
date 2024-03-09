#include "stdafx.h"
#include "SkyDome.h"
#include "GameInstance.h"

CSkyDome::CSkyDome(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CGameObject(pDevice, pContext, strPrototypeTag)
{
}

CSkyDome::CSkyDome(const CSkyDome& rhs)
	: CGameObject(rhs)
{
}

HRESULT CSkyDome::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CSkyDome::Initialize(void* pArg)
{

	m_iCurrentLevel = m_pGameInstance->Get_NextLevel();

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;



	return S_OK;
}

void CSkyDome::Priority_Tick(_float fTimeDelta)
{
}

void CSkyDome::Tick(_float fTimeDelta)
{
}

void CSkyDome::Late_Tick(_float fTimeDelta)
{

	//! ��ī�̹ڽ��� ī�޶� ��ġ�� �������־���Ѵ�. ��� ���̴����� ����������Ʈ�� �����׽�Ʈ�� �����ʰ� ���̱�ϵ� �����ʱ⶧���� �ٸ� ��ü���� �������� �ʰ�, �ڽ��� �׻� �׷����� ���� ��.
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_pGameInstance->Get_CamPosition()));

	if(FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_PRIORITY, this)))
		return;
}

HRESULT CSkyDome::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		m_pModelCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", (_uint)i, aiTextureType_DIFFUSE);

		m_pShaderCom->Begin((_uint)1);
		m_pModelCom->Render((_uint)i);
	}
	
	return S_OK;
}

HRESULT CSkyDome::Ready_Components()
{
	

	//! For. Com_Shader
	if(FAILED(__super::Add_Component(m_iCurrentLevel, TEXT("Prototype_Component_Shader_Model"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_Model 
	if (FAILED(__super::Add_Component(m_iCurrentLevel, TEXT("Prototype_Component_Model_SkyDome"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	


	return S_OK;
}

HRESULT CSkyDome::Bind_ShaderResources()
{
	if(FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float fCamFar = m_pGameInstance->Get_CamFar();
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fCamFar", &fCamFar, sizeof(_float))))
		return E_FAIL;
	
	
	

	return S_OK;
}

CSkyDome* CSkyDome::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CSkyDome* pInstance = new CSkyDome(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyDome");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSkyDome::Clone(void* pArg)
{
	CSkyDome* pInstance = new CSkyDome(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkyDome");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CSkyDome::Pool()
{
	return new CSkyDome(*this);
}

void CSkyDome::Free()
{
	__super::Free();

}


