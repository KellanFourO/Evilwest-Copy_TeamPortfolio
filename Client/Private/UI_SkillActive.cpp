#include "stdafx.h"
#include "UI_SkillActive.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "UI_Manager.h"

CUI_SkillActive::CUI_SkillActive(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_SkillActive::CUI_SkillActive(const CUI_SkillActive& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SkillActive::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_SkillActive::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	m_vCenter.x = 0.5;
	m_vCenter.y = 0.5;

	m_bActive = true;

	m_iMaskNum = 128;

	m_fMaxCoolTime = 2.f;
	m_fCoolTime = m_fMaxCoolTime;
	m_bCoolDown = true;

	m_fOriginScaleX = m_tUIInfo.fScaleX;
	m_fOriginScaleY = m_tUIInfo.fScaleY;

	return S_OK;
}

void CUI_SkillActive::Priority_Tick(_float fTimeDelta)
{

}

void CUI_SkillActive::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bActive == true)
	{
		if (m_pGameInstance->Key_Down(DIK_G))
		{
			m_fOriginScaleX = m_tUIInfo.fScaleX;
			m_fOriginScaleY = m_tUIInfo.fScaleY;
		}
		if (m_pGameInstance->Key_Down(DIK_SPACE))
		{
			m_fAlpha = 0.f;
			m_fCoolTime = m_fMaxCoolTime;
			m_bMaxCoolDown = false;
			m_bCoolDown = true;

			m_pTransformCom->Set_Scaling(m_fOriginScaleX, m_fOriginScaleY, 0.1);
		}

		if (m_bMaxCoolDown == false)
		{
			// Ȱ��ȭ
			if (m_pGameInstance->Key_Pressing(DIK_SPACE))
			{
				if (m_bCoolDown == true &&
					m_bMaxCoolDown == false)
				{
					m_fCoolTime -= fTimeDelta; // ���ҽ�ų���� �������� ������ (�ؽ�ó�� ������)
					m_iShaderNum = 5; // ���� ������ pass
				}

				if (m_fCoolTime <= 0.f) // ���� á�� �� (0)
				{
					m_bCoolDown = false;
					m_bMaxCoolDown = true;

					// Ȱ��ȭ
					Check_SkillActive(fTimeDelta, SKILLSTATE::ACTIVE);
				}
				else
				{
					// ��Ȱ��ȭ
					Check_SkillActive(fTimeDelta, SKILLSTATE::COOLDOWN);
				}
			}
			else
			{
				if (m_fCoolTime < m_fMaxCoolTime)
					m_fCoolTime += fTimeDelta; // ������ų���� �������� ���ҵ� (�ؽ�ó�� �����)
			}
		}
		else
		{
			{
				if (m_fScaleX < 180.f)
					Change_SizeX((+m_fChangeScale * 1.5f));

				if (m_fScaleY < 180.f)
					Change_SizeY((+m_fChangeScale * 1.5f));

				if (m_fAlpha < 1.f)
					m_fAlpha += fTimeDelta * 1.5f;

				if (m_fAlpha >= 1.f)
				{

				}
			}
		}

	}
}

void CUI_SkillActive::Late_Tick(_float fTimeDelta)
{
	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_SkillActive::Render()
{
	if (m_bActive == true)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		//! �� ���̴��� 0��° �н��� �׸��ž�.
		m_pShaderCom->Begin(5); //! Shader_PosTex 7�� �н� = VS_MAIN,  PS_UI_HP

		//! ���� �׸����� �ϴ� ����, �ε��� ���۸� ��ġ�� ���ε���
		m_pVIBufferCom->Bind_VIBuffers();

		//! ���ε��� ����, �ε����� �׷�
		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_SkillActive::UI_Ready(_float fTimeDelta)
{
}

void CUI_SkillActive::UI_Enter(_float fTimeDelta)
{
}

void CUI_SkillActive::UI_Loop(_float fTimeDelta)
{
}

void CUI_SkillActive::UI_Exit(_float fTimeDelta)
{
}

HRESULT CUI_SkillActive::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	//! For.Com_Texture2
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("hold_select_new"),
		TEXT("Com_Texture_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
		return E_FAIL;

	//! For.Com_Distortion_Mask
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mask"),
		TEXT("Com_Distortion_Mask"), reinterpret_cast<CComponent**>(&m_pDistortionCom[MASK]))))
		return E_FAIL;

	////! For.Com_Texture3
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillActive::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	// error : ���̴� ���Ͽ� �ִ°Ŷ� Ÿ�� �ȸ¾���.
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Center", &m_vCenter, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Radius", &m_fRadius, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_CoolTime", &m_fCoolTime, sizeof(_float))))
		return E_FAIL;

	// ���� ��Ÿ���� ����ؼ� �����ش�.
	_float fRatio = (m_fMaxCoolTime - m_fCoolTime) / m_fMaxCoolTime;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &fRatio, sizeof(_float))))
		return E_FAIL;

	//if (FAILED(m_pTextureCom[INACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
	//	return E_FAIL;
	if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pDistortionCom[MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_iMaskNum)))
		return E_FAIL;

	//if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_CoolDownTexture")))
	//	return E_FAIL;

	return S_OK;
}

void CUI_SkillActive::Check_SkillActive(_float fTimeDelta, SKILLSTATE eState)
{


}


json CUI_SkillActive::Save_Desc(json& out_json)
{
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_SkillActive::Load_Desc()
{

}

CUI_SkillActive* CUI_SkillActive::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_SkillActive* pInstance = new CUI_SkillActive(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_SkillActive");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_SkillActive::Pool()
{
	return new CUI_SkillActive(*this);
}

CGameObject* CUI_SkillActive::Clone(void* pArg)
{
	CUI_SkillActive* pInstance = new CUI_SkillActive(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_SkillActive");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_SkillActive::Free()
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
