#include "stdafx.h"
#include "Greed.h"

#include "GameInstance.h"


CGreed::CGreed(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CGreed::CGreed(const CGreed& rhs)
	: CGameObject(rhs)
{
}

HRESULT CGreed::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ������ �����Ѵ�. */
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/
	//int*		pData_int = new int(3);

	//void**	pData = (void**)&pData_int;

	return S_OK;
	
}

HRESULT CGreed::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaling(8.f, 0.1f, 8.f);

	return S_OK;
}

void CGreed::Priority_Tick(_float fTimeDelta)
{

}

void CGreed::Tick(_float fTimeDelta)
{

}

void CGreed::Late_Tick(_float fTimeDelta)
{

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;

}

HRESULT CGreed::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* �� ���̴��� 3��° �н��� �׸��ž�. */
	m_pShaderCom->Begin(1);

	/* ���� �׸������ϴ� ����, �ε������۸� ��ġ�� ���ε���. */
	m_pVIBufferCom->Bind_VIBuffers();

	/* ���ε��� ����, �ε����� �׷�. */
	m_pVIBufferCom->Render();

	return S_OK;
}


HRESULT CGreed::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VtxCube"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;


	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Texture_Tool_Greed"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_DIFFUSE]))))
		return E_FAIL;


	return S_OK;
}

HRESULT CGreed::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	//if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
	//	return E_FAIL;
	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Bind_ShaderResources(m_pShaderCom, "g_Texture")))
	return E_FAIL;

	return S_OK;
}

CGreed* CGreed::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGreed* pInstance = new CGreed(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGreed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGreed::Clone(void* pArg)
{
	CGreed* pInstance = new CGreed(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGreed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGreed::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);

	for (size_t i = 0; i < TYPE_END; i++)
	{
		Safe_Release(m_pTextureCom[i]);
	}

	Safe_Release(m_pShaderCom);
}

