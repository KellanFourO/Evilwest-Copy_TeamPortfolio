#include "stdafx.h"
#include "UI_AimCrosshair.h"
#include "GameInstance.h"
#include "Json_Utility.h"

CUI_AimCrosshair::CUI_AimCrosshair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_AimCrosshair::CUI_AimCrosshair(const CUI_AimCrosshair& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_AimCrosshair::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_AimCrosshair::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fScaleX = 20.0f;
	m_tUIInfo.fScaleY = 20.0f;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	m_ptOffset = { 0, 0 };

	m_eType = UITYPE::CROSSHAIR;
	m_bActive = true;

	return S_OK;
}

void CUI_AimCrosshair::Priority_Tick(_float fTimeDelta)
{

}

void CUI_AimCrosshair::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_pGameInstance->Key_Pressing(DIK_F))
		{
			m_vOffset.x -= 0.1f;
			m_vOffset.y -= 0.1f;
		}
		if (m_pGameInstance->Key_Pressing(DIK_G))
		{
			m_vOffset.x += 0.1f;
			m_vOffset.y += 0.1f;
		}

		if (m_pGameInstance->Key_Pressing(DIK_R))
		{
			m_vRecoil.x -= 0.1f;
			m_vRecoil.y -= 0.1f;
		}
		if (m_pGameInstance->Key_Pressing(DIK_T))
		{
			m_vRecoil.x += 0.1f;
			m_vRecoil.y += 0.1f;
		}

		GetCursorPos(&m_ptMouse);
		ScreenToClient(g_hWnd, &m_ptMouse);  // Ŭ���̾�Ʈ ���� ���콺 ������ �������� 

		m_fPositionX = m_ptMouse.x + m_ptOffset.x;
		m_fPositionY = m_ptMouse.y + m_ptOffset.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_fPositionX - g_iWinSizeX * 0.5f, -(m_fPositionY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		__super::Tick(fTimeDelta);
	}
}

void CUI_AimCrosshair::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_AimCrosshair::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	//! �� ���̴��� 0��° �н��� �׸��ž�.
	m_pShaderCom->Begin(4); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

	//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
	m_pVIBufferCom->Bind_VIBuffers();

	//! ���ε��� ����, �ε����� �׷�
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_AimCrosshair::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	//! For.Com_Texture Top
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_crosshair_pick"),
		TEXT("Com_Texture_Aim_Top"), reinterpret_cast<CComponent**>(&m_pTextureCom[AIM_TOP]))))
		return E_FAIL;

	//! For.Com_Texture Bottom
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_crosshair_pick_bottom"),
		TEXT("Com_Texture_Aim_Bottom"), reinterpret_cast<CComponent**>(&m_pTextureCom[AIM_BOTTOM]))))
		return E_FAIL;

	//! For.Com_Texture Left
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_crosshair_pick_left"),
		TEXT("Com_Texture_Aim_Left"), reinterpret_cast<CComponent**>(&m_pTextureCom[AIM_LEFT]))))
		return E_FAIL;

	//! For.Com_Texture Right
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_crosshair_pick_right"),
		TEXT("Com_Texture_Aim_Right"), reinterpret_cast<CComponent**>(&m_pTextureCom[AIM_RIGHT]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_AimCrosshair::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;	

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Recoil", &m_vRecoil, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Offset", &m_vOffset, sizeof(_float2))))
		return E_FAIL;


	if (FAILED(m_pTextureCom[AIM_TOP]->Bind_ShaderResource(m_pShaderCom, "g_AimTop_Texture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[AIM_BOTTOM]->Bind_ShaderResource(m_pShaderCom, "g_AimBottom_Texture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[AIM_LEFT]->Bind_ShaderResource(m_pShaderCom, "g_AimLeft_Texture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom[AIM_RIGHT]->Bind_ShaderResource(m_pShaderCom, "g_AimRight_Texture")))
		return E_FAIL;

	return S_OK;
}

json CUI_AimCrosshair::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_AimCrosshair::Load_Desc()
{

}

CUI_AimCrosshair* CUI_AimCrosshair::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_AimCrosshair* pInstance = new CUI_AimCrosshair(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_AimCrosshair");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_AimCrosshair::Pool()
{
	return new CUI_AimCrosshair(*this);
}

CGameObject* CUI_AimCrosshair::Clone(void* pArg)
{
	CUI_AimCrosshair* pInstance = new CUI_AimCrosshair(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_AimCrosshair");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_AimCrosshair::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);

	for (auto& pTexture : m_pTextureCom)
	{
		if (pTexture != nullptr)
			Safe_Release(pTexture);
	}

}
