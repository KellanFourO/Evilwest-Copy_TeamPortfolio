#include "stdafx.h"
#include "UI_Player_Skill_Frame.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_Player_Skill_Frame::CUI_Player_Skill_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Player_Skill_Frame::CUI_Player_Skill_Frame(const CUI_Player_Skill_Frame& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_Skill_Frame::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Player_Skill_Frame::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	return S_OK;
}

void CUI_Player_Skill_Frame::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Player_Skill_Frame::Tick(_float fTimeDelta)
{

}

void CUI_Player_Skill_Frame::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	__super::Tick(fTimeDelta);

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_Player_Skill_Frame::Render()
{
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

HRESULT CUI_Player_Skill_Frame::Ready_Components()
{
	if (FAILED(__super::Ready_Components())); // Ready : Texture / MapTexture

	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	////! For.Com_Texture_1
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_element_left"), // HP_Bar_Red
	//	TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CUI_Player_Skill_Frame::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	for (auto& iter : m_pTextureCom)
	{
		if (FAILED(iter->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	return S_OK;
}

json CUI_Player_Skill_Frame::Save_Desc(json& out_json)
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

void CUI_Player_Skill_Frame::Load_Desc()
{

}

CUI_Player_Skill_Frame* CUI_Player_Skill_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Player_Skill_Frame* pInstance = new CUI_Player_Skill_Frame(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_Skill_Frame");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Player_Skill_Frame::Pool()
{
	return new CUI_Player_Skill_Frame(*this);
}

CGameObject* CUI_Player_Skill_Frame::Clone(void* pArg)
{
	CUI_Player_Skill_Frame* pInstance = new CUI_Player_Skill_Frame(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_Skill_Frame");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Player_Skill_Frame::Free()
{
	__super::Free();

	//Safe_Release(m_pVIBufferCom);
	//Safe_Release(m_pShaderCom);

	for (auto& pTexture : m_pTextureCom)
	{
		if(pTexture != nullptr)
			Safe_Release(pTexture);
			//Safe_Release(pTexture);
	}

}