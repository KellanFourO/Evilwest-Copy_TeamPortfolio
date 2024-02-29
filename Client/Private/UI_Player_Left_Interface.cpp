#include "stdafx.h"
#include "UI_Player_Left_Interface.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_Player_Left_Interface::CUI_Player_Left_Interface(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Player_Left_Interface::CUI_Player_Left_Interface(const CUI_Player_Left_Interface& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_Left_Interface::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Player_Left_Interface::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	/* �ؽ�ó ������ */
	m_tUIInfo.fScaleX = g_iWinSizeX;
	m_tUIInfo.fScaleY = g_iWinSizeY;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_Player_Left_Interface::Priority_Tick(_float fTimeDelta)
{
	//if (!m_vecUIParts.empty())
	//{
	//	for (auto& pUIParts : m_vecUIParts)
	//	{
	//		pUIParts->Priority_Tick(fTimeDelta);
	//	}
	//}

}

void CUI_Player_Left_Interface::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//if (!m_vecUIParts.empty())
	//{
	//	for (auto& pUIParts : m_vecUIParts)
	//	{
	//		pUIParts->Tick(fTimeDelta);
	//	}
	//}
}

void CUI_Player_Left_Interface::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	//if (!m_vecUIParts.empty())
	//{
	//	for (auto& pUIParts : m_vecUIParts)
	//	{
	//		pUIParts->Late_Tick(fTimeDelta);
	//	}
	//}


	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_Player_Left_Interface::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//if (!m_vecUIParts.empty())
	//{
	//	for (auto& pUIParts : m_vecUIParts)
	//	{
	//		pUIParts->Render();
	//	}
	//}

	//! �� ���̴��� 0��° �н��� �׸��ž�.
	m_pShaderCom->Begin(0); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

	//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
	m_pVIBufferCom->Bind_VIBuffers();

	//! ���ε��� ����, �ε����� �׷�
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Player_Left_Interface::Ready_Components()
{
	if (FAILED(__super::Ready_Components())); // Ready : Texture / MapTexture

	//! For.Com_Texture1 // �÷��̾� ���� �������̽� ������
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Player_LeftInterface"),
		TEXT("Com_Texture_Player_LeftInterface"), reinterpret_cast<CComponent**>(&m_pTextureCom[LEFT_INTERFACE]))))
		return E_FAIL;

	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* ȿ���� �ʿ��� �༮�� Map�ؽ��ĵ� �߰����ֱ� */
	return S_OK;
}

HRESULT CUI_Player_Left_Interface::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	for (_int i = (_int)0; i < (_int)m_eTexture_Kind; ++i)
	{
		switch (i)
		{
			case CUI_Player_Left_Interface::LEFT_INTERFACE:
			{
				if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
					return E_FAIL;
				break;
			}
			case CUI_Player_Left_Interface::TEXTURE_END:
				break;
			default:
				break;
		}
	}

	return S_OK;
}

void CUI_Player_Left_Interface::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_Player_Left_Interface::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_Player_Left_Interface::Save_Desc(json& out_json)
{
	__super::Save_Desc(out_json);

//	json Out_Json;
//	_ushort iIndex = 1; // ��ü �ѹ� ������ 1���� ����
//
//#pragma region ��ü �ѹ� ����
//	json This_Object;
//	__super::Save_Desc(This_Object);
//	Out_Json.emplace(to_string(0), This_Object);
//#pragma endregion
//
//#pragma region ������ ����
//	for (auto& UI : m_vecUIParts)
//	{
//		json Object;
//
//		dynamic_cast<CUI*>(UI)->Save_Desc(Object);
//		Out_Json.emplace(to_string(iIndex++), Object);
//	}
//#pragma endregion
//	
//	CJson_Utility::Save_Json("../Bin/DataFiles/Data_UI/UI_Player_Left_Interface.json", Out_Json);

	return out_json;
}

void CUI_Player_Left_Interface::Load_Desc()
{

}

CUI_Player_Left_Interface* CUI_Player_Left_Interface::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Player_Left_Interface* pInstance = new CUI_Player_Left_Interface(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_Left_Interface");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_Left_Interface::Clone(void* pArg)
{
	CUI_Player_Left_Interface* pInstance = new CUI_Player_Left_Interface(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_Left_Interface");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_Left_Interface::Pool()
{
	return new CUI_Player_Left_Interface(*this);
}

void CUI_Player_Left_Interface::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
