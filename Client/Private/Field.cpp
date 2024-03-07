#include "stdafx.h"
#include "Field.h"
#include "GameInstance.h"



CField::CField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CGameObject(pDevice, pContext, strPrototypeTag)
{
	m_bIsPoolObject = false;
}

CField::CField(const CField& rhs)
	: CGameObject(rhs)
{
}

HRESULT CField::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CField::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	DINFO* pInfo = (DINFO*)pArg;

	// ĳ���õ� ����ü�� �� ���
	DINFO Info = { pInfo->fX, pInfo->fY, pInfo->fZ };

	Info.vecVertexInfo = pInfo->vecVertexInfo;
	
	// �ּҰ� �ƴ�, ���� ��� ����ü �Ѱ��ֱ�
	ReceiveInfo(Info);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CField::Priority_Tick(_float fTimeDelta)
{
}

void CField::Tick(_float fTimeDelta)
{
	m_pShaderCom->Bind_RawValue("g_vBrushPos", &m_fPickingPos, sizeof(_float4));
}

void CField::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this)))
		return;
}

HRESULT CField::Render()
{
	//TODO ���̴����� ����� �����ִ� ������ �ݵ�� ���̴��� ����Լ��� ȣ���ϱ� ������ �ؾ��Ѵ�.
	//! �� ������, ���̴��� ����Լ� ������ pPass->Apply(0, m_pContext); �ڵ带 �����Ѵ�.
	//! Apply ȣ�� �Ŀ� ����� �����൵ ������ ���� ������, �������� ��������.
	//! Apply ȣ�� �Ŀ� ����� �����ָ�, � ������ ���������� ����ǰ�, � ������ ���� ����� �� �Ѿ�� ��찡 �ִ�.
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//! �� ���̴��� 0��° �н��� �׸��ž�.
	m_pShaderCom->Begin(1);

	if (m_pVIBufferCom != nullptr) // ���� �����Ҷ� �����ϰ� ��������� ������ Ÿ�� �����⶧����, ����ó�� �ص���.
	{
		//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
		m_pVIBufferCom->Bind_VIBuffers();

		//! ���ε��� ����, �ε����� �׷�
		m_pVIBufferCom->Render();
	}

	return S_OK;
}

// void CField::Write_Json(json& Out_Json)
// {
// 	__super::Write_Json(Out_Json);
// 
// 	
// 	Out_Json.emplace("SizeX", m_tDynamicInfo.fX);
// 	Out_Json.emplace("SizeY", m_tDynamicInfo.fY);
// 	Out_Json.emplace("SizeZ", m_tDynamicInfo.fZ);
// }
// 
// void CField::Load_FromJson(const json& In_Json)
// {
// 	__super::Load_FromJson(In_Json);
// 
// 	
// 
// }



void CField::Picking_Terrain(RAY WorldRay, EDIT_MODE eMode)
{

	if (m_pVIBufferCom->Compute_MousePos(WorldRay,m_pTransformCom->Get_WorldMatrix(), &m_fPickingPos))
	{
		m_pVIBufferCom->Update(XMLoadFloat3(&m_fPickingPos), m_fDrawRadious, m_fPower, (_uint)eMode);
	}
	
}

_float3 CField::GetMousePos(RAY WorldRay)
{

	if (m_pVIBufferCom->Compute_MousePos(WorldRay, m_pTransformCom->Get_WorldMatrix(), &m_fPickingPos))
	{
		m_bOnMouse = true;
		return m_fPickingPos;
	}
	else
		m_bOnMouse = false;

	return _float3();
}

_bool CField::MouseOnTerrain()
{
	return m_bOnMouse;	
}

void CField::Update(_fvector vMousePos, _bool bMode)
{
	return m_pVIBufferCom->Update(vMousePos, bMode);
}

void CField::Move_Field(_float3 vAddPos)
{
	return m_pVIBufferCom->Move_Field(vAddPos);
}


HRESULT CField::Ready_Components()
{

	LEVEL eCurrentLevel = (LEVEL)m_pGameInstance->Get_NextLevel();

	//! For.Com_Shader
	if (FAILED(__super::Add_Component(eCurrentLevel, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer_Dynamic
	if (FAILED(__super::Add_Component(eCurrentLevel, TEXT("Prototype_Component_VIBuffer_Field"),	// �������� ����
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), &m_tDynamicInfo)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(eCurrentLevel, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_DIFFUSE]))))
		return E_FAIL;

	/* For.Com_Mask */ // ������ �������� Mask // ���� : �� ������ ID�� �����ʾƼ� �ͷ��� ������ �����߾���, ����� ������ �ȵǴ�, Imgui shutdown�� �� ������
	if (FAILED(__super::Add_Component(eCurrentLevel, TEXT("Prototype_Component_Texture_Terrain_Mask"),
		TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_MASK]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(eCurrentLevel, TEXT("Prototype_Component_Texture_Terrain_Brush"),
		TEXT("Com_Brush"), reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BRUSH]))))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CField::Bind_ShaderResources()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	_float4 vColor = { 0.f, 1.f, 0.f, 1.f };

	// ����ũ
	if (FAILED(m_pTextureCom[TYPE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;

	//// �귯��
	if (FAILED(m_pTextureCom[TYPE_BRUSH]->Bind_ShaderResource(m_pShaderCom, "g_BrushTexture", 0))) // error : 
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBrushPos", &m_fPickingPos, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fBrushRange", &m_fDrawRadious, sizeof(float))))
		return E_FAIL;
	//if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))	// ����
	//	return E_FAIL;

	return S_OK;
}

void CField::ReceiveInfo(DINFO pInfo)
{
	m_tDynamicInfo = { pInfo.fX, pInfo.fY, pInfo.fZ };
	m_tDynamicInfo.vecVertexInfo = pInfo.vecVertexInfo;
}

// void CField::Delete_Component(const wstring& strComTag)
// {
// 	__super::Delete_Component(strComTag);
// 
// 	// ! ������ �ȵɶ� !
// 	// Delete_Component������ ����� �༮�� Safe_Release�� ���۷��� ī��Ʈ�� ���̱⸸ �Ѱ��̶�,
// 	// ����� Ÿ���� Ȯ���ϰ� ���⼭ ��������� ���� ��������Ѵ�.
// 
// 	if(m_pVIBufferCom)
// 	Safe_Release(m_pVIBufferCom);
// 	
// 
// }

CField* CField::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CField* pInstance = new CField(pDevice, pContext, strPrototypeTag);

	/* ���� ��ü�� �ʱ�ȭ�Ѵ�. */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CField");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CField::Clone(void* pArg) // ���⼭ �纻�� ���鶧 ������ ������ ���� �޾��ش�.
{
	CField* pInstance = new CField(*this);

	/* �纻 ��ü�� �ʱ�ȭ�Ѵ�. */
	if (FAILED(pInstance->Initialize(pArg))) // �Ѱ���
	{
		MSG_BOX("Failed to Cloned : CField");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Engine::CGameObject* CField::Pool()
{
	return new CField(*this);
}

void CField::Free()
{
	__super::Free();

	
	Safe_Release(m_pVIBufferCom);

	Safe_Release(m_pShaderCom);

	//Safe_Release(m_pTextureCom);
	// �迭 ����, ->Release ���� : �ؽ�ó ������Ʈ�� �迭 �������� �ٲ�� ������, �ϳ��ϳ� ���� �����ֵ��� �ٲ�����Ѵ�.
	for (size_t i = 0; i < TYPE_END; i++)
	{
		Safe_Release(m_pTextureCom[i]);
	}
}


