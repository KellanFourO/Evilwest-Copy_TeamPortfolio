#include "stdafx.h"
#include "UI_Player_HPBar.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_Player_HPBar::CUI_Player_HPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{

}

CUI_Player_HPBar::CUI_Player_HPBar(const CUI_Player_HPBar& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_HPBar::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Player_HPBar::Initialize(void* pArg)
{
	m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_Player_HPBar::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Player_HPBar::Tick(_float fTimeDelta)
{

}

void CUI_Player_HPBar::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	__super::Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(m_tUIInfo.eRenderGroup, this)))
		return;
}

HRESULT CUI_Player_HPBar::Render()
{
	//TODO ���̴����� ����� �����ִ� ������ �ݵ�� ���̴��� ����Լ��� ȣ���ϱ� ������ �ؾ��Ѵ�.
	//! �� ������, ���̴��� ����Լ� ������ pPass->Apply(0, m_prContext); �ڵ带 �����Ѵ�.
	//! Apply ȣ�� �Ŀ� ����� �����൵ ������ ���� ������, �������� ��������.
	//! Apply ȣ�� �Ŀ� ����� �����ָ�, � ������ ���������� ����ǰ�, � ������ ���� ����� �� �Ѿ�� ��찡 �ִ�.

	//switch (m_tUIInfo.eUIType)
	//{
	//case CUI_Player_HPBar::SMALL:
	//case CUI_Player_HPBar::MID:
	//case CUI_Player_HPBar::LARGE:
	//case CUI_Player_HPBar::SIDE:
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
	//case CUI_Player_HPBar::BOSS:
	//case CUI_Player_HPBar::NONE:
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

HRESULT CUI_Player_HPBar::Ready_Components()
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

HRESULT CUI_Player_HPBar::Bind_ShaderResources()
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

void CUI_Player_HPBar::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_Player_HPBar::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_Player_HPBar::Save_Desc(json& out_json)
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

void CUI_Player_HPBar::Load_Desc()
{

}

CUI_Player_HPBar* CUI_Player_HPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Player_HPBar* pInstance = new CUI_Player_HPBar(pDevice, pContext);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_HPBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_HPBar::Clone(void* pArg)
{
	CUI_Player_HPBar* pInstance = new CUI_Player_HPBar(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_HPBar");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Player_HPBar::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);

}
