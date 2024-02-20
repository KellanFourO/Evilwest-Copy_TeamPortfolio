#include "stdafx.h"
#include "InstanceMonster.h"
#include "Collider.h"
#include "GameInstance.h"
#include "Animation.h"
#include "VIBuffer_AnimModel_Instance.h"
#include "Bone.h"

CInstanceMonster::CInstanceMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CInstanceMonster::CInstanceMonster(const CInstanceMonster& rhs)
	: CGameObject(rhs)
{
}

HRESULT CInstanceMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CInstanceMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Instance()))
		return E_FAIL;

	m_pModelCom->Set_Animation(rand() % 20);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(_float(rand() % 20), 0.f, _float(rand() % 20), 1.f));

	m_iRenderPass = 0;
	m_fTimeDelta = 0;

	return S_OK;
}

void CInstanceMonster::Priority_Tick(_float fTimeDelta)
{
}

void CInstanceMonster::Tick(_float fTimeDelta)
{
}

void CInstanceMonster::Late_Tick(_float fTimeDelta)
{
	//m_pGameInstance->Transform_Frustum_ToLocalSpace(m_pTransformCom->Get_WorldMatrix());

	//if (true == m_pGameInstance->isIn_LocalPlanes(XMVector3TransformCoord(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_pTransformCom->Get_WorldMatrixInverse()), 0.f))
	//{
		_float3 vRootAnimPos = {};

		m_pModelCom->Play_Animation(fTimeDelta, &vRootAnimPos);

		m_pTransformCom->Add_RootBone_Position(vRootAnimPos);

		if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
			return;

//#ifdef _DEBUG
//		m_pGameInstance->Add_DebugRender(m_pColliderCom);
//#endif	
	//}
}

HRESULT CInstanceMonster::Render()
{
	Ready_Render(24);
	Render_Instance(24);
		

	return S_OK;
}

HRESULT CInstanceMonster::Ready_Render(_uint iSize)
{
	if(m_pContext == nullptr || m_tInstanceDesc.pInstanceTexture == nullptr || m_tInstanceDesc.pMatrix == nullptr)
		return E_FAIL;

	
	size_t iSizePerInstance = m_pModelCom->Get_Bones()->size() * sizeof(_float4x4);
	

	//for (_int i = 0; i < iSizePerInstance; ++i)
	//{
	//	XMStoreFloat4x4(&m_tInstanceDesc.pMatrix[i], XMMatrixIdentity());
	//}

	

	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	m_pContext->Map(m_tInstanceDesc.pInstanceTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

//	if (FAILED(m_pContext->Map(m_tInstanceDesc.pInstanceTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
//		return E_FAIL;

	memcpy(SubResource.pData, m_tInstanceDesc.pMatrix, iSizePerInstance * iSize);
	
	m_pContext->Unmap(m_tInstanceDesc.pInstanceTexture, 0);

	
	
	return S_OK;
}

HRESULT CInstanceMonster::Render_Instance(_uint iSize)
{
	


	//hr = FAILED(m_tInstanceDesc.pInstanceShader->Bind_SRV("g_InstanceTransform", m_tInstanceDesc.pInstanceSRV));
	if(FAILED(m_pDissolveTexCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture")))
			return E_FAIL;
	
	if(FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	_int iMeshSize = m_pModelCom->Get_NumMeshes();
	
	for (_int i = 0; i < iMeshSize; ++i)
	{
		if(FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i)))
			return E_FAIL;
		
		if(FAILED(m_pModelCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", (_uint)i, aiTextureType_DIFFUSE)))
			return E_FAIL;
		//m_pModelCom->Bind_ShaderResource(m_pShaderCom, "g_NormalTexture", (_uint)i, aiTextureType_NORMALS); /* �븻 �ؽ��� �߰� */
		

		if (3 == m_iRenderPass)
		{
			if(FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fTimeDelta, sizeof(_float))))
				return E_FAIL;

			if(FAILED(m_pDissolveTexCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture")))
				return E_FAIL;
		}

		

		if(FAILED(m_pShaderCom->Bind_SRV("g_InstanceTransform", m_tInstanceDesc.pInstanceSRV)))
			return E_FAIL;

		if(FAILED(m_pShaderCom->Begin(m_iRenderPass)))
			return E_FAIL;

		if(FAILED(m_pInstanceModelCom->Render(m_pModelCom->Get_Mesh_For_Index(i), i)))
			return E_FAIL;

		
//		m_pModelCom->Render(_uint(i));
	}

//	ID3D11ShaderResourceView* pInstanceSRV = { nullptr };
	
	return S_OK;
}



HRESULT CInstanceMonster::Ready_Components()
{
	_int iCurrentLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Shader */
	{
		if (FAILED(__super::Add_Component(iCurrentLevel, TEXT("Prototype_Component_Shader_AnimModel_Instance"),
			TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
			return E_FAIL;
	}


	{
		if (FAILED(__super::Add_Component(iCurrentLevel, TEXT("Prototype_Component_Model_Fiona"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
	}

	/* For. Texture */
	{
		if (FAILED(__super::Add_Component(iCurrentLevel, TEXT("Prototype_Component_Texture_Dissolve"),
			TEXT("Com_DissolveTex"), reinterpret_cast<CComponent**>(&m_pDissolveTexCom))))
			return E_FAIL;
	}

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC		BoundingDesc = {};
	{
		BoundingDesc.vExtents = _float3(0.5f, 0.7f, 0.5f);
		BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);

		if (FAILED(__super::Add_Component(iCurrentLevel, TEXT("Prototype_Component_Collider_AABB"),
			TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CInstanceMonster::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstanceMonster::Ready_Instance()
{
	_uint iCurrentLevel = m_pGameInstance->Get_NextLevel();

	if(FAILED(Create_AnimationTexture()))
		return E_FAIL;



// 	if (FAILED(__super::Add_Component(iCurrentLevel, TEXT("Prototype_Component_Shader_AnimModel_Instance"),
// 		TEXT("Com_InstanceShader"), reinterpret_cast<CComponent**>(&m_tInstanceDesc.pInstanceShader))))
// 		return E_FAIL;

	//! ����

	//Instance
	
// 		size_t iSizePerInstacne = sizeof(_uint) + sizeof(_float4x4);
// 
// 		
// 		//BYTE* pData = new BYTE[iSizePerInstacne * m_tInstanceDesc.iMaxInstanceCount];
// 		//ZeroMemory(pData, iSizePerInstacne * m_tInstanceDesc.iMaxInstanceCount);
// 
// 		m_tInstanceDesc.pByte = new BYTE[iSizePerInstacne * m_tInstanceDesc.iMaxInstanceCount];
// 		ZeroMemory(m_tInstanceDesc.pByte, iSizePerInstacne * m_tInstanceDesc.iMaxInstanceCount);
// 
// 		m_tInstanceDesc.iSizePerSecond = iSizePerInstacne;
// 	
// 	//! �ؽ�ó
// 		_uint iBoneSize = m_pModelCom->Get_Bones()->size();
// 		//_float4x4* pMatrix = new _float4x4[m_tInstanceDesc.iMaxInstanceCount * iBoneSize];
// 		//ZeroMemory(pMatrix, sizeof(_float4x4) * (m_tInstanceDesc.iMaxInstanceCount * iBoneSize));
// 		
// 		m_tInstanceDesc.pMatrix = new _float4x4[m_tInstanceDesc.iMaxInstanceCount * iBoneSize];
// 		ZeroMemory(m_tInstanceDesc.pMatrix, sizeof(_float4x4) * (m_tInstanceDesc.iMaxInstanceCount * iBoneSize));
// 
// 		
// 
// 		D3D11_TEXTURE2D_DESC	TextureDesc;
// 		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
// 
// 		TextureDesc.Width = iBoneSize * 4;
// 		TextureDesc.Height = m_tInstanceDesc.iMaxInstanceCount;
// 		TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16����Ʈ
// 		TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
// 		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
// 		TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
// 		TextureDesc.MipLevels = 1;
// 		TextureDesc.SampleDesc.Count = 1;
// 		TextureDesc.ArraySize = 1;
// 
// 		D3D11_SUBRESOURCE_DATA tSubResources;
// 
// 		tSubResources.pSysMem = m_tInstanceDesc.pMatrix;
// 		tSubResources.SysMemPitch = iBoneSize * sizeof(_float4x4);
// 
// 		
// 		if(FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &tSubResources, &m_tInstanceDesc.pInstanceTexture)))
// 			return E_FAIL;
// 
// 
// 		D3D11_TEXTURE2D_DESC Desc;
// 		m_tInstanceDesc.pInstanceTexture->GetDesc(&Desc);
// 
// 		if (Desc.Width == 0 || Desc.Height == 0 || Desc.Format == DXGI_FORMAT_UNKNOWN)
// 		{
// 			return E_FAIL;
// 		}
// 
// 		
// 
// 
// 	// Create SRV
// 		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
// 		ZeroMemory(&desc, sizeof(desc));
// 		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
// 		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
// 		desc.Texture2D.MipLevels = 1;
// 
// 		if (FAILED(m_pDevice->CreateShaderResourceView(m_tInstanceDesc.pInstanceTexture, &desc, &m_tInstanceDesc.pInstanceSRV)))
// 			return E_FAIL;
// 	
// 
// 	CVIBuffer_AnimModel_Instance::ANIMINSTANCE_BUFFER_DESC InstanceBufferDesc = {};
// 
// 	InstanceBufferDesc.Desc = m_tInstanceDesc;
// 
// 	if (FAILED(__super::Add_Component(iCurrentLevel, TEXT("Prototype_Component_VIBuffer_AnimModel_Instance"),
// 		TEXT("Com_InstanceBuffer"), reinterpret_cast<CComponent**>(&m_pInstanceModelCom), &InstanceBufferDesc)))
// 		return E_FAIL;



	return S_OK;

	//!�Ʒ� ������ �ӽú���
	//! // 		{
// 			D3D11_BUFFER_DESC			BufferDesc;
// 
// 			ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));
// 
// 			// m_BufferDesc.ByteWidth = �����ϳ��� ũ��(Byte) * ������ ����;
// 			BufferDesc.ByteWidth = m_tInstanceDesc.iMaxInstanceCount * (_uint)iSizePerInstacne;
// 			BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* �������۷� �Ҵ��Ѵ�. (Lock, unLock ȣ�� �Ұ�)*/
// 			BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
// 			BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
// 			BufferDesc.MiscFlags = 0;
// 			BufferDesc.StructureByteStride = (_uint)iSizePerInstacne;
// 
// 			D3D11_SUBRESOURCE_DATA		InitialData;
// 
// 			InitialData.pSysMem = m_tInstanceDesc.pByte;
// 
// 			if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &InitialData, m_tInstanceDesc.pInstanceBuffer)))
// 				return E_FAIL;
// 		}

}

HRESULT CInstanceMonster::Create_AnimationTexture()
{
	_uint iMaxBoneSize = m_pModelCom->Get_Bones()->size();
	_uint iTexelsPerBone = 4;

	_uint iPixelCount = iMaxBoneSize * iTexelsPerBone;
	_uint texWidth = 0;
	_uint texHeight = 0;

	texWidth = (_int)sqrt((float)iPixelCount) + 1;
	texHeight = 1;

	while (texHeight < texWidth)
		texHeight = texHeight << 1;
	texWidth = texHeight;

	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.MipLevels = 1;
	TextureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.ArraySize = 1;
	TextureDesc.Width = texWidth;
	TextureDesc.Height = texHeight;

	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	_uint iBufferSize = texHeight * texWidth * sizeof(_float4);

	_float4* pData = new _float4[TextureDesc.Width * TextureDesc.Height];
	memset((void*)pData, 0, iBufferSize);

	_float4* pCurrentDataPtr = pData;

	vector<CBone*> Bones = *m_pModelCom->Get_Bones();



	for (_int i = 0; i < iMaxBoneSize; ++i)
	{
		_float4x4* pFrameMatrices = new float4x4;
		XMStoreFloat4x4(pFrameMatrices, Bones[i]->Get_CombinedTransformationMatrix());

		_matrix pMatrix = XMLoadFloat4x4(pFrameMatrices);

		_float4 vRight, vUp, vLook, vPosition;

		XMStoreFloat4(&vRight, pMatrix.r[0]);
		XMStoreFloat4(&vUp, pMatrix.r[1]);
		XMStoreFloat4(&vLook, pMatrix.r[2]);
		XMStoreFloat4(&vPosition, pMatrix.r[3]);

		memcpy(pCurrentDataPtr++, &vRight, sizeof(_float4));
		memcpy(pCurrentDataPtr++, &vUp, sizeof(_float4));
		memcpy(pCurrentDataPtr++, &vLook, sizeof(_float4));
		memcpy(pCurrentDataPtr++, &vPosition, sizeof(_float4));
	}

	D3D11_SUBRESOURCE_DATA SubResources = {};

	SubResources.pSysMem = (void*)pData;
	SubResources.SysMemPitch = texWidth * (sizeof(_float4));
	SubResources.SysMemSlicePitch = 1;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResources, &m_tInstanceDesc.pInstanceTexture)))
		return E_FAIL;

	Safe_Delete(pData);

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

	SRVDesc.Format = TextureDesc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = TextureDesc.MipLevels;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_tInstanceDesc.pInstanceTexture, &SRVDesc, &m_tInstanceDesc.pInstanceSRV)))
		return E_FAIL;

	return S_OK;
}

void CInstanceMonster::Add_InstanceData(_uint iSize, _uint& iIndex)
{
	{
		_float4x4* pAnimInstanceValue = m_tInstanceDesc.pMatrix;

		_uint iBoneSize = m_pModelCom->Get_Bones()->size();

		size_t iSizePerInstance = iBoneSize * sizeof(_float4x4);
		_uint iDataIndex = iIndex * iBoneSize;

		vector<CBone*> Bones = *m_pModelCom->Get_Bones();
		

		vector<_float4x4> BoneMatrices;
		BoneMatrices.reserve(Bones.size());

		for (_int i = 0; i < Bones.size(); ++i)
		{
			BoneMatrices.push_back(Bones[i]->Get_CombinedTransformationMatrix());
		}

		memcpy(pAnimInstanceValue + iDataIndex, BoneMatrices.data(), iSizePerInstance);
	}

}

CInstanceMonster* CInstanceMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CInstanceMonster* pInstance = new CInstanceMonster(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CInstanceMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CInstanceMonster::Clone(void* pArg)
{
	CInstanceMonster* pInstance = new CInstanceMonster(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CInstanceMonster");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CInstanceMonster::Free()
{
	__super::Free();

	Safe_Release(m_pDissolveTexCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
