#include "stdafx.h"
#include "..\Public\Effect_Explosion.h"

#include "GameInstance.h"


CEffect_Explosion::CEffect_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CAlphaObject(pDevice, pContext, strPrototypeTag)
{

}

CEffect_Explosion::CEffect_Explosion(const CEffect_Explosion & rhs)
	: CAlphaObject(rhs)
{
}

HRESULT CEffect_Explosion::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ������ �����Ѵ�. */
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/
	//int*		pData_int = new int(3);

	//void**		pData = (void**)&pData_int;

	return S_OK;
}

HRESULT CEffect_Explosion::Initialize(void* pArg)
{	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// void* pArg : �����Ͽ� ��ü�� �����Ҷ� �������� ä���� ���� �����͸� �߰������� �� �ʱ�ȭ���ֱ����ؼ�. 
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(_float(rand() % 20), 5.0f, _float(rand() % 20), 1.f));	

	return S_OK;
}

void CEffect_Explosion::Priority_Tick(_float fTimeDelta)
{
	int a = 10;

	
}

void CEffect_Explosion::Tick(_float fTimeDelta)
{
	m_fFrame += 90.f * fTimeDelta;

	if (m_fFrame >= 90.0f)
		m_fFrame = 0.f;
}

void CEffect_Explosion::Late_Tick(_float fTimeDelta)
{
	Compute_CamDistance();

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_BLEND, this)))
		return ;
}

HRESULT CEffect_Explosion::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* �� �Τ��̴��� 0��° �н��� �׸�����. */
	m_pShaderCom->Begin(1);

	/* ���� �׸������ϴ� ����, �ΉK�������۸� ��ġ�� ���ε���. */
	m_pVIBufferCom->Bind_VIBuffers();

	/* ���ε��� ����, �ε����� �׷�. */
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CEffect_Explosion::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Explosion"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Explosion::Bind_ShaderResources()
{
	
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CEffect_Explosion * CEffect_Explosion::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strPrototypeTag)
{
	CEffect_Explosion*		pInstance = new CEffect_Explosion(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Explosion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Explosion::Clone(void* pArg)
{
	CEffect_Explosion*		pInstance = new CEffect_Explosion(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Explosion");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CEffect_Explosion::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}

