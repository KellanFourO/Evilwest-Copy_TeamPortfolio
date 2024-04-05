#include "stdafx.h"
#include "Grid.h"
#include "Shader.h"
#include "VIBuffer_Grid.h"
#include "GameInstance.h"


CGrid::CGrid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CGameObject(pDevice, pContext, strPrototypeTag)
{
	m_bIsPoolObject = FALSE;
}

CGrid::CGrid(const CGrid& rhs)
	: CGameObject(rhs)
{
}

HRESULT CGrid::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CGrid::Initialize(void* pArg)
{
	if (pArg != nullptr)
	{
		GRID_DESC* pDesc = (GRID_DESC*)pArg;

		m_tDesc.vGridColor = pDesc->vGridColor;
		for (_uint i = 0; i < (_uint)TEXTURE_END; i++)
		{
			m_tDesc.strTextureTag[i] = pDesc->strTextureTag[i];
			m_tDesc.iTextureIndex[i] = pDesc->iTextureIndex[i];
		}
		m_tDesc.bRender = pDesc->bRender;
		m_tDesc.iShaderPassIndex = pDesc->iShaderPassIndex;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	if (FAILED(Ready_Components()))
		return E_FAIL;


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-25.f, 0.f, -25.f, 1.f));


	return S_OK;
}

void CGrid::Priority_Tick(_float fTimeDelta)
{
	if (m_tDesc.bRender)
	{
		__super::Priority_Tick(fTimeDelta);
	}

}

void CGrid::Tick(_float fTimeDelta)
{
	if (m_tDesc.bRender)
	{
		__super::Tick(fTimeDelta);

	}
}

void CGrid::Late_Tick(_float fTimeDelta)
{

	if (m_tDesc.bRender)
	{
		__super::Late_Tick(fTimeDelta);

		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
			return;

	}
}

HRESULT CGrid::Render()
{
	if (m_tDesc.bRender)
	{
		__super::Render();
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		// �� ���̴��� 1��° �н��� �׸��ž�.
		m_pShaderCom->Begin(m_tDesc.iShaderPassIndex);

		if (m_pVIBufferCom != nullptr)
		{
			// ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
			m_pVIBufferCom->Bind_VIBuffers();

			// ���ε��� ����, �ε����� �׷�
			m_pVIBufferCom->Render();
		}

	}

	return S_OK;
}


HRESULT CGrid::Ready_Components()
{

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_TOOL, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	CVIBuffer_Grid::GRID_BUFFER_DESC	tBufferDesc = {};
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Grid"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &tBufferDesc)))
		return E_FAIL;

	/* For.Com_Texture */
	{
		if (TEXT("") != m_tDesc.strTextureTag[TEXTURE_DIFFUSE])
		{
			/* For.Com_Texture */
			if (FAILED(__super::Add_Component(LEVEL_STATIC, m_tDesc.strTextureTag[TEXTURE_DIFFUSE],
				TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_DIFFUSE]))))
				return E_FAIL;
		}

		if (TEXT("") != m_tDesc.strTextureTag[TEXTURE_MASK])
		{
			/* For.Com_Mask */
			if (FAILED(__super::Add_Component(LEVEL_STATIC, m_tDesc.strTextureTag[TEXTURE_MASK],
				TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_MASK]))))
				return E_FAIL;
		}
	}


	return S_OK;
}

HRESULT CGrid::Bind_ShaderResources()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	// ��ǻ��
	if (nullptr != m_pTextureCom[TEXTURE_DIFFUSE])
	{
		if (FAILED(m_pTextureCom[TEXTURE_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_tDesc.iTextureIndex[TEXTURE_DIFFUSE])))
			return E_FAIL;
	}


	// ����ũ
	if (nullptr != m_pTextureCom[TEXTURE_MASK])
	{
		if (FAILED(m_pTextureCom[TEXTURE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_tDesc.iTextureIndex[TEXTURE_MASK])))
			return E_FAIL;
	}


	// �÷�
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_tDesc.vGridColor, sizeof(_float4))))
		return E_FAIL;


	return S_OK;
}


CGrid* CGrid::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CGrid* pInstance = new CGrid(pDevice, pContext, strPrototypeTag);

	/* ���� ��ü�� �ʱ�ȭ�Ѵ�. */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGrid");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGrid::Clone(void* pArg) // ���⼭ �纻�� ���鶧 ������ ������ ���� �޾��ش�.
{
	CGrid* pInstance = new CGrid(*this);

	/* �纻 ��ü�� �ʱ�ȭ�Ѵ�. */
	if (FAILED(pInstance->Initialize(pArg))) // �Ѱ���
	{
		MSG_BOX("Failed to Cloned : CGrid");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Engine::CGameObject* CGrid::Pool()
{
	return new CGrid(*this);
}

void CGrid::Free()
{
	__super::Free();

	
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);

	for (_uint i = 0; i < (_uint)TEXTURE_END; i++)
	{
		Safe_Release(m_pTextureCom[i]);
	}

}


