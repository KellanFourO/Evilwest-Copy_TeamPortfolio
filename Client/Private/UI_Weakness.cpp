#include "stdafx.h"
#include "UI_Weakness.h"
#include "GameInstance.h"
#include "Json_Utility.h"
#include "GameObject.h"

CUI_Weakness::CUI_Weakness(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Weakness::CUI_Weakness(const CUI_Weakness& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Weakness::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Weakness::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	//m_tUIInfo.bWorld = true;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	m_bActive = false;
	m_tUIInfo.bWorld = true;

	return S_OK;
}

void CUI_Weakness::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Weakness::Tick(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_V))
		m_fOffset -= 0.1f;
	if (m_pGameInstance->Key_Down(DIK_B))
		m_fOffset += 0.1f;
	if (m_pGameInstance->Key_Down(DIK_C))
		m_bActive = !m_bActive;
	
	__super::Tick(fTimeDelta);
}

void CUI_Weakness::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	//Check_TargetWorld();
	//SetUp_BillBoarding();

	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this)))
		return;
}

HRESULT CUI_Weakness::Render()
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

void CUI_Weakness::Set_TargetPosition(_vector vTargetPosition)
{
	m_vTargetPosition = vTargetPosition;
}

void CUI_Weakness::Check_TargetWorld()
{
	if (m_tUIInfo.bWorld == true)
	{
		if (m_bActive == false)
			return;

		// ü�¹ٸ� ����.
		_float4 vCamPos = m_pGameInstance->Get_CamPosition();
		_vector vTempForDistance = m_pTransformCom->Get_Position() = XMLoadFloat4(&vCamPos);
		_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance));

		// Distance�� 0.1f���� Ŭ��츸 ���.
		if (fDistance > 0.1f)
		{
			_float3 vTemp = m_pTransformCom->Get_Scaled();
			_vector vScale = XMVectorSet(vTemp.x, vTemp.y, vTemp.z, 0.f);

			_vector vTargetPos = m_vTargetPosition;
			_float4 vTargetTemp;
			XMStoreFloat4(&vTargetTemp, vTargetPos);
			vCamPos.y = vTargetTemp.y;

			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Position() - XMLoadFloat4(&vCamPos));
			_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
			_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			//_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)); // Y�����带 ���� ���� Up�� 0, 1, 0���� ������

			m_World.r[CTransform::STATE_RIGHT] = XMVectorScale(vRight, vTemp.x);
			m_World.r[CTransform::STATE_UP] = XMVectorScale(vUp, vTemp.y);
			m_World.r[CTransform::STATE_LOOK] = XMVectorScale(vLook, vTemp.z);

			vTargetTemp.y += m_fOffset;
			m_World.r[CTransform::STATE_POSITION] = XMLoadFloat4(&vTargetTemp);

			m_pTransformCom->Set_WorldMatrix(m_World);
			//m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this);
		}
	}
}

HRESULT CUI_Weakness::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	//! For.Com_Texture
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("sparks"),
		TEXT("Com_Texture_Lock"), reinterpret_cast<CComponent**>(&m_pTextureCom[SPARKS]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Weakness::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	for (_int i = (_int)0; i < (_int)TEXTURE_END; ++i)
	{
		switch (i)
		{
		case CUI_Weakness::SPARKS:
		{
			if (FAILED(m_pTextureCom[i]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
				return E_FAIL;
			break;
		}
		default:
			break;
		}
	}

	return S_OK;
}

json CUI_Weakness::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_Weakness::Load_Desc()
{

}

CUI_Weakness* CUI_Weakness::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Weakness* pInstance = new CUI_Weakness(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Weakness");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Weakness::Pool()
{
	return new CUI_Weakness(*this);
}

CGameObject* CUI_Weakness::Clone(void* pArg)
{
	CUI_Weakness* pInstance = new CUI_Weakness(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Weakness");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Weakness::Free()
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