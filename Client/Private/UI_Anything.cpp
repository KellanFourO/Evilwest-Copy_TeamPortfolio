#include "stdafx.h"
#include "UI_Anything.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Texture.h"

CUI_Anything::CUI_Anything(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Anything::CUI_Anything(const CUI_Anything& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Anything::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Anything::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_Anything::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Anything::Tick(_float fTimeDelta)
{

}

void CUI_Anything::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	__super::Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_Anything::Render()
{
	//TODO ���̴����� ����� �����ִ� ������ �ݵ�� ���̴��� ����Լ��� ȣ���ϱ� ������ �ؾ��Ѵ�.
	//! �� ������, ���̴��� ����Լ� ������ pPass->Apply(0, m_prContext); �ڵ带 �����Ѵ�.
	//! Apply ȣ�� �Ŀ� ����� �����൵ ������ ���� ������, �������� ��������.
	//! Apply ȣ�� �Ŀ� ����� �����ָ�, � ������ ���������� ����ǰ�, � ������ ���� ����� �� �Ѿ�� ��찡 �ִ�.

	//switch (m_tUIInfo.eUIType)
	//{
	//case CUI_Anything::SMALL:
	//case CUI_Anything::MID:
	//case CUI_Anything::LARGE:
	//case CUI_Anything::SIDE:
	//{
	//	///* ������� ���� ��ġ�� ���� UI�� ī�޶� ����ü �ȿ� ������ ��쿡�� ǥ���ϱ� ���� */
	//	//if (m_fOwnerCamDistance > 40.f || false == In_Frustum())
	//	//{
	//	//	// m_pGameInstance->Get_CamDir();
	//	//	return E_FAIL;
	//	//}
	//	//__super::SetUp_WorldToScreen(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION));

	//	//__super::SetUp_Transform(m_fWorldToScreenX, m_fWorldToScreenY, m_fDefaultScale * m_fScaleOffsetX, m_fDefaultScale * m_fScaleOffsetY);


	//	//__super::SetUp_BillBoarding();
	//	break;
	//}
	//case CUI_Anything::BOSS:
	//case CUI_Anything::NONE:
	//	break;
	//default:
	//	break;
	//}

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//! �� ���̴��� 0��° �н��� �׸��ž�.
	m_pShaderCom->Begin(0); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

	//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
	m_pVIBufferCom->Bind_VIBuffers();

	//! ���ε��� ����, �ε����� �׷�
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Anything::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	wstring strPrototag;
	m_pGameInstance->String_To_WString(m_tUIInfo.strProtoTag, strPrototag);

	//! For.Com_Texture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Anything::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Anything::Set_ParentTransform(CTransform* pParentTransformCom)
{
	m_tUIInfo.pParentTransformCom = pParentTransformCom;
	return S_OK;
}

void CUI_Anything::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_Anything::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_Anything::Save_Desc(json& out_json)
{
	_float fSizeX = 0.f;
	_float fSizeY = 0.f;
	_float fPositionX = 0.f;
	_float fPositionY = 0.f;

	_float fCurPosX = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0];
	_float fCurPosY = m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1];

	fCurPosX = fCurPosX + (_float)g_iWinSizeX * 0.5f;
	fCurPosY = (_float)g_iWinSizeY * 0.5f - fCurPosY;

	out_json["CloneTag"] = m_tUIInfo.strCloneTag;

	out_json["ProtoTag"] = m_tUIInfo.strProtoTag;

	out_json["FilePath"] = m_tUIInfo.strFilePath;

	m_pTransformCom->Write_Json(out_json);

	return out_json;
}

void CUI_Anything::Load_Desc()
{

}

CUI_Anything* CUI_Anything::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Anything* pInstance = new CUI_Anything(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Anything");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Anything::Clone(void* pArg)
{
	CUI_Anything* pInstance = new CUI_Anything(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Anything");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Anything::Pool()
{
	return new CUI_Anything(*this);
}

void CUI_Anything::Free()
{
	__super::Free();

	if(m_pTextureCom)
		Safe_Release(m_pTextureCom);

}


