#include "stdafx.h"
#include "UI_Interaction.h"
#include "GameInstance.h"
#include "Json_Utility.h"


CUI_Interaction::CUI_Interaction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	:CUI(pDevice, pContext, strPrototypeTag)
{

}

CUI_Interaction::CUI_Interaction(const CUI_Interaction& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Interaction::Initialize_Prototype()
{
	//TODO ������ü�� �ʱ�ȭ������ �����Ѵ�.
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/

	return S_OK;
}

HRESULT CUI_Interaction::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_tUIInfo = *(UI_DESC*)pArg;

	m_tUIInfo.fScaleX = 60.f;
	m_tUIInfo.fScaleY = 60.f;
	m_fOriginScaleX = m_tUIInfo.fScaleX;
	m_fOriginScaleY = m_tUIInfo.fScaleY;

	//m_tUIInfo.bWorld = true;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(&m_tUIInfo))) //!  Ʈ������ ����, m_tUIInfo�� bWorldUI �� false �� ��쿡�� ������ġ ����
		return E_FAIL;

	m_bActive = false;
	//m_tUIInfo.bWorld = true;
	m_vAxis = { 0.f, 0.f, 1.f, 0.f };



	if (m_tUIInfo.strProtoTag == "")
		m_tUIInfo.strProtoTag = "UI_Icon_exclamation_mark";

	m_fActive_Distance = 10.f;

	m_tUIInfo.iRenderGroup = 18; // Back

	return S_OK;
}

void CUI_Interaction::Priority_Tick(_float fTimeDelta)
{

}

void CUI_Interaction::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pGameInstance->Key_Down(DIK_R))
		m_bOnInteraction = true;


	// ���̴� ����
	if (m_fTarget_Distance <= m_fActive_Distance && m_bOnCollision == true)
	{
		m_bAvailable = true;
	}
	else
	{
		m_bAvailable = false;
	}

	if (m_bActive == true)
	{
		if (m_bOnInteraction == true)
		{
			if (m_fScaleX < 200.f)
				Change_SizeX((+m_fChangeScale * 1.5f));

			if (m_fScaleY < 200.f)
				Change_SizeY((+m_fChangeScale * 1.5f));

			if (m_fAlpha < 1.f)
				m_fAlpha += fTimeDelta * 1.5f;

			if (m_fAlpha >= 1.f)
				m_bActive = false;
		}
	}
}

void CUI_Interaction::Late_Tick(_float fTimeDelta)
{
	//if (m_tUIInfo.bWorldUI == true)
	//	Compute_OwnerCamDistance();

	if (m_bActive == true)
	{
		if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tUIInfo.iRenderGroup, this)))
			return;
	}
}

HRESULT CUI_Interaction::Render()
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
	}

	return S_OK;
}

void CUI_Interaction::UI_Ready(_float fTimeDelta)
{

}

void CUI_Interaction::UI_Enter(_float fTimeDelta)
{

}

void CUI_Interaction::UI_Loop(_float fTimeDelta)
{
}

void CUI_Interaction::UI_Exit(_float fTimeDelta)
{
}

void CUI_Interaction::Set_TargetPosition(_vector vTargetPosition)
{
	m_vTargetPosition = vTargetPosition;
}

void CUI_Interaction::Check_TargetWorld()
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

			//vTargetTemp.y += m_fOffset;
			m_World.r[CTransform::STATE_POSITION] = XMLoadFloat4(&vTargetTemp);

			m_pTransformCom->Set_WorldMatrix(m_World);
			//m_pGameInstance->Add_RenderGroup(CRenderer::RENDER_UI, this);
		}
	}
}

void CUI_Interaction::Reset_Interaction_UI()
{

	m_fAlpha = 0.f;
	m_pTransformCom->Set_Scaling(m_fOriginScaleX, m_fOriginScaleY, m_tUIInfo.fPositionZ);
	m_bAvailable = false;
	m_bOnInteraction = false;
}

HRESULT CUI_Interaction::Ready_Components()
{
	//! For.Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//! For.Com_VIBuffer
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

#pragma region ������
	/* ù Create�� �ƹ��ų� �⺻ ���������� Initialize���� �ױ׸� �������ְ�, ���õ� �ؽ�ó�� �Ľ��ϸ� �Ľ̵� �ؽ�ó�� ����ϰ� ������. */
	wstring strPrototag;
	m_pGameInstance->String_To_WString(m_tUIInfo.strProtoTag, strPrototag);

	//! For.Com_Texture2 // ��Ȱ��ȭ
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
		TEXT("Com_Texture_Cooldown"), reinterpret_cast<CComponent**>(&m_pTextureCom[NONACTIVE]))))
		return E_FAIL;

	wstring strActive = TEXT("_active");
	strPrototag += strActive;

	//! For.Com_Texture2 // Ȱ��ȭ
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strPrototag,
		TEXT("Com_Texture_Active"), reinterpret_cast<CComponent**>(&m_pTextureCom[ACTIVE]))))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CUI_Interaction::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (m_bAvailable == true)
	{
		if (FAILED(m_pTextureCom[ACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTextureCom[NONACTIVE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	return S_OK;
}

json CUI_Interaction::Save_Desc(json& out_json)
{
	/* �⺻���� ���� */
	__super::Save_Desc(out_json);

	return out_json;
}

void CUI_Interaction::Load_Desc()
{

}

CUI_Interaction* CUI_Interaction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CUI_Interaction* pInstance = new CUI_Interaction(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Interaction");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CUI_Interaction::Pool()
{
	return new CUI_Interaction(*this);
}

CGameObject* CUI_Interaction::Clone(void* pArg)
{
	CUI_Interaction* pInstance = new CUI_Interaction(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Interaction");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Interaction::Free()
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
