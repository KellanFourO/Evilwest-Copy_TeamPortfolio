#include "stdafx.h"
#include "..\Public\Effect_Rect.h"

#include "GameInstance.h"


CEffect_Rect::CEffect_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
	: CEffect_Void(pDevice, pContext, strPrototypeTag)
{
	m_bIsPoolObject = FALSE;
}

CEffect_Rect::CEffect_Rect(const CEffect_Rect & rhs)
	: CEffect_Void(rhs)
{
}

HRESULT CEffect_Rect::Initialize_Prototype()
{
	/* ������ü�� �ʱ�ȭ������ �����Ѵ�. */
	/* 1.�����κ��� ���� �޾ƿͼ� �ʱ�ȭ�Ѵ� .*/
	/* 2.����������� ���� ���� �޾ƿ´�.*/
	//int*		pData_int = new int(3);

	//void**		pData = (void**)&pData_int;

	return S_OK;
}

HRESULT CEffect_Rect::Initialize(void* pArg)
{	
	//m_tRectDesc = *(EFFECT_RECT_DESC*)pArg;
	//*static_cast<EFFECTVOID_DESC*>(&m_tRectDesc) = *static_cast<EFFECTVOID_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// void* pArg : �����Ͽ� ��ü�� �����Ҷ� �������� ä���� ���� �����͸� �߰������� �� �ʱ�ȭ���ֱ����ؼ�. 
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _float3(0.f, 0.f, 1.f));	

	return S_OK;
}

void CEffect_Rect::Priority_Tick(_float fTimeDelta)
{


	
}

void CEffect_Rect::Tick(_float fTimeDelta)
{
	if (m_tVoidDesc.bActive_Tool)
	{	
		m_tVoidDesc.fSequenceTime = m_tVoidDesc.fLifeTime + m_tVoidDesc.fRemainTime;

		if (m_tVoidDesc.bPlay)
		{
			m_tVoidDesc.fSequenceAcc += fTimeDelta;

			// �������� �����ð��� ������ ���� ����(�� ����Ʈ ����)
			if (m_tVoidDesc.fWaitingAcc <= m_tVoidDesc.fWaitingTime)
			{
				m_tVoidDesc.fWaitingAcc += fTimeDelta;

				if (m_tVoidDesc.fWaitingAcc >= m_tVoidDesc.fWaitingTime)
				{
					m_tVoidDesc.bRender = TRUE;
				}
				else
					return;
			}

			// ������Ÿ�� ���� ����
			m_tVoidDesc.fTimeAcc += fTimeDelta;
			m_tVoidDesc.fLifeTimeRatio = min(1.0f, m_tVoidDesc.fTimeAcc / m_tVoidDesc.fLifeTime);

			/* ======================= ������ Ÿ�� ���� ���� ======================= */

			if (SPRITE == m_tRectDesc.eType)
			{
				if (m_tVoidDesc.fTimeAcc > m_tSpriteDesc.fSequenceTerm)
				{
					(m_tSpriteDesc.vUV_CurTileIndex.x)++;

					if (m_tSpriteDesc.vUV_CurTileIndex.x == m_tSpriteDesc.vUV_MaxTileCount.x)
					{
						(m_tSpriteDesc.vUV_CurTileIndex.y)++;
						m_tSpriteDesc.vUV_CurTileIndex.x = m_tSpriteDesc.vUV_MinTileCount.x;

						if (m_tSpriteDesc.vUV_CurTileIndex.y == m_tSpriteDesc.vUV_MaxTileCount.y)
						{
							m_tSpriteDesc.vUV_CurTileIndex.y = m_tSpriteDesc.vUV_MinTileCount.y;
						}
					}
					m_tVoidDesc.fTimeAcc = 0.f;
					
				}
				return;
			}

			/* ======================= ������ Ÿ�� ���� ��  ======================= */

			if (m_tVoidDesc.fTimeAcc >= m_tVoidDesc.fLifeTime)
			{
				// ���� ���ð� ���� ����
				m_tVoidDesc.fRemainAcc += fTimeDelta;

				// ������ ����
				m_tVoidDesc.bDissolve = TRUE;
				if (m_tVoidDesc.bDissolve)
				{
					m_tVoidDesc.fDissolveAmount = Easing::LerpToType(0.f, 1.f, m_tVoidDesc.fRemainAcc, m_tVoidDesc.fRemainTime, m_tVoidDesc.eType_Easing);
				}
				// ������ ��


				// �� ����Ʈ�� Ÿ�Ӷ��� ����
				if (m_tVoidDesc.fRemainAcc >= m_tVoidDesc.fRemainTime)
				{
					m_tVoidDesc.fDissolveAmount = 1.f;
					m_tVoidDesc.bRender = TRUE;
					return;
				}
			}
		}
	}

};

void CEffect_Rect::Late_Tick(_float fTimeDelta)
{
	Compute_CamDistance();

	if (m_tVoidDesc.bActive_Tool)
	{
		if (m_tVoidDesc.bRender)
		{

			__super::Update_PivotMat();


			if (FAILED(m_pGameInstance->Add_RenderGroup((CRenderer::RENDERGROUP)m_tVoidDesc.iRenderGroup, this)))
				return;
		}
	}

}

HRESULT CEffect_Rect::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* �� ���̴��� 0��° �н��� �׸��ž�. */
	m_pShaderCom->Begin(m_tVoidDesc.iShaderPassIndex);

	/* ���� �׸������ϴ� ����, �ε������۸� ��ġ�� ���ε���. */
	m_pVIBufferCom->Bind_VIBuffers();

	/* ���ε��� ����, �ε����� �׷�. */
	m_pVIBufferCom->Render();

	return S_OK;
}

void CEffect_Rect::ReSet_Effect()
{
	if (SPRITE == m_tRectDesc.eType)
	{
		m_tSpriteDesc.vUV_CurTileIndex.y = m_tSpriteDesc.vUV_MinTileCount.y;
		m_tSpriteDesc.vUV_CurTileIndex.x = m_tSpriteDesc.vUV_MinTileCount.x;
	}
	else
	{
		__super::ReSet_Effect();

		m_tVoidDesc.fDissolveAmount = 0.f;
		m_tVoidDesc.bDissolve		= FALSE;
		//m_tRectDesc.bRender			= FALSE;
	}

}

void CEffect_Rect::End_Effect()
{
	__super::End_Effect();

	if (m_tVoidDesc.bLoop)
	{
		ReSet_Effect();
	}
}


HRESULT CEffect_Rect::Ready_Components()
{
	_uint iNextLevel = m_pGameInstance->Get_NextLevel();

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Shader_EffectTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	{
		//if (SINGLE == m_tRectDesc.eType)
		{		
			if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Texture_Effect_Diffuse"),
				TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_DIFFUSE]))))
				return E_FAIL;
		}
		//else
		{
			if (FAILED(__super::Add_Component(iNextLevel, TEXT("Prototype_Component_Texture_Effect_Sprite"),
				TEXT("Com_Sprite"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_SPRITE]))))
				return E_FAIL;
		}

		if (TEXT("") != m_tVoidDesc.strTextureTag[TEXTURE_MASK])
		{
			/* For.Com_Mask */
			if (FAILED(__super::Add_Component(iNextLevel, m_tVoidDesc.strTextureTag[TEXTURE_MASK],
				TEXT("Com_Mask"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_MASK]))))
				return E_FAIL;
		}

		if (TEXT("") != m_tVoidDesc.strTextureTag[TEXTURE_NOISE])
		{
			/* For.Com_Noise */
			if (FAILED(__super::Add_Component(iNextLevel, m_tVoidDesc.strTextureTag[TEXTURE_NOISE],
				TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEXTURE_NOISE]))))
				return E_FAIL;
		}
	}



	return S_OK;
}

HRESULT CEffect_Rect::Bind_ShaderResources()
{
	
	/* Matrix ============================================================================================ */
	if (m_tVoidDesc.bParentPivot)
	{
		FAILED_CHECK(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_tVoidDesc.matCombined));
	}
	else
	{
		FAILED_CHECK(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix"));
	}

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (SINGLE == m_tRectDesc.eType)
	{
		FAILED_CHECK(m_pTextureCom[TEXTURE_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_tVoidDesc.iTextureIndex[TEXTURE_DIFFUSE]));
	}
	else
	{
		FAILED_CHECK(m_pTextureCom[TEXTURE_SPRITE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_tVoidDesc.iTextureIndex[TEXTURE_SPRITE]));
	}


	if (nullptr != m_pTextureCom[TEXTURE_MASK])
	{
		if (FAILED(m_pTextureCom[TEXTURE_MASK]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", m_tVoidDesc.iTextureIndex[TEXTURE_MASK])))
			return E_FAIL;
	}
	if (nullptr != m_pTextureCom[TEXTURE_NOISE])
	{
		if (FAILED(m_pTextureCom[TEXTURE_NOISE]->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", m_tVoidDesc.iTextureIndex[TEXTURE_NOISE])))
			return E_FAIL;
	}


	/* Camera ============================================================================================ */
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4)));
	_float3 vCamDirectionFloat3 = m_pGameInstance->Get_CamDirection();
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_vCamDirection", &vCamDirectionFloat3, sizeof(_float3)));

	_float fCamFar = m_pGameInstance->Get_CamFar();
	FAILED_CHECK(m_pShaderCom->Bind_RawValue("g_fCamFar", &fCamFar, sizeof(_float)));



	if (SINGLE == m_tRectDesc.eType)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fFrameTime", &m_tVoidDesc.fTimeAcc, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vScrollSpeeds", &m_tDistortionDesc.vScrollSpeeds, sizeof(_float3))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vScales", &m_tDistortionDesc.vScales, sizeof(_float3))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vDistortion1", &m_tDistortionDesc.vDistortion1, sizeof(_float2))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vDistortion2", &m_tDistortionDesc.vDistortion2, sizeof(_float2))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vDistortion3", &m_tDistortionDesc.vDistortion3, sizeof(_float2))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDistortionScale", &m_tDistortionDesc.fDistortionScale, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fDistortionBias", &m_tDistortionDesc.fDistortionBias, sizeof(_float))))
			return E_FAIL;
	}
	else
	{
		m_tVoidDesc.vUV_Offset = { (_float)(m_tSpriteDesc.vUV_CurTileIndex.x * m_tSpriteDesc.vTileSize.x) / m_tSpriteDesc.vTextureSize.x
								, (_float)(m_tSpriteDesc.vUV_CurTileIndex.y * m_tSpriteDesc.vTileSize.y) / m_tSpriteDesc.vTextureSize.y };

		m_tVoidDesc.vUV_Scale = { (_float)m_tSpriteDesc.vTileSize.x / m_tSpriteDesc.vTextureSize.x
								, (_float)m_tSpriteDesc.vTileSize.y / m_tSpriteDesc.vTextureSize.y };


		if (FAILED(m_pShaderCom->Bind_RawValue("g_UVOffset", &m_tVoidDesc.vUV_Offset, sizeof(_float2))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_UVScale", &m_tVoidDesc.vUV_Scale, sizeof(_float2))))
			return E_FAIL;
	}


	if (FAILED(m_pShaderCom->Bind_RawValue("g_DiscardValue", &m_tVoidDesc.vColor_Clip.w, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Depth"), m_pShaderCom, "g_DepthTexture")))
		return E_FAIL;

	return S_OK;
}

CEffect_Rect * CEffect_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPrototypeTag)
{
	CEffect_Rect*		pInstance = new CEffect_Rect(pDevice, pContext, strPrototypeTag);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CEffect_Rect::Clone(void* pArg)
{
	CEffect_Rect*		pInstance = new CEffect_Rect(*this);

	/* ������ü�� �ʱ�ȭ�Ѵ�.  */
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Rect");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CEffect_Rect::Pool()
{
	return new CEffect_Rect(*this);
}

void CEffect_Rect::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);

	for (_int i = 0; i < (_int)TEXTURE_END; i++)
		Safe_Release(m_pTextureCom[i]);

	Safe_Release(m_pShaderCom);
}

