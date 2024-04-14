#include "stdafx.h"
#include "UI_TalkBox.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_TalkBox::CUI_TalkBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI_Text(pDevice, pContext, strPrototypeTag)
{

}

CUI_TalkBox::CUI_TalkBox(const CUI_TalkBox& rhs)
	: CUI_Text(rhs)
{
}

HRESULT CUI_TalkBox::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_TalkBox::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//if (FAILED(Ready_Text())) //! �ؽ�Ʈ ����
	//	return E_FAIL;

	//if (FAILED(Find_Change("Default"))) //! �ؽ�Ʈ ����
	//	return E_FAIL;

	m_bActive = false;

	return S_OK;
}

void CUI_TalkBox::Priority_Tick(_float fTimeDelta)
{

}

void CUI_TalkBox::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	//Check_Disappear(fTimeDelta);

	if (m_bActive == true)
	{

	}
}

void CUI_TalkBox::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_TalkBox::Render()
{
	if (m_bActive == true)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		//! �� ���̴��� 0��° �н��� �׸��ž�.
		m_pShaderCom->Begin(0); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

		//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
		m_pVIBufferCom->Bind_VIBuffers();

		//! ���ε��� ����, �ε����� �׷�
		m_pVIBufferCom->Render();

		//if (!m_bFreeMove)
		//{
		//	m_fPosX = m_pTransformCom->Get_Position().x + (g_iWinsizeX / 2.f);
		//	m_fPosY = -m_pTransformCom->Get_Position().y + (g_iWinsizeY / 2.f);
		//}

		//if (m_pTextInfo != nullptr && m_bTrigger == true)
		//{
		//	RenderTextWithLineBreak(m_pGameInstance->Convert_WString_To_String(m_strText), 10);
		//	m_pGameInstance->Render_Font(m_strFontTag, m_strText, _float2(m_fPosX, m_fPosY), m_vColor, m_fScale, m_vOrigin, m_fRotation);
		//}
	}

	return S_OK;
}

void CUI_TalkBox::UI_Ready(_float fTimeDelta)
{
}

void CUI_TalkBox::UI_Enter(_float fTimeDelta)
{
}

void CUI_TalkBox::UI_Loop(_float fTimeDelta)
{
}

void CUI_TalkBox::UI_Exit(_float fTimeDelta)
{
}

void CUI_TalkBox::Add_Text(string strTextKey, string strFontTag, string strText, _float fPosX, _float fPosY, _vector vColor, _float fScale, _float2 vOrigin, _float fRotation)
{
	__super::Add_Text(strTextKey, strFontTag, strText, fPosX, fPosY, vColor, fScale, vOrigin, fRotation);
}

HRESULT CUI_TalkBox::Find_Change(const string& strTextTag)
{
	__super::Find_Change(strTextTag);
	return S_OK;
}

HRESULT CUI_TalkBox::Ready_Components()
{
	FAILED_CHECK(__super::Ready_Components()); // Ready : Texture / MapTexture


	//! For.Com_Texture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("TutorialText"),
		TEXT("Com_Texture_Text"), reinterpret_cast<CComponent**>(&m_pTextureCom[TUTORIALTEXT]))))
		return E_FAIL;

	/* ȿ���� �ʿ��� �༮�� Map�ؽ��ĵ� �߰����ֱ� */
	return S_OK;
}

HRESULT CUI_TalkBox::Bind_ShaderResources()
{

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;


	if (FAILED(m_pTextureCom[TUTORIALTEXT]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture_Front", m_iTextNum)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_TalkBox::Ready_Text()
{
	/* �Ľ� ���� �ޱ� */
	TEXTINFO* LoadInfo = new TEXTINFO;

	/* ���� �� (�� �� �ε��ؼ� �ޱ�) */
	LoadInfo->fPosX = 72.f;
	LoadInfo->fPosY = 103.f;
	LoadInfo->fScale = 0.5f;
	LoadInfo->vOrigin.x = 0.f;
	LoadInfo->vOrigin.y = 0.f;
	LoadInfo->fRotation = 0.f;
	LoadInfo->strTextKey = "Default";
	LoadInfo->strFontTag = "Font_EvilWest";
	LoadInfo->strText = "Hello Wordl";
	LoadInfo->vColor.m128_f32[0] = 1.f;
	LoadInfo->vColor.m128_f32[1] = 1.f;
	LoadInfo->vColor.m128_f32[2] = 1.f;
	LoadInfo->vColor.m128_f32[3] = 1.f;

	m_mapText.emplace(LoadInfo->strTextKey, LoadInfo);

	TEXTINFO* pTextInfo = nullptr;

	return S_OK;
}

void CUI_TalkBox::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_TalkBox::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_TalkBox::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);


	/* �߰����� ���� */


	return out_json;
}

void CUI_TalkBox::Load_Desc()
{

}

CUI_TalkBox* CUI_TalkBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_TalkBox* pInstance = new CUI_TalkBox(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_TalkBox");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_TalkBox::Clone(void* pArg)
{
	CUI_TalkBox* pInstance = new CUI_TalkBox(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_TalkBox");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_TalkBox::Pool()
{
	return new CUI_TalkBox(*this);
}

void CUI_TalkBox::Free()
{
	__super::Free();
}
