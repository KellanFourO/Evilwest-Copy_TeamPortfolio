#include "stdafx.h"
#include "UI_MouseCursor.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "Data_Manager.h"

CUI_MouseCursor::CUI_MouseCursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_MouseCursor::CUI_MouseCursor(const CUI_MouseCursor& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_MouseCursor::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_MouseCursor::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fScaleX = 5.0f;
	m_tUIInfo.fScaleY = 5.0f;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	m_ptOffset = { 0, 0 };

	m_eType = UITYPE::CROSSHAIR;
	m_bActive = true;
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), 0.35f);

	m_tUIInfo.iRenderGroup = 11;

	return S_OK;
}

void CUI_MouseCursor::Priority_Tick(_float fTimeDelta)
{

}

void CUI_MouseCursor::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//if (m_pGameInstance->Key_Down(DIK_V))
	//	m_pData_Manager->Set_GameState(GAME_STATE::GAMEPLAY);
	//if (m_pGameInstance->Key_Down(DIK_B))
	//	m_pData_Manager->Set_GameState(GAME_STATE::OPTION);

	if (m_bActive == true)
	{
		if (m_pData_Manager->Get_GameState() == GAME_STATE::GAMEPLAY)
		{
			m_bGamePlayMouse = true;
		}
		else if (m_pData_Manager->Get_GameState() == GAME_STATE::MAINMENU ||
			m_pData_Manager->Get_GameState() == GAME_STATE::OPTION ||
			m_pData_Manager->Get_GameState() == GAME_STATE::SKILLWINDOW ||
			m_pData_Manager->Get_GameState() == GAME_STATE::UI)
		{
			m_bGamePlayMouse = false;
		}

		if (m_bGamePlayMouse == true)
		{
			/* �� ��ǥ�� ȭ�� ���� �� ���� �����Ǵ� ��ǥ. */
			//m_fPositionX = _float(g_iWinSizeX * 0.5f);
			//m_fPositionY = _float(g_iWinSizeY * 0.5f);
			m_fPositionX = 0.0f;
			m_fPositionY = 0.0f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_fPositionX, m_fPositionY, 0.1f, 1.f));
			m_pTransformCom->Set_Scaling(4.f, 4.f, 0.1f);
		}
		else
		{
			GetCursorPos(&m_ptMouse);
			ScreenToClient(g_hWnd, &m_ptMouse);  // Ŭ���̾�Ʈ ���� ���콺 ������ �������� 

			m_fPositionX = _float(m_ptMouse.x + m_ptOffset.x);
			m_fPositionY = _float(m_ptMouse.y + m_ptOffset.y);
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_fPositionX - g_iWinSizeX * 0.5f, -(m_fPositionY - g_iWinSizeY * 0.5f), 0.1, 1.f));
			m_pTransformCom->Set_Scaling(40.f, 40.f, 0.1f);
		}
	}
}

void CUI_MouseCursor::Late_Tick(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
		return;
}

HRESULT CUI_MouseCursor::Render()
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

void CUI_MouseCursor::UI_Ready(_float fTimeDelta)
{
}

void CUI_MouseCursor::UI_Enter(_float fTimeDelta)
{
}

void CUI_MouseCursor::UI_Loop(_float fTimeDelta)
{
}

void CUI_MouseCursor::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_MouseCursor::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	//! For.Com_Texture �ΰ��� Ŀ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("ui_aim_dot"),
		TEXT("Com_Texture_GamePlay_Cursor"), reinterpret_cast<CComponent**>(&m_pTextureCom[PLAYGAME_CROSSHAIR]))))
		return E_FAIL;

	//! For.Com_Texture �ɼ� Ŀ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Keyboard_Black_Mouse_Middle"),
		TEXT("Com_Texture_Cursor"), reinterpret_cast<CComponent**>(&m_pTextureCom[CURSOR]))))
		return E_FAIL;

	//! For.Com_Texture �ɼ� Ŭ�� Ŀ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Keyboard_Black_Mouse_Left"),
		TEXT("Com_Texture_Left_ClickCursor"), reinterpret_cast<CComponent**>(&m_pTextureCom[CURSOR_LEFTCLICK]))))
		return E_FAIL;

	//! For.Com_Texture �ɼ� Ŭ�� Ŀ��
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Keyboard_Black_Mouse_Right"),
		TEXT("Com_Texture_Right_ClickCursor"), reinterpret_cast<CComponent**>(&m_pTextureCom[CURSOR_RIGHTCLICK]))))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_MouseCursor::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (m_bGamePlayMouse == true)
	{
		if (FAILED(m_pTextureCom[PLAYGAME_CROSSHAIR]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom[CURSOR]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;

		if (m_bMouseDown_LB == true ||
			m_bMousePressing_LB == true)
		{
			if (FAILED(m_pTextureCom[CURSOR_LEFTCLICK]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
		}
		else if (m_bMouseDown_RB == true ||
			m_bMousePressing_RB == true)
		{
			if (FAILED(m_pTextureCom[CURSOR_RIGHTCLICK]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
		}

	}

	return S_OK;
}

json CUI_MouseCursor::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_MouseCursor::Load_Desc()
{

}

CUI_MouseCursor* CUI_MouseCursor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_MouseCursor* pInstance = new CUI_MouseCursor(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_MouseCursor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_MouseCursor::Pool()
{
	return new CUI_MouseCursor(*this);
}

CGameObject* CUI_MouseCursor::Clone(void* pArg)
{
	CUI_MouseCursor* pInstance = new CUI_MouseCursor(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_MouseCursor");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_MouseCursor::Free()
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
