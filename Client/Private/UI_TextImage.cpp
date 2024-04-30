#include "stdafx.h"
#include "UI_TextImage.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_TextImage::CUI_TextImage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI_Text(pDevice, pContext, strPrototypeTag)
{

}

CUI_TextImage::CUI_TextImage(const CUI_TextImage& rhs)
	: CUI_Text(rhs)
{
}

HRESULT CUI_TextImage::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_TextImage::Initialize(void* pArg)
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

	m_iTextNum = 0;

	return S_OK;
}

void CUI_TextImage::Priority_Tick(_float fTimeDelta)
{

}

void CUI_TextImage::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	//Check_Disappear(fTimeDelta);

	if (m_bActive == true)
	{	//
		//if (m_pGameInstance->Key_Down(DIK_4))
		//	m_iTextNum = 0;
		//if (m_pGameInstance->Key_Down(DIK_5))
		//{
		//	m_iTextNum = 1;
		//	m_fTime = (_float)GetTickCount64();
		//}
		//if (m_pGameInstance->Key_Down(DIK_6))
		//{
		//	m_iTextNum = 2;
		//	m_fTime = (_float)GetTickCount64();
		//}

		if (m_iTextNum != 0)
		{
			if (m_fTime + 1500.f < GetTickCount64())
			{
				m_iTextNum = 0;
			}
		}

		_float3 vPosition = { 0.f, 0.f, 0.5f };
		if (m_iTextNum == 0)
		{
			vPosition = { 420.f, -250.f, 0.5f };
		}
		else if (m_iTextNum == 1)
		{
			vPosition = { 435.f, -250.f, 0.5f };
		}
		else if (m_iTextNum == 2)
		{
			vPosition = { 427.f, -250.f, 0.5f };
		}

		m_pTransformCom->Set_Position(vPosition);
	}
}

void CUI_TextImage::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_TextImage::Render()
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

void CUI_TextImage::UI_Ready(_float fTimeDelta)
{
}

void CUI_TextImage::UI_Enter(_float fTimeDelta)
{
}

void CUI_TextImage::UI_Loop(_float fTimeDelta)
{
}

void CUI_TextImage::UI_Exit(_float fTimeDelta)
{
}

void CUI_TextImage::Add_Text(string strTextKey, string strFontTag, string strText, _float fPosX, _float fPosY, _vector vColor, _float fScale, _float2 vOrigin, _float fRotation)
{
	__super::Add_Text(strTextKey, strFontTag, strText, fPosX, fPosY, vColor, fScale, vOrigin, fRotation);
}

HRESULT CUI_TextImage::Find_Change(const string& strTextTag)
{
	__super::Find_Change(strTextTag);
	return S_OK;
}

HRESULT CUI_TextImage::Ready_Components()
{
	FAILED_CHECK(__super::Ready_Components()); // Ready : Texture / MapTexture

	//! For.Com_Texture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Button_Message"),
		TEXT("Com_Texture_Text"), reinterpret_cast<CComponent**>(&m_pTextureCom[RWARD_BUTTON_MESSAGE]))))
		return E_FAIL;

	/* ȿ���� �ʿ��� �༮�� Map�ؽ��ĵ� �߰����ֱ� */
	return S_OK;
}

HRESULT CUI_TextImage::Bind_ShaderResources()
{

	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[RWARD_BUTTON_MESSAGE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_TextImage::Ready_Text()
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

void CUI_TextImage::Compute_OwnerCamDistance()
{
	//_vector		vPosition = m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION);
	//_vector		vCamPosition = XMLoadFloat4(&m_pGameInstance->Get_CamPosition());

	//m_fOwnerCamDistance = XMVectorGetX(XMVector3Length(vPosition - vCamPosition));
}

_bool CUI_TextImage::In_Frustum()
{
	return false;
	//return m_pGameInstance->isIn_WorldPlanes(m_tUIInfo.pOwnerTransform->Get_State(CTransform::STATE_POSITION), 2.f);
}

json CUI_TextImage::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);


	/* �߰����� ���� */


	return out_json;
}

void CUI_TextImage::Load_Desc()
{

}

CUI_TextImage* CUI_TextImage::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_TextImage* pInstance = new CUI_TextImage(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_TextImage");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_TextImage::Clone(void* pArg)
{
	CUI_TextImage* pInstance = new CUI_TextImage(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_TextImage");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_TextImage::Pool()
{
	return new CUI_TextImage(*this);
}

void CUI_TextImage::Free()
{
	__super::Free();
}
